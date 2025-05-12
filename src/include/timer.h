#include "splash_screen.h"

uint_64 ticks;
const uint_32 freq = 100;

void onTickTimer(InterruptRegisters* regs)
{
    ticks += 1;
    outb(PIC1_COMMAND, PIC_EOI);
}