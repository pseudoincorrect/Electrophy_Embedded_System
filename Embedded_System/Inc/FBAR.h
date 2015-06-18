/********************************************************************************
* @file    FBAR.h
* @author  Maxime Clement
* @version V1.0
* @date    01-Mar-2015
* @brief   Header file of the RHD2132 module.
*******************************************************************************/

#ifndef __FBAR_H__
#define __FBAR_H__

#include <stdint.h>
#include <math.h>
#include <CommonDefine.h>

void FBAR_Init(uint8_t EtaIndex);

void FBAR_Reset(uint16_t * bufferFrom, uint8_t * bufferTo);

void FBAR_Compress(uint16_t * bufferCompress, uint8_t * bufferSample);

void FBAR_Dissemble(uint16_t * bufferFrom, uint8_t * bufferTo, DataStateTypeDef DataState);

#endif
