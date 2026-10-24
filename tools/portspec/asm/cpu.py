from models import *

@model('DetectCpu','Probe CPU features by attempting EFLAGS.AC then EFLAGS.ID changes. If AC cannot be set return3; if AC succeeds but ID cannot toggle return4; otherwise execute CPUID leaf1 and return base family bits8..11 of EAX (no extended-family combination). CPUID at1:06167 is abstracted; supported AC/ID writes at1:06134/0614d are explicit processor-capability stubs. Preserve EDX/ESI/EDI/EBP,restore low16 EBX only; high16 EBX is zero on legacy paths or CPUID EBX high16 on modern path. ECX becomes CPUID ECX only on modern path. Restore EFLAGS and SP alignment.','32-bit flat caller stack,initial EFLAGS=2 in certificate; three CPU capability profiles (386/486/CPUID) and randomized CPUID leaf1 outputs. Original function temporarily aligns low16 SP to4.',globals_read=[]) 
def detect(s):
    r=s.r;original_bx=r['ebx'];s.push(original_bx,2);s.push(2,2)
    # The test stack begins4-byte aligned, so AND SP,FFFC yields FFFC,
    # with SF/PF set; these are ABI-visible dead stack contents.
    s.push(0x86);s.push(0x86);s.pop();s.push(0x40086);s.pop()
    ac,ident=s.inputs['cpu_capabilities'];s.io.append(['flag_probe','AC',ac]);s.push(0x40086 if ac else 0x86);s.pop();s.pop()
    if not ac:r['eax']=3;r['ebx']=original_bx&65535
    else:
        s.push(6);s.pop();s.push(0x200006);s.pop();s.io.append(['flag_probe','ID',ident]);s.push(0x200006 if ident else 6);s.pop();s.push(6);s.pop()
        if not ident:r['eax']=4;r['ebx']=original_bx&65535
        else:
            s.push(r['edx']);s.io.append(['cpuid',1,r['ecx']]);answer=s.inputs['cpu_response'];r['eax']=(answer['eax']>>8)&15;r['ebx']=(answer['ebx']&0xffff0000)|(original_bx&65535);r['ecx']=answer['ecx'];r['edx']=s.pop()
    s.pop(2);s.pop(2);s.flags=2;s.flagmask=0xffffffff;ret(s)

def prepare(s,name,rng,seed):
    if name!='DetectCpu':return
    mode=seed%3;s.inputs['cpu_capabilities']=(mode!=0,mode==2)
    s.inputs['cpu_response']={n:rng.getrandbits(32) for n in ('eax','ebx','ecx','edx')}
