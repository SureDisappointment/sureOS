#include "exception.h"
#include "plugbox.h"
#include "panic.h"
#include <stdbool.h>
#include <stddef.h>

/// Exception: interrupt generated by the CPU when an 'error' occurs.
/// Interrupt vectors 0..31 are reserved for exceptions (some are reserved or legacy (outdated) and not used)
/// Classified as:
/// - Faults: can be corrected and the program may continue as if nothing happened
/// - Traps: reported immediately after the execution of the trapping instruction
/// - Aborts: severe unrecoverable error
/// ---------------------------------------------------
/// Some exceptions push a 32-bit error code on the stack, which provides additional information about the error.
/// This value must be pulled from the stack before returning control back to the currently running program.
/// https://wiki.osdev.org/Exceptions
/// Many exceptions set a `segment selector index` error code, for information see https://wiki.osdev.org/Exceptions#Selector_Error_Code


// 0 - Division Error: Fault, no error code
// -----------------------------------------
// Divide by 0 or division result too large
// The saved instruction pointer points to the instruction which caused the exception
bool de_prologue()
{
    panic("Division Error");
    return false;
}

// 1 - Debug: Fault/Trap, no error code
// -------------------------------------
// Instruction fetch breakpoint (Fault)
// General detect condition (Fault)
// Data read or write breakpoint (Trap)
// I/O read or write breakpoint (Trap)
// Single-step (Trap)
// Task-switch (Trap)
// -------------------------------------
// When the exception is a fault, the saved instruction pointer points to the instruction which caused the exception. When the exception is a trap, the saved instruction pointer points to the instruction after the instruction which caused the exception.
// Exception information is provided in the debug registers
bool db_prologue()
{
    panic("Debug");
    return false;
}

// 2 - Non-maskable Interrupt: Interrupt, no error code
// -----------------------------------------------------
// Hardware failure / watchdog timer
// https://wiki.osdev.org/Non_Maskable_Interrupt
bool nmi_prologue()
{
    panic("Non-maskable Interrupt");
    return false;
}

// 3 - Breakpoint: Trap, no error code
// ------------------------------------
// INT3 instruction
// The saved instruction pointer points to the byte after the INT3 instruction
bool bp_prologue()
{
    panic("Breakpoint");
    return false;
}

// 4 - Overflow: Trap, no error code
// ----------------------------------
// INTO instruction executed while overflow bit in RFLAGS is set to 1
// The saved instruction pointer points to the instruction after the INTO instruction
bool of_prologue()
{
    panic("Overflow");
    return false;
}

// 5 - Bound Range Exceeded: Fault, no error code
// -----------------------------------------------
// BOUND instruction executed and index out of bounds
// The saved instruction pointer points to the BOUND instruction which caused the exception
bool br_prologue()
{
    panic("Bound Range extended");
    return false;
}

// 6 - Invalid Opcode: Fault, no error code
// -----------------------------------------
// Invalid or undefined opcode, instruction with invalid prefixes, instruction length exceeds 15 bytes, non-existent control register, UD instruction
// The saved instruction pointer points to the instruction which caused the exception
bool ud_prologue()
{
    panic("Invalid Opcode");
    return false;
}

// 7 - Device Not Available: Fault, no error code
// -----------------------------------------------
// FPU instruction attempted when there is no FPU or the FPU is disabled
// The saved instruction pointer points to the instruction that caused the exception
bool nm_prologue()
{
    panic("Device Not Available");
    return false;
}

// 8 - Double Fault: Abort, error code: 0
// ---------------------------------------
// Exception not handled or exception occurred while trying to call an exception handler
// The saved instruction pointer is undefined. A double fault cannot be recovered. The faulting process must be terminated.
// See also: https://wiki.osdev.org/Triple_Fault
bool df_prologue()
{
    panic("Double Fault");
    return false;
}

// 10 - Invalid TSS: Fault, error code: segment selector index
// ------------------------------------------------------------
// Invalid segment selector referenced as part of a task switch, or as a result of a control transfer through a gate descriptor, which results in an invalid stack-segment reference using an SS selector in the TSS.
// When the exception occurred before loading the segment selectors from the TSS, the saved instruction pointer points to the instruction which caused the exception. Otherwise (more common), it points to the first instruction in the new task.
bool ts_prologue()
{
    panic("Invalid TSS");
    return false;
}

// 11 - Segment Not Present: Fault, error code: segment selector index
// --------------------------------------------------------------------
// Trying to load a segment or gate which has its present bit set to 0 (however some will instead cause exception 12)
// The saved instruction pointer points to the instruction which caused the exception
bool np_prologue()
{
    panic("Segment Not Present");
    return false;
}

// 12 - Stack-Segment Fault: Fault, error code: segment selector index 
// --------------------------------------------------------------------
// Loading a stack-segment referencing a segment descriptor which is not present
// Stack-limit check fails
// Any PUSH or POP instruction or any instruction using ESP or EBP as a base register is executed, while the stack address is not in canonical form (error code 0)
bool ss_prologue()
{
    panic("Stack-Segment Fault");
    return false;
}

// 13 - General Protection Fault: Fault, error code: segment selector index
// -------------------------------------------------------------------------
// Various reasons (most common: https://wiki.osdev.org/Exceptions#General_Protection_Fault)
// The saved instruction pointer points to the instruction which caused the exception
// If the exception is not segment related, the error code is 0
bool gp_prologue()
{
    panic("General Protection Fault");
    return false;
}

// 14 - Page Fault: Fault, error code: https://wiki.osdev.org/Exceptions#Page_Fault
// ---------------------------------------------------------------------------------
// Page directory or table entry is not present in physical memory
// Attempting to load the instruction TLB with a translation for a non-executable page
// Protection check (privileges, read/write) failed
// Reserved bit in the page directory or table entries is set to 1
// The saved instruction pointer points to the instruction which caused the exception
bool pf_prologue()
{
    panic("Page Fault");
    return false;
}

// 16 - x87 Floating-Point Exception: Fault, no error code
// --------------------------------------------------------
// Any waiting floating-point instruction is executed and CR0.NE is 1 and an unmasked x87 floating point exception is pending
// The saved instruction pointer points to the instruction which is about to be executed when the exception occurred. The x87 instruction pointer register contains the address of the last instruction which caused the exception.
// Exception information is available in the x87 status word register
bool mf_prologue()
{
    panic("x87 Floating-Point Exception");
    return false;
}

// 17 - Alignment Check: Fault, error code: ?
// ------------------------------------------
// Alignment checking is enabled (CPL 3) and an unaligned memory data reference is performed
// The saved instruction pointer points to the instruction which caused the exception
bool ac_prologue()
{
    panic("Alignment Check");
    return false;
}

// 18 - Machine Check: Abort, no error code
// -----------------------------------------
// Model specific, processor implementations are not required to support it
// Not enabled by default
// The value of the saved instruction pointer depends on the implementation and the exception
bool mc_prologue()
{
    panic("Machine Check");
    return false;
}

// 19 - SIMD Floating-Point Exception: Fault, no error code
// ---------------------------------------------------------
// Unmasked 128-bit media floating-point exception occurs and the CR4.OSXMMEXCPT bit is set to 1 (will cause exception 6 otherwise)
// The saved instruction pointer points to the instruction which caused the exception
// Exception information is available in the MXCSR register
bool xm_prologue()
{
    panic("SIMD Floating-Point Exception");
    return false;
}

// 20 - Virtualization Exception: Fault, no error code
// ----------------------------------------------------
// TODO
bool ve_prologue()
{
    panic("Virtualization Exception");
    return false;
}

// 21 - Control Protection Exception: Fault, error code
// -----------------------------------------------------
// TODO
bool cp_prologue()
{
    panic("Control Protection Exception");
    return false;
}

// 28 - Hypervisor Injection Exception: Fault, no error code
// ----------------------------------------------------------
// TODO
bool hv_prologue()
{
    panic("Hypervisor Injection Exception");
    return false;
}

// 29 - VMM Communication Exception: Fault, error code
// ----------------------------------------------------
// TODO
bool vc_prologue()
{
    panic("VMM Communication Exception");
    return false;
}

// 30 - Security Exception: Fault, error code
// -------------------------------------------
// TODO
bool sx_prologue()
{
    panic("Security Exception");
    return false;
}


void exception_defaults()
{
    plugbox_assign(int_de, new_interrupt_handler(de_prologue, NULL));
    plugbox_assign(int_db, new_interrupt_handler(db_prologue, NULL));
    plugbox_assign(int_nmi, new_interrupt_handler(nmi_prologue, NULL));
    plugbox_assign(int_bp, new_interrupt_handler(bp_prologue, NULL));
    plugbox_assign(int_of, new_interrupt_handler(of_prologue, NULL));
    plugbox_assign(int_br, new_interrupt_handler(br_prologue, NULL));
    plugbox_assign(int_ud, new_interrupt_handler(ud_prologue, NULL));
    plugbox_assign(int_nm, new_interrupt_handler(nm_prologue, NULL));
    plugbox_assign(int_df, new_interrupt_handler(df_prologue, NULL));
    plugbox_assign(int_ts, new_interrupt_handler(ts_prologue, NULL));
    plugbox_assign(int_np, new_interrupt_handler(np_prologue, NULL));
    plugbox_assign(int_ss, new_interrupt_handler(ss_prologue, NULL));
    plugbox_assign(int_gp, new_interrupt_handler(gp_prologue, NULL));
    plugbox_assign(int_pf, new_interrupt_handler(pf_prologue, NULL));
    plugbox_assign(int_mf, new_interrupt_handler(mf_prologue, NULL));
    plugbox_assign(int_ac, new_interrupt_handler(ac_prologue, NULL));
    plugbox_assign(int_mc, new_interrupt_handler(mc_prologue, NULL));
    plugbox_assign(int_xm, new_interrupt_handler(xm_prologue, NULL));
    plugbox_assign(int_ve, new_interrupt_handler(ve_prologue, NULL));
    plugbox_assign(int_cp, new_interrupt_handler(cp_prologue, NULL));
    plugbox_assign(int_hv, new_interrupt_handler(hv_prologue, NULL));
    plugbox_assign(int_vc, new_interrupt_handler(vc_prologue, NULL));
    plugbox_assign(int_sx, new_interrupt_handler(sx_prologue, NULL));
}