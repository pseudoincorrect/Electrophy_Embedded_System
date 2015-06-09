#include "DataBuffer.h"

DataBuffer ElectrophyData;

volatile uint8_t * NRFptr;
volatile uint16_t * RHDptr;

/**************************************************************/
//					DataBuffer_Init
/**************************************************************/
void DataBuffer_Init(void)
{
	NRFptr = ElectrophyData.Data8[0];
	RHDptr = ElectrophyData.Data16[0];
	
	ElectrophyData.Write16_index  = 0;
	ElectrophyData.Read16_index 	= 0;
	
	ElectrophyData.Write8_index 	= 0;
	ElectrophyData.Write8_element	= 0;
	ElectrophyData.Read8_index 		= 0;

  ElectrophyData.DataState = FIRST_STATE;
}

/**************************************************************/
//					DataBuffer_Data16_CheckFill
/**************************************************************/	
uint8_t DataBuffer_Data16_CheckFill(void)
{
	if (ElectrophyData.Write16_index != ElectrophyData.Read16_index)
		return 1;
	else
		return 0;
}

uint16_t previousRead16_index;
/**************************************************************/
//					DataBuffer_Read16
/**************************************************************/
static uint16_t * DataBufferRead16(void)
{
	previousRead16_index = ElectrophyData.Read16_index;
	ElectrophyData.Read16_index++;
	
	if(ElectrophyData.Read16_index >= SIZE_BUFFER_RHD)
		ElectrophyData.Read16_index = 0;
	
	return (ElectrophyData.Data16[previousRead16_index]); 
}

/**************************************************************/
//					DataBufferWrite16
/**************************************************************/
uint16_t * DataBuffer_Write16(void)
{	
	ElectrophyData.Write16_index++;
	
	if(ElectrophyData.Write16_index >= SIZE_BUFFER_RHD)
		ElectrophyData.Write16_index = 0;

	return (ElectrophyData.Data16[ElectrophyData.Write16_index]); 
}

/**************************************************************/
//					DataBuffer_Data8_CheckFill
/**************************************************************/
uint8_t DataBuffer_Data8_CheckFill(void)
{
	if (ElectrophyData.Read8_index != ElectrophyData.Write8_index)
		return 1;
	else
		return 0;
}

uint16_t previousRead8_index;
/**************************************************************/
//					DataBufferRead8
/**************************************************************/
uint8_t * DataBufferRead8(void)
{	
	previousRead8_index = ElectrophyData.Read8_index;
	ElectrophyData.Read8_index++;
	
	if(ElectrophyData.Read8_index >= SIZE_BUFFER_NRF)
		ElectrophyData.Read8_index  = 0;
		
	return (ElectrophyData.Data8[previousRead8_index]); 
}

uint16_t previousWrite8_element;
uint16_t previousWrite8_index;
/**************************************************************/
//					 DataBufferWrite8
/**************************************************************/
static uint8_t *  DataBufferWrite8(void)
{	
	previousWrite8_element = ElectrophyData.Write8_element;
	previousWrite8_index 	 = ElectrophyData.Write8_index;
	
	ElectrophyData.Write8_element += CHANNEL_SIZE;
	
	if(ElectrophyData.Write8_element > (SAMPLE_BUFFER_SIZE - CHANNEL_SIZE))
	{
		ElectrophyData.Write8_element = 0;
		ElectrophyData.Write8_index++;
	
		if(ElectrophyData.Write8_index >= SIZE_BUFFER_NRF)
			ElectrophyData.Write8_index = 0;
	}
	return (&ElectrophyData.Data8[previousWrite8_index][previousWrite8_element]); 
}

/**************************************************************/
//					 DataBuffer_ApplyReset
/**************************************************************/
void  DataBuffer_ApplyReset(void)
{	
	ElectrophyData.Write8_element += BYTES_PER_FRAME;
	
	FBAR_Reset(DataBufferRead16(), &ElectrophyData.Data8[ElectrophyData.Write8_index][0] );	; 
}

static uint16_t * DataBufferRead16ptr;
static uint8_t  * DataBufferWrite8ptr;
/**************************************************************/
//					DataBuffer_Process
/**************************************************************/
void DataBuffer_Process(void)
{
	if(DataBuffer_Data16_CheckFill())
	{
    if(ElectrophyData.DataState == __8ch_16bit_20kHz__C__)  // if Compression
    {
      if(ElectrophyData.Write8_element)
        FBAR_Compress(DataBufferRead16(),  DataBufferWrite8() );			
      else
        DataBuffer_ApplyReset();
    }
    else // if NOT Compression
		{
       DataBufferRead16ptr = DataBufferRead16();
       DataBufferWrite8ptr = DataBufferWrite8();
      
       FBAR_Dissemble(DataBufferRead16ptr, DataBufferWrite8ptr);			
      
       // if we send 8 channels of 16 bit ,and not 4, it will take twice more place
       // on the sending buffer , thius we call "DataBufferWrite8()" twice
       if(ElectrophyData.DataState == __8ch_16bit_10kHz_NC__)
       {
         DataBufferWrite8ptr = DataBufferWrite8();
         DataBufferRead16ptr += CHANNEL_SIZE;
         
         FBAR_Dissemble(DataBufferRead16ptr, DataBufferWrite8ptr);      
       }
    }	
	}
}

/**************************************************************/
//					DataBuffer_ChangeState
/**************************************************************/
void DataBuffer_ChangeState(DataStateTypeDef DataState)
{
  ElectrophyData.DataState = DataState;  
}



























