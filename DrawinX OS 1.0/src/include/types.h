#define false 0
#define true !(false)
#define NULL ((void*)0)

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
typedef char* va_list;

typedef unsigned int uintptr_t;

// VA Stuff

#define VA_SIZE(type) ((sizeof(type) + sizeof(int) - 1) & ~(sizeof(int) - 1)) 

#define va_start(ap, last) (ap = (va_list)&last + VA_SIZE(last))
#define va_arg(ap, type) (*(type*)((ap += VA_SIZE(type)) - VA_SIZE(type)))
#define va_end(ap) (ap = (va_list)0)

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

static inline uint_16 inw(uint_16 port) {
    uint_16 result;
    __asm__ __volatile__("inw %1, %0" : "=a"(result) : "Nd"(port));
    return result;
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


void memcpy(void *dest, const void *src, uint_32 size) {
    uint_8 *d = (uint_8 *)dest;
    const uint_8 *s = (const uint_8 *)src;
    for (uint_32 i = 0; i < size; i++) {
        d[i] = s[i];
    }
}

int strncmp(const char *str1, const char *str2, uint_32 n) {
    for (uint_32 i = 0; i < n; i++) {
        if (str1[i] != str2[i]) {
            return (int)(str1[i] - str2[i]);
        }
        if (str1[i] == '\0' || str2[i] == '\0') {
            break;
        }
    }
    return 0;
}

int strlen(const char *str) {
    int len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

void strncpy(char *dest, const char *src, unsigned int n) {
    unsigned int i = 0;

    while (i < n && src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }

    // Agar source chhota pad gaya, baaki jagah NULL bhar do
    while (i < n) {
        dest[i] = '\0';
        i++;
    }
}

char* strchr(const char* str, int c) {
    while (*str) {
        if (*str == (char)c)
            return (char*)str;
        str++;
    }
    return NULL;
}

char* strtok(char* str, const char* delim) {
    static char* next = NULL;

    if (str != NULL)
        next = str;

    if (next == NULL)
        return NULL;

    // 🔒 safety check
    if (delim == NULL || *delim == '\0') {
        char* start = next;
        next = NULL;
        return start;
    }

    // skip starting delimiters
    char* start = next;
    while (*start && strchr(delim, *start))
        start++;

    if (*start == '\0') {
        next = NULL;
        return NULL;
    }

    // find token end
    char* end = start;
    while (*end && !strchr(delim, *end))
        end++;

    if (*end) {
        *end = '\0';
        next = end + 1;
    } else {
        next = NULL;
    }

    return start;
}