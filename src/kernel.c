#include "DrX.h"

uint_8* mybuffer;

void kernel_main()
{
    showSplashScreen();
    for (int i = 0; i < 100000000; i++);
    clearScreen();
    loadGdt();
    initIDT();
    setIDTGatesSystemReserved();
    initTimer();
    init_paging();
    fs_init();                    // Storage Ready
    fs_create("test");    // File "test" banao
    int f = fs_open("test");              // File khol lo
    fs_write(f, (uint_8*) "Hello!", strlen("Hello!"));    // Data likho
    fs_read(f, mybuffer, directory[f].size);   // Data padho
    setRowColumnForDebugPrintf(0, 0);
    debugPrintf("%s", mybuffer);
    fs_close(f);                  // File band karo

    setIDTKeyboardGate();
    //for (int i = 0; i < 100000000; i++);
    //clearScreen();
    //cprintf((string) "Welcome to DrawinX OS!", 0x12);
    //printf((string) "DrawinX>", 2, 0);
    while (1);
}