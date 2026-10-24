"""Bounded executable certificates. Run from repository root with python -B.

No per-seed traces are retained. Every detected mismatch is kept in the routine
result JSON across reruns; a corrected model earns a new successful certificate.
"""
import argparse,json,hashlib,time,copy,traceback
from engine import *
from models import MODELS,DETAILS,TIMER,CLIP,binding
MODULES=('sprites','weather','raster','tiles','hardware','records','copies','builders','deform','joystick','callbacks','cpu','private')
def bundle_hashes():
    return {n+'.py':hashlib.sha256((HERE/(n+'.py')).read_bytes()).hexdigest() for n in ('engine','models','prove')+MODULES}

def prepare(oracle,name,seed):
    rng=random.Random(seed)
    r={n:rng.getrandbits(32) for n in REGS};r.update(esp=SP,esi=SRC+0x2000,edi=BUF+0x2000)
    inputs={};s=State(oracle.segments_for(rng),r,inputs)
    def setreg(**kw):s.r.update({k:u32(v) for k,v in kw.items()})
    def put(a,v,n=4):s.mem.put(a,v,n)
    put(SP,STOP)
    for a,v in zip(CLIP,(8,312,0,200)):put(a,v)
    w=rng.choice([1,2,3,4,5,7,8,9,15,16,17,31,32,63]);h=rng.choice([1,2,3,4,7,8,15,16])
    x=rng.randrange(16,200);y=rng.randrange(16,100)
    flags=2
    if name in ('synchro','endsynchro'):
        bit=8 if name=='synchro' else 0
        s.inputs[0x3da]=[(rng.randrange(256)&247)|(bit^8) for _ in range(seed%7)]+[(rng.randrange(256)&247)|bit]
    elif name in ('set_speaker_on','set_speaker_off','speaker_enable'):s.inputs[0x61]=[rng.randrange(256)]
    elif name=='sprite_clipping':
        edge=[-2147483648,-321,-1,0,1,199,200,201,319,320,321,2147483647]
        for n in ('eax','ebx','ecx','edx'):s.r[n]=u32(rng.choice(edge) if seed%2 else rng.randint(-400,700))
    elif name=='get_sprite_clipping':
        setreg(eax=AUX+16,ebx=AUX+20,ecx=AUX+24,edx=AUX+28)
        for a in CLIP:put(a,rng.randrange(400))
    elif name in ('clock_ticks','wait_until'):
        period=rng.choice([0,65535])
        counter=rng.choice([0,1,65534,65535,rng.randrange(1,65535)]);delta=period-counter;mode=seed%4
        last=delta-1 if mode==0 else delta+1 if mode==1 else delta
        repeats=rng.choice([0,1,19,20,21]);epoch=rng.choice([0,1,0x7fffffff,0xfffffff0,rng.randrange(100000)])
        for a,v in zip(TIMER,(period,last,epoch,repeats)):put(a,v)
        counters=[counter]
        if name=='wait_until':
            # Valid monotonic PIT countdown; exercise 1..5 polls and the disabled path.
            last=0;epoch=0;repeats=0
            for a,v in zip(TIMER,(period,last,epoch,repeats)):put(a,v)
            polls=1+seed%5;counter=60000;counters=[counter-i*100 for i in range(polls+1)]
            setreg(ebx=65535-counter+(polls-1)*100-1)
        s.inputs[0x42]=[v for c in counters for v in (c&255,c>>8)]
    elif name=='sub_2748':s.inputs[('int',0x1a)]={'ecx':rng.getrandbits(32),'edx':rng.getrandbits(32)}
    elif name=='copy_dwords':
        setreg(ecx=rng.choice([0,1,2,3,17,64,256]));direction=1 if seed%2 else -1;s.inputs['direction']=direction
        if seed%3==0:s.r['edi']=s.r['esi']+rng.choice([-16,-4,0,4,16])
        flags=2 if direction==1 else 0x402
    elif name=='asm_0283c':setreg(eax=rng.getrandbits(32))
    elif name=='select_display_buffer':setreg(eax=rng.choice([0xa0500,0xa6500]))
    elif name=='DrawBlackBoxNormal':setreg(eax=x,ebx=y,ecx=h,edx=w)
    elif name=='DrawBorderBoxNormal':setreg(eax=rng.getrandbits(32),esi=x,ebx=y,ecx=max(2,h),edx=w)
    elif name=='DrawFondBoxNormal':setreg(eax=rng.choice([0,1,2,3]),esi=x,ebx=y,ecx=h,edx=w)
    elif name=='DrawWldPointPlan2Normal':setreg(eax=x,ebx=y)
    elif name=='DisplayAnyPictureNormal':
        setreg(eax=rng.randrange(8),ebx=rng.randrange(8),ecx=x,edx=y)
        for i,v in zip((4,8,12),(rng.choice([128,256,320,384]),w,h)):put(SP+i,v)
    elif name=='Swap_To_Screen':setreg(eax=h,ecx=w)
    elif name=='clear_borders_Normal':setreg(ecx=rng.randrange(2,25),edx=rng.randrange(32,280))
    elif name=='flocon_clipping':setreg(eax=rng.randrange(16),ebx=rng.randrange(160,200),ecx=rng.randrange(16),edx=rng.randrange(280,320))
    elif name=='fplot_Normal':setreg(eax=x,ebx=y,esi=rng.randrange(256))
    # Families registered by supplemental model modules can constrain their own inputs.
    for module in MODULES:
        try:
            mod=__import__(module)
            if hasattr(mod,'prepare'):mod.prepare(s,name,rng,seed)
        except ModuleNotFoundError as e:
            if e.name!=module:raise
    if name=='asm_0275c':flags=2 if s.inputs['direction']==1 else 0x402
    if name.startswith(('get_joy_input','dos_game_io_port')):s.flags=0x200;s.flagmask=0x600
    if name=='asm_037e4':s.flags=0x200;s.flagmask=0x600
    s.mem.writes.clear()
    return s,flags

def run(names=None,count=64,start=0xA550000):
    for module in MODULES:
        try:__import__(module)
        except ModuleNotFoundError as e:
            if e.name!=module:raise
    inv=json.loads((HERE/'inventory.json').read_text());byname={f['name']:f for f in inv}
    oracle=Oracle();total=0;failed=0
    for name in sorted(names or MODELS,key=lambda n:(-byname[n]['callsite_count'],byname[n]['offset'])):
        if name not in MODELS:print(name,'OPEN: no model',flush=True);continue
        off=byname[name]['offset'];cfg=oracle.cfg(off);visited=set();blocks=set();mismatches=[];cases=0;written=set();io_cases=0;beg=time.time();changed=set();vga_count=0;read_io=set()
        path=HERE/'results'/f'{name}.json';path.parent.mkdir(exist_ok=True)
        old=json.loads(path.read_text()) if path.exists() else {}
        for i in range(count):
            seed=start+i
            try:
                initial,flags=prepare(oracle,name,seed)
                expected=copy.deepcopy(initial);MODELS[name](expected)
                actual=oracle.run(off,initial,flags)
                errors=compare(expected,actual)
                visited.update(actual['instructions']);blocks.update(actual['blocks']);written.update(actual['writes']);io_cases+=bool(actual['io'])
                changed.update(n for n in REGS if n!='esp' and initial.r[n]!=actual['regs'][n]);vga_count+=len(actual['vga_writes'])
                read_io.update(tuple(item[:3]) for item in actual['io'])
                if errors:mismatches.append(dict(seed=seed,errors=errors));break
                cases+=1
            except Exception as e:
                mismatches.append(dict(seed=seed,errors=[{'kind':'harness_or_model_exception','detail':repr(e),'traceback':traceback.format_exc()}]));break
        history=old.get('mismatch_history',[])
        if mismatches:history.append({'script_sha256s':bundle_hashes(),'mismatches':mismatches})
        result=dict(name=name,address=byname[name]['address'],status='EQUIVALENT_TESTED' if not mismatches else 'MISMATCH',
            cases=cases,seed_start=start,seed_end=start+cases-1,requested_cases=count,
            comparison={'all_GPRs':True,'all_written_bytes_including_stack':True,'written_byte_address_sets':True,'exact_IO_sequence':True,'CF':expected.cf is not None if 'expected' in locals() else False,'flag_mask':hex(expected.flagmask) if 'expected' in locals() else None,'ES':name=='bios_wait_timer'},
            basic_blocks={'covered':len(cfg['blocks']&visited),'total':len(cfg['blocks']),'percent':round(100*len(cfg['blocks']&visited)/len(cfg['blocks']),2),
                'uncovered':[f'1:{x:05x}' for x in sorted(cfg['blocks']-visited)],'unresolved_indirect_jumps':[f'1:{x:05x}' for x in cfg['unresolved']]},
            instructions={'covered':len(cfg['instructions']&visited),'total':len(cfg['instructions'])},
            unique_written_bytes=len(written),cases_with_IO=io_cases,mismatches=mismatches,mismatch_history=history,
            script_sha256s=bundle_hashes(),register_behavior={'changed':sorted(changed),'preserved_in_all_cases':sorted(set(REGS)-changed-{'esp'}),'ESP':'entry+16' if name=='DisplayAnyPictureNormal' else 'entry+4'},logical_VGA_plane_byte_checks=vga_count,
            elapsed_seconds=round(time.time()-beg,3),oracle=oracle.image.name,
            proof_level='finite differential execution of independent portable model; not universal formal proof; not STRICT')
        path.write_text(json.dumps(result,indent=2)+'\n')
        total+=cases;failed+=bool(mismatches)
        print(f"{name}: {result['status']} {cases}/{count}; blocks {result['basic_blocks']['covered']}/{result['basic_blocks']['total']}; {result['elapsed_seconds']}s",flush=True)
        if mismatches:print(json.dumps(mismatches[0]['errors'])[:1500],flush=True)
    print('TOTAL passed cases',total,'failed routines',failed,flush=True)
    return bool(failed)

if __name__=='__main__':
    p=argparse.ArgumentParser();p.add_argument('names',nargs='*');p.add_argument('--count',type=int,default=64);p.add_argument('--seed',type=lambda s:int(s,0),default=0xA550000)
    a=p.parse_args();sys.exit(run(a.names or None,a.count,a.seed))
