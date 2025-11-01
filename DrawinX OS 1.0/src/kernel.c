#include "DrX.h"

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
    for (int i = 0; i < 100000000; i++);
    clearScreen();
    greeting();
    printf((string) "DrawinX>", 2, 0);
    setIDTKeyboardGate();
    while (1);
}
