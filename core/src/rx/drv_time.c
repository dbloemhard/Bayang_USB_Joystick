/*
The MIT License (MIT)

Copyright (c) 2016 silverx

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include "stm32f10x.h"                  // Device header
#include "drv_time.h"
#include "defines.h"

void failloop( int val);

unsigned long lastticks;
unsigned long globalticks;
volatile unsigned long systickcount = 0;

#define SYS_CLOCK_FREQ_HZ 72000000

 // divider by 8 is enabled in this systick config                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  
static __INLINE uint32_t SysTick_Config2(uint32_t ticks)
{ 
  if (ticks > SysTick_LOAD_RELOAD_Msk)  return (1);            /* Reload value impossible */
                                                               
  SysTick->LOAD  = (ticks & SysTick_LOAD_RELOAD_Msk) - 1;      /* set reload register */
  NVIC_SetPriority (SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1);  /* set Priority for Cortex-M0 System Interrupts */
  SysTick->VAL   = 0;                                          /* Load the SysTick Counter Value */
  SysTick->CTRL  = //SysTick_CTRL_CLKSOURCE_Msk |   // divide by 8
                   SysTick_CTRL_TICKINT_Msk   | 
                   SysTick_CTRL_ENABLE_Msk;                    /* Enable SysTick IRQ and SysTick Timer */
  return (0);                                                  /* Function successful */
}


void time_init()
{

	  if (SysTick_Config2( SYS_CLOCK_FREQ_HZ / 9 ))
    {// not able to set divider
			  failloop(5);
        while (1);
    }
}

// called at least once per 16ms or time will overflow
unsigned long time_update(void)
{
	unsigned long maxticks = SysTick->LOAD;	
	unsigned long ticks = SysTick->VAL;	
	unsigned long quotient;	
	unsigned long elapsedticks;	
	static unsigned long remainder = 0;// carry forward the remainder ticks;

	if (ticks < lastticks) 
	{
		elapsedticks = lastticks - ticks;	
	}
	else
	{
		// overflow ( underflow really)
		elapsedticks = lastticks + ( maxticks - ticks);			
	}

	lastticks = ticks;
	elapsedticks += remainder;

	quotient = elapsedticks / 8;
	remainder = elapsedticks - quotient*8;

	globalticks = globalticks + quotient; 

	return globalticks;	
}

// return time in uS from start ( micros())
unsigned long gettime()
{
	return time_update();
}

// delay in uS
void delay(uint32_t data)
{
	volatile uint32_t count;
	count = data * 7;
	while (count--);
}


void SysTick_Handler(void)
{

}
