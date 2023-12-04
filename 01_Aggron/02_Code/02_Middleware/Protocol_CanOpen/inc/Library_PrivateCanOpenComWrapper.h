/*============================================================================*/
/*                        	ROBOSOFT                                      */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*/
/*! 
 * $Source: Library_ComWrapperCanOpen.h
 * $Author: BRobin 
 * $Date: 2015/12/08  
 */
/*============================================================================*/
/* DESCRIPTION :                                                              */
/** \file  Library_ComWrapperCanOpen.h
  * \brief Fichier Header du Service ComWrapperCanOpen
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

/* Includes 								      */
/*============================================================================*/  
#include "Protocol_CanOpen.h"

/* Constants and types 							      */
/*============================================================================*/  

/* Exported Variables 							      */
/*============================================================================*/

/* Exported functions prototypes 					      */
/*============================================================================*/    
 
Std_ReturnType ComWrapperCanOpen_Init(void);
Std_ReturnType ComWrapperCanOpen_Configure(const ts_CanopenCfg * const ps_canopen_cfg);
Std_ReturnType ComWrapperCanOpen_Receive(ts_CanopenCanFrame * const ps_can_frame, te_CanopenWrapperStatus * const pe_can_data_read_status);
Std_ReturnType ComWrapperCanOpen_Send(const ts_CanopenCanFrame * const ps_can_frame, te_CanopenWrapperStatus * const pe_can_data_write_status);
Std_ReturnType ComWrapperCanOpen_Close(const ts_CanopenCfg * const ps_canopen_cfg);
