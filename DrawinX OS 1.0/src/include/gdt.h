#include "paging.h"

extern void initGdtAsm(addr_t);
extern void tssFlush();

struct GDTEntry
{
    uint_16 limit;
    uint_16 base_low;
    uint_8 base_middle;
    uint_8 access;
    uint_8 flags;
    uint_8 base_hi;
} __attribute__((packed));

struct GDTPointer
{
    uint_16 limit;
    uint_32 base;
} __attribute__((packed));

struct TaskStateSegmentEntry
{
    uint_32 esp0;
    uint_32 ss0;
    uint_32 esp1;
    uint_32 ss1;
    uint_32 esp2;
    uint_32 ss2;
    uint_32 cr3;
    uint_32 eip;
    uint_32 eflags;
    uint_32 eax;
    uint_32 ecx;
    uint_32 edx;
    uint_32 ebx;
    uint_32 esp;
    uint_32 ebp;
    uint_32 esi;
    uint_32 edi;
    uint_32 es, cs, ss, ds, fs, gs;
    uint_32 ssp;
} __attribute__((packed));

struct GDTEntry gdt[6];
struct GDTPointer gdt_ptr;
struct TaskStateSegmentEntry tss;

void setGdtGate(int n, uint_32 base, uint_32 limit, uint_8 access, uint_8 flags)
{
    gdt[n].base_low = (base & 0xFFFF);
    gdt[n].base_middle = (base >> 16) & 0xFF;
    gdt[n].base_hi = (base >> 24) & 0xFF;

    gdt[n].limit = (limit & 0xFFFF);
    gdt[n].flags = (limit >> 16) & 0x0F;
    gdt[n].flags |= (flags & 0xF0);

    gdt[n].access = access;
}

void writeTSS(uint_32 num, uint_16 ss0, uint_32 esp0)
{
    uint_32 base = (uint_32) &tss;
    uint_32 limit = base + sizeof(tss);

    setGdtGate(num, base, limit, 0xE9, 0x00);
    memset(&tss, 0, sizeof(tss));
    tss.ss0 = ss0;
    tss.esp0 = esp0;
    tss.cs = 0x08 | 0x3;
    tss.ss = tss.ds = tss.es = tss.fs = tss.gs = 0x10 | 0x3;
}

void loadGdt()
{
    gdt_ptr.limit = (sizeof(struct GDTEntry) * 6) - 1;
    gdt_ptr.base = (uint_32) &gdt;

    setGdtGate(0, 0, 0, 0, 0); // Null Segment Selector               
    setGdtGate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Kernel Code Segment -> 0x9A -> 1001 1010
    setGdtGate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Kernel Data Segment -> 0x92 -> 1001 0010
    setGdtGate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User Code Segment -> 0xFA -> 1111 1010
    setGdtGate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User Data Segment -> 0xF2 -> 1111 0010
    writeTSS(5, 0x10, 0x0);
    initGdtAsm((addr_t) &gdt_ptr);
    tssFlush();
}

