global int_enable
global int_disable
global cpu_idle
global cpu_halt

section .text
int_enable:
	sti
	ret

int_disable:
	cli
	ret

cpu_idle:
	sti ; STI und HLT werden atomar ausgefuehrt
	hlt
        ret

cpu_halt:
	cli
	hlt