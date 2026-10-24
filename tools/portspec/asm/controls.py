"""Negative controls for each comparison dimension; no oracle mutation."""
import json,copy
from prove import *
for module in MODULES:__import__(module)

def main():
    oracle=Oracle();byname={f['name']:f for f in json.loads((HERE/'inventory.json').read_text())};out=[]
    def check(label,name,mutate,want):
        initial,flags=prepare(oracle,name,0xa550000);expected=copy.deepcopy(initial);MODELS[name](expected);actual=oracle.run(byname[name]['offset'],initial,flags)
        assert not compare(expected,actual),(label,'positive control failed')
        mutate(expected,actual)
        errors=compare(expected,actual);kinds={e['kind'] for e in errors}
        assert want in kinds,(label,'mutant not detected',errors)
        out.append(dict(control=label,entry=name,status='EXPECTED_MISMATCH',detected=sorted(kinds)))
    check('wrong_pixel','N_CLRSCR',lambda s,a:s.mem.put(s.r['edi']-64000,1,1),'memory')
    check('extra_same_value_write','N_CLRSCR',lambda s,a:s.mem.put(AUX,s.mem.get(AUX,1),1),'written_byte_set')
    check('omitted_write','N_CLRSCR',lambda s,a:s.mem.writes.remove(s.r['edi']-64000),'written_byte_set')
    check('wrong_output_register','copy_dwords',lambda s,a:s.r.__setitem__('esi',s.r['esi']+1),'register')
    check('clobbered_preserved_register','copy_dwords',lambda s,a:s.r.__setitem__('ebp',s.r['ebp']^1),'register')
    check('dead_stack_corruption','N_CLRSCR',lambda s,a:s.mem.put(SP-8,s.mem.get(SP-8,1)^1,1),'memory')
    check('wrong_port_word','asm_0283c',lambda s,a:s.io[0].__setitem__(3,s.io[0][3]^0x100),'io_sequence')
    def plane(s,a):
        key=next(iter(s.mem.vga_writes));s.mem.vga_writes[key]^=1
    check('wrong_VGA_plane_pixel','mode_x_effect_2b3d',plane,'VGA_plane_bytes')
    check('wrong_self_modifying_immediate','effect_reset_geometry',lambda s,a:s.mem.put(BASE[1]+0x815,0),'memory')
    check('wrong_clip_carry','loc_052DA',lambda s,a:setattr(s,'cf',1-s.cf),'carry')
    check('unrestored_ES','bios_wait_timer',lambda s,a:a.__setitem__('es',17),'preserved_segment')
    check('wrong_final_interrupt_flag','get_joy_input2_windows1',lambda s,a:setattr(s,'flags',s.flags^0x200),'flags')
    starts=0;steps=0
    for row in range(224):
        for column in range(80):
            seq=[(row*80+column+4*c)//80!=row for c in range(20)]
            assert all(not seq[i] or seq[i+1] for i in range(19))
            starts+=1;steps+=20
    result=dict(status='PASS',negative_controls=out,negative_control_count=len(out),seam_reversal={'status':'PROVEN bounded arithmetic','cache_start_positions':starts,'column_steps':steps,'conclusion':'row-crossing predicate never transitions true->false; asm_02850 block1:02920 is unreachable for a valid forward20-tile row'},script_sha256s=bundle_hashes())
    (HERE/'controls.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2))
if __name__=='__main__':main()
