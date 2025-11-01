section                 .text

global                  idtFlush

idtFlush:

mov eax,                [esp+4]                         ; Load the pointer to the IDT in the memory

lidt                    [eax]                           ; Load the IDT

ret                                                     ; Return from the funtion



