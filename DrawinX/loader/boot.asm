BITS                32                                                 ; SPECS: 32-bit assembly

section             .multiboot                                         ; The multiboot header

align               4                                                  ; Aligning the header
dd                  0x1BADB002                                         ; SPECS: Magic number
dd                  0x00000000                                         ; SPECS: Flags
dd                  - (0x1BADB002 + 0x00000000)                        ; SPECS: Checksum

extern              kernel_main                                        ; The main function in the kernel
global              _start                                             ; The start function of ASM

section             .text                                              ; The main text header
_start:

call                kernel_main                                        ; Call the kernel

_halt:

hlt                                                                    ; Halt (freeze) the CPU
jmp _halt



