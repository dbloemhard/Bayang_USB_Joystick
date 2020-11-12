// defines for things that do not normally need changing
// TX Configuration
//#define USE_STOCK_TX
//#define USE_MULTI
#define USE_DEVO

// Pin mapping
// NRF24L01 configuration
#define SOFTSPI_4WIRE
#define SPI_MISO_PIN GPIO_Pin_14
#define SPI_MISO_PORT GPIOB
#define SPI_MOSI_PIN GPIO_Pin_15
#define SPI_MOSI_PORT GPIOB
#define SPI_CLK_PIN GPIO_Pin_13
#define SPI_CLK_PORT GPIOB
#define SPI_SS_PIN GPIO_Pin_7
#define SPI_SS_PORT GPIOB
#define SPI_CE_PIN GPIO_Pin_12
#define SPI_CE_PORT GPIOB

// Check radio is responding during initialization
#define RADIO_CHECK
#define RADIO_XN297

//LED Configuration
#define LED1PIN GPIO_Pin_13
#define LED1PORT GPIOC
#define LED2PIN GPIO_Pin_1
#define LED2PORT GPIOA

//Button configuration
#define BINDBUTTONPIN GPIO_Pin_0
#define BINDBUTTONPORT GPIOA

//Bayang protocol configuration
// failsafe time in uS
#define FAILSAFETIME 1000000  // one second
#define AUXNUMBER 8  // Only 8 aux channels (12 total channels) supported

// Debug radio
// #define RXDEBUG
//Default data rate 1000k
//#define RX_DATARATE_250K

#define RXMODE_BIND 0
#define RXMODE_NORMAL (!RXMODE_BIND)

#ifdef USE_DEVO
// devo tx channel mapping
// also for nr24multipro
#define CH_INV 0      // Channel 5
#define CH_FLIP 1     // Channel 6
#define CH_PIC 2      // Channel 7
#define CH_VID 3      // Channel 8
#define CH_HEADFREE 4 // Channel 9
#define CH_RTH 5      // Channel 10
#define CH_TO 6       // Channel 11
#define CH_EMG 7      // Channel 12
#endif

#ifdef USE_MULTI
// multimodule mapping ( taranis )
#define CH_FLIP 0     // Channel 5
#define CH_RTH 1      // Channel 6
#define CH_PIC 2      // Channel 7
#define CH_VID 3      // Channel 8
#define CH_HEADFREE 4 // Channel 9
#define CH_INV 5      // Channel 10
#define CH_TO 6       // Channel 12
#define CH_EMG 7      // Channel 13
#endif

#ifdef USE_STOCK_TX
#define CH_EXPERT 0
#define CH_AUX1 1
#define CH_HEADFREE 2
#define CH_RLL_TRIM 3
#define CH_PIT_TRIM 4
#endif


#define int32 int_fast32_t
#define int16 int_fast16_t
#define int8 int_fast8_t

#define uint32 uint_fast32_t
#define uint16 uint_fast16_t
#define uint8 uint_fast8_t

// define logic - do not change
///////////////

#pragma diag_warning 1035 , 177 , 4017
#pragma diag_error 260



