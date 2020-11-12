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

#include "binary.h"
#include "drv_spi.h"
#include "stm32f10x.h"                  // Device header
#include "xn297.h"
#include "defines.h"

// all cases except 3 wires radio
#if !defined (SOFTSPI_3WIRE)

void xn_writereg( int reg , int val)
{
	reg = reg&0x0000003F;
	reg = reg|W_REGISTER;
	spi_cson();
	spi_sendbyte( reg);
	spi_sendbyte( val);
	spi_csoff();
}

int xn_readreg( int reg)
{
	reg = reg&REGISTER_MASK;
	spi_cson();
	spi_sendrecvbyte( reg);
	int val =spi_sendzerorecvbyte();
	spi_csoff();
	return val;
}

int xn_command( int command)
{
	spi_cson();
	int status = spi_sendrecvbyte(command);
	spi_csoff();
	return status;
}
//
void _spi_write_address( int reg, int val);

void _spi_write_address( int reg, int val)
{
	spi_cson();
	spi_sendbyte( reg);
	spi_sendbyte( val);
	spi_csoff();
}


void xn_readpayload( int *data , int size )
{
	int index = 0;
	spi_cson();
	spi_sendrecvbyte( R_RX_PAYLOAD ); // read rx payload
	while( index < size )
	{
	data[index]= spi_sendzerorecvbyte();
	index++;
	}
	spi_csoff();
}



void xn_writerxaddress(  int *addr )	
{
		int index = 0;
    spi_cson();
    spi_sendbyte(W_REGISTER | RX_ADDR_P0);
	while(index<5)
	{
	spi_sendbyte( addr[index] );
	index++;
	}
spi_csoff();
}


void xn_writetxaddress(  int *addr )	
{
		int index = 0;
    spi_cson();
    spi_sendbyte(W_REGISTER|TX_ADDR);
	while(index<5)
	{
	spi_sendbyte( addr[index] );
	index++;
	}
spi_csoff();
}


void xn_writepayload( int data[] , int size )
{
	int index = 0;
	spi_cson();
	spi_sendrecvbyte( W_TX_PAYLOAD ); // write tx payload
	while(index<size)
	{
	spi_sendrecvbyte( data[index] );
	index++;
	}
	spi_csoff();
}

#endif











