#include "timer.h"

extern void idtFlush(addr_t);

struct IDTEntry
{
    uint_16 base_low;
    uint_16 selector;
    uint_8 always0;
    uint_8 flags;
    uint_16 base_high;
} __attribute__((packed));

struct IDTPointer
{
    uint_16 limit;
    uint_32 base;
} __attribute__((packed));

struct IDTEntry idt[256];
struct IDTPointer idt_ptr;

void setIDTGate(uint_32 num, uint_32 base, uint_16 selector, uint_8 flags)
{
    idt[num].base_low = (base & 0xFFFF);
    idt[num].selector = selector;
    idt[num].always0 = 0;
    idt[num].flags = flags;
    idt[num].base_high = (base >> 16) & 0xFFFF;
}

void initIDT()
{
    idt_ptr.limit = (sizeof(struct IDTEntry) * 256) - 1;
    idt_ptr.base = (uint_32) &idt;

    uint_8 mask_mas = ipd(0x21);
    uint_8 mask_slave = ipd(0xA1);

    outb(0x20, 0x11);               // Send initialisation start code to the command port (Master PiC)
    outb(0xA0, 0x11);              // Send initialisation start code to the command port (Slave PiC)

    outb(0x21, 0x20);             // Send the Master PiC it's identity using the data port (ICW2)
    outb(0xA1, 0x28);            // Send the Master PiC it's identity using the data port (ICW3)

    outb(0x21, 0x04);           // Send signal to clarify Slave PiC on IRQ2 (ICW3)
    outb(0xA1, 0x02);          // Send it it's identity that is ICW3 (Slave PiC)

    outb(0x21, 0x01);         // Set it to 8086/88 mode ICW4 (Master PiC)
    outb(0xA1, 0x01);        // Set it to 8086/88 mode ICW4 (Slave PiC)

    outb(0x21, mask_mas);   // Restore saved mask (Mater PiC)
    outb(0xA1, mask_slave); // Restore saved mask (Slave PiC)

    uint_8 mask = ipd(0x21);   // Read the current mask from the master PIC
    mask &= ~0x01;              // Clear bit 0 to unmask IRQ0
    outb(0x21, mask);           // Write the updated mask back to the PIC

    uint_8 masks = ipd(0x21);   // Read current IRQ mask
    masks &= ~(1 << 1);         // Clear bit 1 (unmask IRQ1)
    outb(0x21, masks);          // Unmask IRQ1

    outb(0x21, ipd(0x21) | (1 << 6));  // Mask IRQ 14 (ATA Primary)
    outb(0x21, ipd(0x21) | (1 << 7));  // Mask IRQ 15 (ATA Secondary)

    idtFlush((addr_t) &idt_ptr); 
    asm volatile("sti");  // Enable the interrupts
}