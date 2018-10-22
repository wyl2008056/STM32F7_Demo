#include "stm32f7xx.h"


//system clock  200000000
//stimulus port 0

/* Redirect the printf to the ITM_PUTCHAR */
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */

#ifdef __GNUC__
#define ITM_PUTCHAR int __io_putchar(int ch)
#else
#define ITM_PUTCHAR int fputc(int ch, FILE *f)
#endif /* __GNUC__ */


ITM_PUTCHAR
{
	ITM_SendChar(ch);
	return ch;
}