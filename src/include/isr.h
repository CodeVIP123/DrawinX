#include "idt.h"

void interrupt_handler(InterruptRegisters* regs)
{
    switch (regs->interrupt_no)
    {
        case 0:
            printf("***ERROR*** Interrupt 0: Division by Zero ***ERROR***", 0, 0);
            while(1);
            break;
        case 14:
            printf("***ERROR*** Interrupt 14: Page Fault ***ERROR***", 0, 0);
            while(1);
            break;
        default:
            uint_16 cur_p = printf("Unknown interrupt:", 0, 0);
            setRowColumnForDebugPrintf(0, 20);
            debugPrintf((string) "%x", regs->interrupt_no);
            while(1);
            break;
    }

    outb(0x20, 0x20); // Send EOI to PIC
}

__attribute__((naked)) void isr_common_stub()
{
    asm volatile(
        "cli\n\t"                 // Disable interrupts
        "pusha\n\t"               // Save general-purpose registers
        "push %ds\n\t"            // Save segment registers
        "push %es\n\t"
        "push %fs\n\t"
        "push %gs\n\t"
        "mov $0x10, %ax\n\t"      // Load kernel data segment
        "mov %ax, %ds\n\t"
        "mov %ax, %es\n\t"
        "mov %ax, %fs\n\t"
        "mov %ax, %gs\n\t"
        "mov %esp, %eax\n\t"      // Get the current stack pointer
        "push %eax\n\t"           // Push pointer to InterruptRegisters onto the stack
        "call interrupt_handler\n\t" // Call the C interrupt handler
        "add $4, %esp\n\t"        // Clean up argument (remove pushed pointer)
        "pop %gs\n\t"             // Restore segment registers
        "pop %fs\n\t"
        "pop %es\n\t"
        "pop %ds\n\t"
        "popa\n\t"                // Restore general-purpose registers
        "sti\n\t"                 // Re-enable interrupts
        "iret\n\t"                // Return from interrupt
    );
}

__attribute__((naked)) void isr_timer()
{
    asm volatile(
        "cli\n\t"                 // Disable interrupts
        "pusha\n\t"               // Save general-purpose registers
        "push %ds\n\t"            // Save segment registers
        "push %es\n\t"
        "push %fs\n\t"
        "push %gs\n\t"
        "mov $0x10, %ax\n\t"      // Load kernel data segment
        "mov %ax, %ds\n\t"
        "mov %ax, %es\n\t"
        "mov %ax, %fs\n\t"
        "mov %ax, %gs\n\t"
        "mov %esp, %eax\n\t"      // Get the current stack pointer
        "push %eax\n\t"           // Push pointer to InterruptRegisters onto the stack
        "call onTickTimer\n\t" // Call the C interrupt handler
        "add $4, %esp\n\t"        // Clean up argument (remove pushed pointer)
        "pop %gs\n\t"             // Restore segment registers
        "pop %fs\n\t"
        "pop %es\n\t"
        "pop %ds\n\t"
        "popa\n\t"                // Restore general-purpose registers
        "sti\n\t"                 // Re-enable interrupts
        "iret\n\t"                // Return from interrupt
    );
}

__attribute__((naked)) void isr_keyboard()
{
    asm volatile(
        "cli\n\t"                 // Disable interrupts
        "pusha\n\t"               // Save general-purpose registers
        "push %ds\n\t"            // Save segment registers
        "push %es\n\t"
        "push %fs\n\t"
        "push %gs\n\t"
        "mov $0x10, %ax\n\t"      // Load kernel data segment
        "mov %ax, %ds\n\t"
        "mov %ax, %es\n\t"
        "mov %ax, %fs\n\t"
        "mov %ax, %gs\n\t"
        "mov %esp, %eax\n\t"      // Get the current stack pointer
        "push %eax\n\t"           // Push pointer to InterruptRegisters onto the stack
        "call keyboardHandler\n\t" // Call the C interrupt handler
        "add $4, %esp\n\t"        // Clean up argument (remove pushed pointer)
        "pop %gs\n\t"             // Restore segment registers
        "pop %fs\n\t"
        "pop %es\n\t"
        "pop %ds\n\t"
        "popa\n\t"                // Restore general-purpose registers
        "sti\n\t"                 // Re-enable interrupts
        "iret\n\t"                // Return from interrupt
    );
}


void setIDTGatesSystemReserved(){
    for (int i = 0; i <= 31; i++)
    {
        setIDTGate(i, (uint_32) isr_common_stub, 0x08, 0x8E);
    } 
}

void initTimer()
{
    ticks = 0;
    setIDTGate(32, (uint_32) isr_timer, 0x08, 0x8E);
    uint_32 divisor = 1193180/freq;
    outb(0x43, 0x36);
    outb(0x40, (uint_8) (divisor & 0xFF));
    outb(0x40, (uint_8) ((divisor >> 8) & 0xFF));
}

void setIDTKeyboardGate()
{
    setIDTGate(33, (uint_32) isr_keyboard, 0x08, 0x8E);
}



