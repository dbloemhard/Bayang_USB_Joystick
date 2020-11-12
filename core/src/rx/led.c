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
#include "led.h"
#include "defines.h"

#define LEDALL 15

void led1on( uint8_t val )
{
	if ( val&1)	GPIO_SetBits( LED1PORT, LED1PIN);
}

void led1off( uint8_t val )
{
	if ( val&1) GPIO_ResetBits( LED1PORT, LED1PIN);
}

void led2on( uint8_t val )
{
	if ( val&1)	GPIO_SetBits( LED2PORT, LED2PIN);
}

void led2off( uint8_t val )
{
	if ( val&1) GPIO_ResetBits( LED2PORT, LED2PIN);
}

void led1flash( uint32_t period , int duty )
{
	if ( gettime() % period > (period*duty)>>4 )
	{
		led1on(LEDALL);
	}
	else
	{
		led1off(LEDALL);
	}
}

void led2flash( uint32_t period , int duty )
{
	if ( gettime() % period > (period*duty)>>4 )
	{
		led2on(LEDALL);
	}
	else
	{
		led2off(LEDALL);
	}
}








