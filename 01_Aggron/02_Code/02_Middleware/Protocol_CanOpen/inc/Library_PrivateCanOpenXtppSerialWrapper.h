/*============================================================================*/
/*                        	ROBOSOFT                                      */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*/
/*! 
 * $Source: Library_XtppWrapperCanOpen.h 
 * $Author: BRobin 
 * $Date: 2015/12/08  
 */
/*============================================================================*/
/* DESCRIPTION :                                                              */
/** \file  Library_XtppWrapperCanOpen.h 
  * \brief Fichier Header du Service XtppWrapperCanOpen
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

#include "Driver_SerialDriver.h"
#include "Protocol_CanOpen.h"

/* Constants and types 							      */
/*============================================================================*/  

/* Exported Variables 							      */
/*============================================================================*/

/* Exported functions prototypes 					      */
/*============================================================================*/    
 
Std_ReturnType XtppWrapperCanOpen_Init(void);
Std_ReturnType XtppWrapperCanOpen_Configure(const serialdrv_cfg_type * const ps_canopen_xtpp_serial_cfg);
Std_ReturnType XtppWrapperCanOpen_Read(ts_CanopenCanFrame * const ps_can_frame, te_CanopenWrapperStatus * const pe_can_data_read_status);
Std_ReturnType XtppWrapperCanOpen_Write(const ts_CanopenCanFrame * const ps_can_frame, te_CanopenWrapperStatus * const pe_can_data_read_status);
Std_ReturnType XtppWrapperCanOpen_Close(void);
