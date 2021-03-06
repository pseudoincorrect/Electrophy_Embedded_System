// *************************************************************************
/* ************************************************************************
  * @file    SampleSend.c
  * @author  Maxime CLEMENT
  * @version V1.0
  * @date    06-Oct-2015
  * @brief   RHD2000 module driver driver.
  *          This file provides functions to manage the SPI module  RHD2000:        
  @verbatim
*/

#include "SampleSend.h"

static DataStateTypeDef DataState = STATE_INIT;

volatile uint32_t Tim1DIER;
volatile uint32_t Tim1SR;
	
static TIM_HandleTypeDef    TimHandle;

/**************************************************************/
//					SampleSend_Init
/**************************************************************/
void SampleSend_Init(void)
{ 
	NRF_Init();
	RHD_Init();
	DataBuffer_Init(STATE_INIT, ETA_, BETA_);
	SampleSend_SetState(STATE_INIT);
}

/**************************************************************/
//					TIM2Init
/**************************************************************/
static void TIM2Init(uint32_t reloadValue, uint16_t prescalerValue)
{	
  __TIM2_CLK_ENABLE(); 

	TimHandle.Instance = TIM2;

	TimHandle.Init.Period            = reloadValue;
  TimHandle.Init.Prescaler         = prescalerValue;
  TimHandle.Init.ClockDivision     = 0;
  TimHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
  TimHandle.Init.RepetitionCounter = 0;
	HAL_TIM_Base_Init(&TimHandle);
  
	// Set the TIMx priority 
	HAL_NVIC_SetPriority(TIM2_IRQn, 1, 0);	
	// Enable the TIMx global Interrupt 
  HAL_NVIC_EnableIRQ(TIM2_IRQn);
	
	__HAL_TIM_DISABLE_IT(&TimHandle, TIM_IT_UPDATE); //The specified TIM3 interrupt : update
  __HAL_TIM_DISABLE(&TimHandle);
}

/**************************************************************/
//					TIM2_IRQHandler
/**************************************************************/
void TIM2_IRQHandler(void)
{	
	if(__HAL_TIM_GET_FLAG(&TimHandle, TIM_FLAG_UPDATE) != RESET)
	{
		if(__HAL_TIM_GET_ITSTATUS(&TimHandle, TIM_IT_UPDATE) !=RESET)
		{ 
			__HAL_TIM_CLEAR_IT(&TimHandle, TIM_IT_UPDATE); // Remove TIMx update interrupt flag 
			__HAL_TIM_CLEAR_FLAG(&TimHandle, TIM_IT_UPDATE);
		}	
		//DEBUG_HIGH;		
#ifdef TESTBUFFER
		RHD_SampleTest(DataBuffer_Write16(), 1);
#else
		RHD_Sample(DataBuffer_Write16());
#endif
		//DEBUG_LOW;
	}
}	

/**************************************************************/
//					SampleSend_Enable
/**************************************************************/
void SampleSend_Enable(uint8_t state)
{		
		if(state)
		{
			__HAL_TIM_ENABLE_IT(&TimHandle, TIM_IT_UPDATE); // TIM2 interrupt : update
			__HAL_TIM_ENABLE(&TimHandle);
		}
		else
		{
			__HAL_TIM_DISABLE_IT(&TimHandle, TIM_IT_UPDATE); // TIM2 interrupt : update
			__HAL_TIM_DISABLE(&TimHandle);				
		}
}

/**************************************************************/
//					SampleSend_Acquisition
/**************************************************************/
void SampleSend_Acquisition(void)
{
	if (DataBuffer_Data8_CheckFill())
	{	
		NRF_SendBuffer(DataBufferRead8());
	}
}

/**************************************************************/
//					SampleSend_SetState
/**************************************************************/
void SampleSend_SetState(DataStateTypeDef State)
{
  DataState = State;

  //(268,8)  = 20 kHz sample
	//(245,12) = 15 kHz sample
	//(262,13) = 13 kHz sample
	//(250,18) = 10 kHz sample
  
  if(DataState == __8ch_16bit_10kHz_NC__)
     TIM2Init (250,18); 
  else if (DataState == __8ch_2bit__20kHz__C__)
  { 
    #ifdef PARAMETER_SELECTION
    TIM2Init (250,16); 
		//TIM2Init (250,40); 
    #else
    TIM2Init (245,12); 
    #endif
  }
  else
     TIM2Init (245,12); 
}








