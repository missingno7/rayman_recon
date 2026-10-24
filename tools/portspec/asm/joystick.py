from models import *
XY=[binding('joyx0'),binding('joyy0')];CENTRE=[binding('xpadcentre'),binding('ypadcentre')]
BUTTONS=[binding('joy_button'+n) for n in ('A1','A2','B1','B2')]
MENU=binding('During_The_Menu');START=BASE[1]+0x5959
def buttons(s,v,pair):
    order=(0,1,2,3) if pair==1 else (2,3,0,1)
    for bit,index in enumerate(order,4):s.mem.put(BUTTONS[index],1-((v>>bit)&1),2)
def fallback(s):
    for a,b in zip(XY,CENTRE):s.mem.put(a,s.mem.get(b,2),2)
def tick(s,retaddr):s.push(BASE[1]+retaddr);clock_ticks(s)

def detect(s,name):
    s.pushad();s.out(0x201,255)
    pair=2 if name.endswith('2') else 1;mask=12 if pair==2 else 3
    if pair==2:s.inp(0x201) # discarded extra first sample at1:05d26
    polls=0
    for i in range(2000):
        polls+=1
        if not s.inp(0x201)&mask:break
    else:polls=0
    s.popad();s.r['eax']=int(polls>1);ret(s)
for name in ('dos_game_io_port_something','dos_game_io_port_something2'):
    def fn(s,n=name):detect(s,n)
    model(name,'Trigger game-port discharge with OUT0x201=255; '+('discard one initial read; ' if name.endswith('2') else '')+'poll IN0x201 until axis mask'+('12' if name.endswith('2') else '3')+' clears. Return EAX=1 only if clearance takes2..2000 polls, otherwise0. Preserve other GPRs; finish IF=1,DF=0.','Finite scripted RC response, including immediate-clear and2000-poll timeout.')(fn)

def poll(s,name):
    r=s.r;pair=2 if name.endswith('2') else 1;mask=12 if pair==2 else 3;bits=(4,8) if pair==2 else (1,2);windows='windows' in name
    s.pushad()
    if windows:
        for a in XY:s.mem.put(a,0,2)
    r['eax']=0;r['edx']=setlo(r['edx'],0x201,16);v=s.inp(0x201);r['eax']=v;r['ebx']=v;buttons(s,v,pair)
    if not windows:
        r['eax']=s.mem.get(MENU,2)
        if r['eax']==1:s.popad();ret(s);return
    r['ebx']&=mask
    if r['ebx']:fallback(s)
    else:
        if not windows:
            for a in XY:s.mem.put(a,0,2)
        r['ebx']=2000
        if not windows:
            tick(s,0x59e3 if pair==1 else 0x5ae1);s.mem.put(START,r['eax'])
        r['eax']=setlo(r['eax'],255);s.out(0x201,255)
        for i in range(2000):
            r['eax']=setlo(r['eax'],s.inp(0x201));r['ebp']=r['eax']
            for axis,bit in enumerate(bits):
                r['eax']=r['ebp']&bit
                if not windows or pair==2:r['eax']=setlo(r['eax'],r['eax']+s.mem.get(XY[axis],2),16)
                if windows:
                    if r['eax']&65535:s.mem.put(XY[axis],s.mem.get(XY[axis],2)+1,2)
                elif not r['eax']&65535:
                    site={(1,0):0x5a36,(1,1):0x5a49,(2,0):0x5b34,(2,1):0x5b47}[(pair,axis)]
                    tick(s,site);r['eax']=u32(r['eax']-s.mem.get(START));s.mem.put(XY[axis],r['eax'],2)
            r['ebx']-=1
            if not r['ebx']:fallback(s);break
            r['ebp']&=mask
            if not r['ebp']:break
    s.popad();ret(s)

for name in ('get_joy_input2_dos1','get_joy_input2_dos2','get_joy_input2_windows1','get_joy_input2_windows2'):
    def fn(s,n=name):poll(s,n)
    windows='windows' in name;pair=2 if name.endswith('2') else 1
    desc='Read active-low buttons at port0x201 bits4..7, mapping to'+('B1,B2,A1,A2' if pair==2 else 'A1,A2,B1,B2')+'. '
    desc+=('Zero both axes first. Count poll iterations ' if windows else 'If During_The_Menu==1 leave axes unchanged. Otherwise zero axes and time bit-clear events using clock_ticks relative to initial timestamp. ')
    desc+=('Only increment each axis while its own bit is set. ' if windows and pair==1 else 'Axis update test is (axis_bit+current_u16_axis)!=0, so Windows pair2 continues incrementing a nonzero counter even after its bit clears. ' if windows else 'Record elapsed low16 ticks only when its bit and previous axis value are both zero. ')
    desc+='Selected axis bits are'+('2/3' if pair==2 else '0/1')+'. Initial selected bits nonzero or2000-poll timeout falls back to calibrated x/y centers. OUT0x201=255 triggers sampling. Preserve all GPRs; finish IF=1,DF=0.'
    model(name,desc,'Mapped game joystick globals; scripted port response with monotonic discharges or timeout; live PIT response abstracted.',globals_read=['During_The_Menu','xpadcentre','ypadcentre']+([] if windows else ['timer state']),globals_written=['joyx0','joyy0','joy_buttonA1/A2/B1/B2']+([] if windows else ['code1:05959 timer start','timer state']))(fn)

def sample_axes(s,pair,sites):
    r=s.r;bits=(4,8) if pair==2 else (1,2)
    r['eax']=setlo(r['eax'],s.inp(0x201));r['ebp']=r['eax']
    for axis,bit in enumerate(bits):
        r['eax']=setlo(r['ebp']&bit,(r['ebp']&bit)+s.mem.get(XY[axis],2),16)
        if not r['eax']&65535:
            tick(s,sites[axis]);r['eax']=u32(r['eax']-s.mem.get(START));s.mem.put(XY[axis],r['eax'],2)

def frame_copy(s,name):
    r=s.r;pair=2 if name.endswith('2') else 1;mask=12 if pair==2 else 3;s.pushad();r['esp']-=12;s.mem.put(r['esp']+4,r['eax'])
    for a in XY:s.mem.put(a,0,2)
    r['eax']=0;r['edx']=setlo(r['edx'],0x201,16);v=s.inp(0x201);r['ebp']=v;buttons(s,v,1);r['eax']=s.mem.get(r['esp']+4);r['ebp']&=mask
    r['ecx']>>=2;s.mem.put(r['esp'],r['ecx']);n=r['ecx'];source_pitch=s.mem.get(BASE[1]+0x5951);dest_pitch=s.mem.get(BASE[1]+0x5955)
    invalid=bool(r['ebp'])
    if invalid:fallback(s)
    else:
        s.mem.put(r['esp']+4,r['eax']);tick(s,0x5dd8 if pair==1 else 0x5fc2);s.mem.put(START,r['eax']);r['eax']=setlo(r['eax'],255);s.out(0x201,255);r['eax']=s.mem.get(r['esp']+4)
    r['ebx']=r['esi'];r['edx']=r['edi']
    def copy_row():
        r['ecx']=n;s.movs(4,n);r['ecx']=0;r['ebx']+=source_pitch;r['edx']+=dest_pitch;r['esi']=r['ebx'];r['edi']=r['edx'];r['eax']-=1
    finish=invalid
    while r['eax']:
        if not finish:
            s.mem.put(r['esp']+8,r['edx']);s.mem.put(r['esp']+4,r['eax']);r['edx']=setlo(r['edx'],0x201,16)
            sample_axes(s,pair,(0x5ee1,0x5ef7) if pair==1 else (0x60cb,0x60e1))
            r['edx']=s.mem.get(r['esp']+8);r['eax']=s.mem.get(r['esp']+4);r['ebp']&=mask
            if not r['ebp']:finish=True
        copy_row()
    if not finish:
        r['ebx']=2000;r['edx']=setlo(r['edx'],0x201,16)
        while True:
            sample_axes(s,pair,(0x5f0d,0x5f23) if pair==1 else (0x60f7,0x610d));r['ebx']-=1
            if not r['ebx']:fallback(s);break
            r['ebp']&=mask
            if not r['ebp']:break
    r['esp']+=12;s.popad();ret(s)

for name in ('get_joy_input1_dos1','get_joy_input1_dos2'):
    def fn(s,n=name):frame_copy(s,n)
    model(name,'Copy EAX rows of floor(ECX/4)*4 bytes from ESI to EDI using code-owned source/destination pitches at1:05951/05955 (initially320). Zero axes and read buttons in A1,A2,B1,B2 order in BOTH variants. Sample port0x201 axis bits'+('2/3' if name.endswith('2') else '0/1')+' before each copied row until both bits clear, then finish copy without further polling. If rows end first, poll up to2000 further reads; timeout or invalid initial selected bits uses calibrated centers. Timing uses clock_ticks relative to initial latch; record each axis when bit and previous result are zero. Trigger with OUT0x201=255. Preserve GPRs, including input EAX height; final IF=1,DF=0. Exact interleaving and timer state are part of contract.','Positive frame height and mapped source/destination; width0..320; scripted monotonic device response and PIT counter.',globals_read=['code1:05951 source pitch','code1:05955 destination pitch','xpadcentre','ypadcentre','timer state'],globals_written=['joy axes/buttons','code1:05959 timer start','timer state'])(fn)

def prepare(s,name,rng,seed):
    if not name.startswith(('get_joy_input','dos_game_io_port')):return
    pair=2 if name.endswith('2') else 1;mask=12 if pair==2 else 3;bits=(4,8) if pair==2 else (1,2);mode=seed%6
    for a in XY+CENTRE:s.mem.put(a,rng.randrange(2000),2)
    s.mem.put(MENU,1 if mode==0 else 0,2)
    port=rng.randrange(256)&~mask
    if name.startswith('dos_game'):
        n=1 if mode==0 else 2001 if mode==1 else rng.randrange(2,15)
        seq=[port|mask]*(n-1)+[port]
        if pair==2:seq=[port]+seq
    else:
        initial=port|mask if mode==1 else port
        seq=[initial]
        if mode==2:seq+=[port|mask]*2100
        else:
            first=rng.randrange(1,7);second=first+rng.randrange(1,7)
            order=rng.randrange(2)
            seq+=[port|mask]*first+[port|bits[order]]*(second-first)+[port]
        for a,v in zip(TIMER,(65535,0,0,0)):s.mem.put(a,v)
        counters=[60000-i*100 for i in range(12)]
        s.inputs[0x42]=[v for c in counters for v in (c&255,c>>8)]
    s.inputs[0x201]=seq
    if name.startswith('get_joy_input1_'):s.r.update(eax=rng.choice([1,2,4,8,16]),ecx=rng.choice([1,3,4,8,15,32,64,320]))
