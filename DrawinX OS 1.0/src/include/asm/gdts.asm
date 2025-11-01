global                      initGdtAsm                              ; Declare this function globally

initGdtAsm:

mov eax,                [esp + 4]                                   ; Get the addr_t attribute

lgdt                    [eax]                                       ; Load the GDT into memory

mov eax,                0x10                                        ; Point EAX to GDT

mov ds,                 ax                                          ; Load the Data Segment (DS)

mov es,                 ax                                          ; Load the ES

mov fs,                 ax                                          ; Load the FS

mov gs,                 ax                                          ; Load GS

mov ss,                 ax                                          ; Load State Segment (SS)

jmp                     0x08:.flush                                 ; Load the Code Segment using the offset

.flush:

    ret                                                             ; Return from the function

global tssFlush

tssFlush:

mov ax,             0x2B                                            ; The descriptor of the TSS in the GDT

ltr                 ax                                              ; Load the TSS from the descriptor

ret                                                                 ; Return from the function