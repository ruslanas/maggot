; Interrupt Descriptor Table

use32

idtr:
	dw end_idt - idt
	dd idt

idt:
	gate_entry 0x08,  div_by_zero_irs
	gate_entry 0x08,  exception
	gate_entry 0x08,  exception
	gate_entry 0x08,  breakpoint_irs
	gate_entry 0x08,  exception
	gate_entry 0x08,  exception
	gate_entry 0x08,  invalid_opcode_irs
	gate_entry 0x08,  exception
	gate_entry 0x08,  exception
	gate_entry 0x08,  exception
	gate_entry 0x08,  invalid_tss_irs   ; 10 - invalid_tss_irs
	gate_entry 0x08,  exception	    ; 11 - segment not present
	gate_entry 0x08,  exception
	gate_entry 0x08,  gp_fault_irs	    ; 0xD general protection fault #GP
	repeat 0x80 - 14
	       gate_entry 0x08, exception
	end repeat
	gate_entry 0x08, system_interrupt   ; int 0x80
end_idt:
