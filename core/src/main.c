#include "stm32f10x.h"                  // Device header
#include "misc.h"                       // Keil::Device:StdPeriph Drivers:Framework
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM
#include "stm32f10x_usart.h"            // Keil::Device:StdPeriph Drivers:USART
#include "stm32f10x_spi.h"              // Keil::Device:StdPeriph Drivers:SPI


#include <stdio.h>

#include "main.h"
#include "hw_config.h"
#include "ppm_decoder.h"
#include "ppm_reader.h"
#include "rx/defines.h"
#include "rx/drv_spi.h"
#include "rx/drv_time.h"
#include "rx/rx_bayang.h"
#include "rx/led.h"


// NRF24L01 for Bayang RX /////////////////////////////////////////////////////
// looptime in seconds
//float looptime;

// holds the main four channels, roll, pitch , yaw , throttle
float rx[4];
// holds auxilliary channels
char aux[AUXNUMBER] = { 0, 0, 0, 0, 0, 0, 0, 0};

// bind / normal rx mode
extern int rxmode;
// failsafe on / off
extern int failsafe;

void checkbindbutton(void);
int bindbuttonpressed = 0;
void rxsendusb(void);

// PPM decoder ////////////////////////////////////////////////////////////////

#define PPM_MIN             1000
#define PPM_MAX             3000
#define PPM_BTN_THRESHOLD   1300
uint16_t ppm_buffer[PPM_NUM_CHANNELS] = {0};

// USB ////////////////////////////////////////////////////////////////////////

#define USB_BUFFER_SIZE (4 + 1)

__IO uint8_t PrevXferComplete = 1;
uint16_t usb_send_buffer[USB_BUFFER_SIZE];

// UART ///////////////////////////////////////////////////////////////////////

uint8_t uart_header[] = { 0xAA, 0xBB };

///////////////////////////////////////////////////////////////////////////////

void usb_init(void);
void gpio_init(void);
void usart_init(void);

void process_impulse(uint16_t duration);
void process_channel(uint8_t axis, uint16_t value);
void ppm_send_usb(void);
void ppm_send_uart(void);


int main()
{
	//ppm_decoder_init(process_channel, ppm_send_usb);		
	usb_init();
	gpio_init();
	spi_init();
  time_init();

	rx_init();
	
	//ppm_reader_init(process_impulse);
	usart_init();
	led1on(255);
	led2off(255);
	//GPIOC->ODR &= ~GPIO_Pin_13;
	
	while(1)
	{
		// bind button
		//checkbindbutton();  // Not currently working
		// receiver function
		checkrx();
		// send data to USB
		rxsendusb();
		// led flash logic	
		if ( rxmode == RXMODE_BIND)
		{// bind mode
			led2flash ( 100000, 12);
		}else
		{// non bind
			if ( failsafe) 
				led2flash ( 500000, 15);			
			else 
				led2on(255);
		}
	}
}

void usb_init(void)
{
	Set_System();
  USB_Interrupts_Config();
  Set_USBClock();
  USB_Init();
}


// Initialize user-defined GPIOs (LEDs, Button)
void gpio_init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	//LED 1
	GPIO_InitTypeDef gpio_initled1 = {0};
	gpio_initled1.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio_initled1.GPIO_Speed = GPIO_Speed_50MHz;
	gpio_initled1.GPIO_Pin = LED1PIN;
	GPIO_Init(LED1PORT, &gpio_initled1);
	//gpio_init.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15;
	//GPIO_Init(GPIOC, &gpio_init);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	//LED 2
	GPIO_InitTypeDef gpio_initled2 = {0};
	gpio_initled2.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio_initled2.GPIO_Speed = GPIO_Speed_50MHz;
	gpio_initled2.GPIO_Pin = LED2PIN;
	GPIO_Init(LED2PORT, &gpio_initled2);
	
	//Bind button
	GPIO_InitTypeDef gpio_initbtn = {0};
	gpio_initbtn.GPIO_Mode = GPIO_Mode_IPU;
	gpio_initbtn.GPIO_Speed = GPIO_Speed_50MHz;
	gpio_initbtn.GPIO_Pin = BINDBUTTONPIN;
	GPIO_Init(BINDBUTTONPORT, &gpio_initbtn);
}


void checkbindbutton(void)
{
	if ((GPIO_ReadInputDataBit(BINDBUTTONPORT, BINDBUTTONPIN) != 0))
		bindbuttonpressed = 1;
	else
	{
		// Start binding after letting go of button
		if (bindbuttonpressed){
			rxmode = RXMODE_BIND;
			rx_init();
		}
		bindbuttonpressed = 0;
	}
	
}

void rxsendusb(void)
{
	int curr_val;
	for (int i = 0; i < 3; i++)
  {
    curr_val = (rx[i] + 1) * 500; 
		usb_send_buffer[i] = curr_val << 6;
  }
  curr_val = rx[3] * 1000;  //throttle 
	usb_send_buffer[3] = curr_val << 6;
	for (int i = 0; i < 8; i++)
	{
		if(aux[i]) {
			usb_send_buffer[4] |= (1 << i);
		} else {
			usb_send_buffer[4] &= ~(1 << i);
		}
	}
	if(bDeviceState == CONFIGURED && PrevXferComplete) {
		USB_HID_Joystic_Send((void*)usb_send_buffer, 4 * 2 + 1);
	}
}

void ppm_send_usb(void)
{
	if(bDeviceState == CONFIGURED && PrevXferComplete) {
		USB_HID_Joystic_Send((void*)usb_send_buffer, 4 * 2 + 1);
		GPIOC->ODR ^= GPIO_Pin_13;
	}
}

void ppm_send_uart(void)
{
	usart_write(uart_header, sizeof(uart_header));
	usart_write((uint8_t*)ppm_buffer, 4 * sizeof(uint16_t));
	GPIOC->ODR ^= GPIO_Pin_13;
}


uint16_t ppm_to_joystic(uint16_t value)
{
	if(value < PPM_MIN) value = PPM_MIN;
	else if(value > PPM_MAX) value = PPM_MAX;
	value -= PPM_MIN;
	return value << 6; // [0; 1000] -> [0; 64000]
}

// Process each channel
void process_channel(uint8_t axis, uint16_t value)
{
	ppm_buffer[axis] = value;
	if(axis < 4) {
		// analog
		usb_send_buffer[axis] = ppm_to_joystic(value);
	} else if (axis < 12){
		// digital
		uint8_t btn_index = axis - 4;
		uint8_t btn_value = value < PPM_BTN_THRESHOLD;
		if(btn_value) {
			usb_send_buffer[4] |= (1 << btn_index);
		} else {
			usb_send_buffer[4] &= ~(1 << btn_index);
		}
	}
}

void process_impulse(uint16_t duration)
{
	ppm_decoder_decode(duration);
}

void usart_init(void)
{
	// USART GPIO init
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef gpio_init = {0};
	gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
	// TX
	gpio_init.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio_init.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOA, &gpio_init);
	
	// USART init
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	USART_InitTypeDef usart_init = {0};
	usart_init.USART_BaudRate = 115200;
	usart_init.USART_Mode = USART_Mode_Tx;
	usart_init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usart_init.USART_Parity = USART_Parity_No;
	usart_init.USART_StopBits = USART_StopBits_1;
	usart_init.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &usart_init);
	
	USART_Cmd(USART1, ENABLE);
}


void usart_send_byte(uint8_t byte)
{
	while(!USART_GetFlagStatus(USART1, USART_FLAG_TXE));
	USART1->DR = byte;
}

void usart_write(uint8_t* buffer, uint32_t size)
{
	for(uint32_t i = 0; i < size; i++) {
		usart_send_byte(buffer[i]);
	}
}

void failloop( int val)
{

	while(1)
	{
		for ( int i = 0 ; i < val; i++)
		{
		 led1on(255);		
		 delay(200000);
		 led1off(255);
		 delay(200000);			
		}
		delay(800000);
	}	
	
}
