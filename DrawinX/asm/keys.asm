section             .data                               ; Variables section
global              keycode                             ; Make the 'keycode' usable for C
keycode:            db 0                                ; Store memory for keycode variable

section             .text
global              keycodePolls                        ; Make this function usable in other files
keycodePolls:

.waitForKey:                                            ; The segment of the function that read the keys
in                  al, 0x64                            ; Read the status of the keyboard
test                al, 0x01                            ; Check if the output buffer is full
jz                  .waitForKey                         ; If not, wait for the key
in                  al, 60                              ; Read the data from the port
mov                 [keycode], al                       ; Move the keycode into output variable (AL)
ret                                                     ; Return from the function