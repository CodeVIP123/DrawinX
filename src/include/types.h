#define false 0
#define true !(false)

typedef unsigned char uint_8; // 1 CHAR = 8 bits
typedef signed char int_8;

typedef unsigned short uint_16; // 1 SHORT = 16 bits
typedef signed short int_16;

typedef unsigned int uint_32; // 1 INT = 32 bits
typedef signed int int_32;

typedef unsigned long int addr_t;

typedef unsigned long long uint_64; // 1 LONG LONG = 64 bits
typedef signed long long int_64;

typedef char* address_type; // address_type VGA = (address_type) 0xB8000

typedef const char* string;

typedef _Bool bool;

uint_8 ipd(uint_16 port)
{
    uint_8 result = 0;
    //     ( "string : in %dx, %al"  : "=a" (result) output "will be stored in >
    __asm__("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}

void outb(uint_16 port, uint_8 data)
{
    __asm__ volatile("out %0, %1" : : "a" (data), "Nd" (port));
}

typedef struct InterruptRegisters
{
    uint_32 cr2;
    uint_32 ds;
    uint_32 edi;
    uint_32 esi;
    uint_32 ebp;
    uint_32 esp;
    uint_32 ebx;
    uint_32 edx;
    uint_32 ecx;
    uint_32 eax;
    uint_32 interrupt_no, err_code;
    uint_32 eip;
    uint_32 cs;
    uint_32 eflags;
    uint_32 user_esp;
    uint_32 ss;
} __attribute__((packed)) InterruptRegisters;

void memset(void* dest, char src, uint_32 count)
{
    char* temp = (char*) dest;

    for (; count != 0; count--)
        *temp++ = src;
}

