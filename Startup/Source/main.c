#include <bsp.h>
#include <cmsis_os2.h>

int main(void)
{
    BSP_Init();
    osKernelInitialize();
    osKernelStart();
    while (true); // Should Never Get Here.
}