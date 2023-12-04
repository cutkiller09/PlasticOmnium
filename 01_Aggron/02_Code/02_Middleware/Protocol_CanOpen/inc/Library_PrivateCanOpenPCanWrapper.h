/*============================================================================*/
/*                        	ROBOSOFT                                      */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*/
/*! 
 * $Source: Library_PCanWrapperCanOpen.h
 * $Author: BRobin 
 * $Date: 2015/12/08  
 */
/*============================================================================*/
/* DESCRIPTION :                                                              */
/** \file  Library_PCanWrapperCanOpen.h
  * \brief Fichier Header du Service PCanWrapperCanOpen
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
 
Std_ReturnType PCanWrapperCanOpen_Init(void);
Std_ReturnType PCanWrapperCanOpen_Configure(const uint8 * const pu8_canopen_pcan_ifname_cfg);
Std_ReturnType PCanWrapperCanOpen_Read(ts_CanopenCanFrame * const ps_can_frame, te_CanopenWrapperStatus * const pe_can_data_read_status);
Std_ReturnType PCanWrapperCanOpen_Write(const ts_CanopenCanFrame * const ps_can_frame, te_CanopenWrapperStatus * const pe_can_data_write_status);
Std_ReturnType PCanWrapperCanOpen_Close(void);
