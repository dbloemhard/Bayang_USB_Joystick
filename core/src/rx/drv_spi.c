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
#include "drv_spi.h"
#include "binary.h"
#include "defines.h"

void spi_init(void)
{
	// spi port inits

	GPIO_InitTypeDef  GPIO_InitStructure;

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  //GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_InitStructure.GPIO_Pin = SPI_MOSI_PIN;
	GPIO_Init(SPI_MOSI_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = SPI_CLK_PIN;
	GPIO_Init(SPI_CLK_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = SPI_SS_PIN;
	GPIO_Init(SPI_SS_PORT, &GPIO_InitStructure);

	// MISO should be input by default
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Pin = SPI_MISO_PIN;
	GPIO_Init(SPI_MISO_PORT, &GPIO_InitStructure);

	spi_ceon();
	spi_csoff();

}


#define gpioset( port , pin) port->BSRR = pin
#define gpioreset( port , pin) port->BRR = pin

#define MOSIHIGH gpioset( SPI_MOSI_PORT, SPI_MOSI_PIN)
#define MOSILOW gpioreset( SPI_MOSI_PORT, SPI_MOSI_PIN);
#define SCKHIGH gpioset( SPI_CLK_PORT, SPI_CLK_PIN);
#define SCKLOW gpioreset( SPI_CLK_PORT, SPI_CLK_PIN);
#define CEHIGH gpioset( SPI_CE_PORT, SPI_CE_PIN);
#define CELOW gpioreset( SPI_CE_PORT, SPI_CE_PIN);

//#define READMISO (GPIO_ReadInputDataBit(SPI_MISO_PORT, SPI_MISO_PIN) )
#define READMISO (SPI_MISO_PORT->IDR & SPI_MISO_PIN)

#pragma push

#pragma Otime
#pragma O2

void spi_ceon( )
{
	SPI_CE_PORT->BRR = SPI_CE_PIN;
}

void spi_ceoff( )
{
	SPI_CE_PORT->BSRR = SPI_CE_PIN;
}

void spi_cson( )
{
	SPI_SS_PORT->BRR = SPI_SS_PIN;
}

void spi_csoff( )
{
	SPI_SS_PORT->BSRR = SPI_SS_PIN;
}


void spi_sendbyte ( int data)
{
for ( int i =7 ; i >=0 ; i--)
	{
		if (  (data>>i)&1  )
		{
			MOSIHIGH;
		}
		else
		{
			MOSILOW;
		}

		SCKHIGH;
		SCKLOW;
	}
}


int spi_sendrecvbyte2( int data)
{
	int recv = 0;
	for ( int i =7 ; i >=0 ; i--)
	{
		if ( (data) & (1<<7)  )
		{
			MOSIHIGH;
		}
		else
		{
			MOSILOW;
		}
		SCKHIGH;
		data = data<<1;
		if ( READMISO ) recv= recv|(1<<7);
		recv = recv<<1;
		SCKLOW;
	}
	  recv = recv>>8;
    return recv;
}


 int spi_sendrecvbyte( int data)
{ int recv = 0;

	for ( int i = 7 ; i >=0 ; i--)
	{
		recv = recv<<1;
		if ( (data) & (1<<7)  )
		{
			MOSIHIGH;
		}
		else
		{
			MOSILOW;
		}

		data = data<<1;

		SCKHIGH;

		if ( READMISO ) recv= recv|1;

		SCKLOW;

	}

    return recv;
}


 int spi_sendzerorecvbyte( )
{ int recv = 0;
	MOSILOW;

	for ( int i = 7 ; i >=0 ; i--)
	{
		recv = recv<<1;

		SCKHIGH;

		if ( READMISO ) recv= recv|1;

		SCKLOW;

	}
    return recv;
}


#pragma pop









