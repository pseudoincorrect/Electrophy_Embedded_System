
#ifndef __COMMONDEFINE_H__
#define __COMMONDEFINE_H__

/********************************************************************/
//CHANGEABLE PARAMETER 

// This mode will generate an artifiacial signal (a sort of replacement of te RHD acquired signal) to test the system
//#define TESTBUFFER

//enable the change of Eta and Beta via receiver inputs
//#define PARAMETER_SELECTION 

// On compressing mode, compress only 4 chanel and send at the same time their non-compressed value to compare
#define COMPARISON 

#define ETA_      512					    // adaptation parameter
#define BETA_     8					     // error parameter
#define H_        120/128


/********************************************************************/
// FIXED PARAMETERS 
#define LOW		0
#define HIGH	1

#define CHANNEL_SIZE 	0x08 // must be a multiple of BYTES_PER_FRAME (NRF.h)

#define BYTES_PER_FRAME	   32 // ammount of byte in one packet transmission
#define NUMBER_OF_PACKETS	 30 // number of packet send at once (size of the buffer)
#define SAMPLE_BUFFER_SIZE 	(BYTES_PER_FRAME * NUMBER_OF_PACKETS) // defined in NRF.h

#define SIZE_BUFFER_RHD		 70	
#define SIZE_BUFFER_NRF		 3

#define NBIT 	2		// resolution of the compression

#define POW_2_NBIT  	(1 << NBIT) 			// 2^NBIT
#define CUT_VAL_SIZE 	(POW_2_NBIT - 1) // number of cut value


#define DEBUG_HIGH 	(GPIOA->BSRR |= GPIO_PIN_3)
#define DEBUG_LOW		(GPIOA->BSRR |= ((uint32_t) GPIO_PIN_3 << 16))

#define	SIZE_VALUE	 	 	25
#define INTERVAL_TEST  	4
#define SIZE_TEST				(SIZE_VALUE * INTERVAL_TEST)

#define STATE_INIT			__8ch_2bit__20kHz__C__

/**************************************************************/
//					Enum
/**************************************************************/
typedef enum
{
	__8ch_2bit__20kHz__C__ = 0x01,
	__4ch_16bit_20kHz_NC__ = 0x02,
	__8ch_16bit_10kHz_NC__ = 0x03,
  __8ch_8bit__20kHz_NC__ = 0x04,
  
} DataStateTypeDef;

#endif
