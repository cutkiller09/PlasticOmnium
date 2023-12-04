/*============================================================================*/
/*                        	ROBOSOFT                                      */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*/
/*! 
 * $Source: Library_PrivateCanOpenNmtManage.h
 * $Author: BRobin 
 * $Date: 2015/12/08  
 */
/*============================================================================*/
/* DESCRIPTION :                                                              */
/** \file  Library_PrivateCanOpenNmtManage.h
  * \brief Fichier Header du Service PrivateCanOpenNmtManage
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
 
Std_ReturnType ProCan_NmtManage_Initialize(void);
Std_ReturnType ProCan_NmtManage_Configure(const ts_CanopenCfg * const ps_canopen_cfg);
Std_ReturnType ProCan_NmtManage_StateControl(const uint8 u8_canopen_node_idx, const te_CanopenNmtCommands e_canopen_nmt_cmd, te_CanopenStatus * const pe_canopen_exchange_status);
Std_ReturnType ProCan_NmtManage_StateRequest(const uint8 u8_canopen_node_idx, te_CanopenStatus * const pe_canopen_exchange_status);
Std_ReturnType ProCan_NmtManage_ProcessFrame(const ts_CanopenCanFrame * const ps_canopen_can_frame, uint8 * const u8_canopen_node_idx,
		ts_CanopenData * const ps_canopen_data, te_CanopenStatus * const pe_canopen_exchange_status, te_CanopenFrameProcessStatus * const pe_frame_processed);
Std_ReturnType ProCan_NmtManage_CheckTimeout(const uint8 u8_canopen_node_idx, te_CanopenStatus * const pe_canopen_exchange_status);
