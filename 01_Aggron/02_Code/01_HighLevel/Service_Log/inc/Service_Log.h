/*============================================================================*/
/*                        	ROBOSOFT                                      */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*/
/*! 
 * $Source: Service_Log.h 
 * $Author: Slo 
 * $Date: 2015/12/08  
 */
/*============================================================================*/
/* DESCRIPTION :                                                              */
/** \file  Service_Log.h 
  * \brief Fichier Header du Service Log
*/
/*============================================================================*/
/* COPYRIGHT (C) ROBOSOFT 2015	                                              */ 
/* ALL RIGHTS RESERVED                                                        */
/*                                                                            */
/* The reproduction, transmission, or use of this document or its content is  */
/* not permitted without express written authority. Offenders will be liable  */
/* for damages.                                                               */
/* All rights, including rights created by patent grant or registration of a  */
/* utility model or design, are reserved.                                     */
/*                                                                            */
/*============================================================================*/  

#pragma once

/* Includes 							              */
/*============================================================================*/  

/* Constants and types 							      */
/*============================================================================*/  
typedef enum
{
	E_LOG_LEVEL_0 = 0,
	E_LOG_LEVEL_1,
	E_LOG_LEVEL_2,
	E_LOG_LEVEL_3,
	E_LOG_LEVEL_4,
	E_LOG_LEVEL_5,
	E_LOG_LEVEL_6,
	E_LOG_LEVEL_7
}te_LogLevel;

typedef enum
{
	E_LOG_1MS=0,
	E_LOG_10MS
}te_LogSelection;

typedef enum
{
	E_LOG_STOP=0,
	E_LOG_START
}te_LogState;


/* Exported Variables 						              */
/*============================================================================*/

/* Exported functions prototypes 					      */
/*============================================================================*/    
 
Std_ReturnType SrvLog_Initialize(te_LogSelection e_LogSelection) ;
void SrvLog_Stop(void);
void SrvLog_Start(void);
void SrvLog_Set_Selection(const te_LogSelection e_LogSelection);
void SrvLog_Set_Level(const te_LogLevel e_LogLevel);
void SrvLog_Increment_Level(void);
void SrvLog_Decrement_Level(void);
void SrvLog_RaiseEvent_State(const uint8 * const pu8_LogInformation, const Std_ReturnType t_EventStatus );
Std_ReturnType SrvLog_Get_LogFile(uint8 *pu8_LogFile_10ms, uint8 *pu8_LogFile_1ms);
