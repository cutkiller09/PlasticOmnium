/*============================================================================*/
/*                        	ROBOSOFT                                      */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*/
/*! 
 * $Source: Library_PrivateCanOpenPdoManage.h
 * $Author: BRobin 
 * $Date: 2015/12/08  
 */
/*============================================================================*/
/* DESCRIPTION :                                                              */
/** \file  Library_PrivateCanOpenPdoManage.h
  * \brief Fichier Header du Service PrivateCanOpenPdoManage
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
 
Std_ReturnType ProCan_PdoManage_Initialize(void);
Std_ReturnType ProCan_PdoManage_Configure(const ts_CanopenCfg * const ps_canopen_cfg);
Std_ReturnType ProCan_PdoManage_RegisterTPdo(const uint8 u8_canopen_node_idx, const uint16 u16_canopen_tpdo_can_id, const uint8 u8_canopen_tpdo_timeout_val, uint8 * const pu8_canopen_pdo_channel);
Std_ReturnType ProCan_PdoManage_RegisterRPdo(const uint8 u8_canopen_node_idx, const uint16 u16_canopen_rpdo_can_id, uint8 * const pu8_canopen_pdo_channel);
Std_ReturnType ProCan_PdoManage_Send(const uint8 u8_canopen_node_idx, const uint8 u8_canopen_pdo_idx, const uint8 * const pu8_canopen_pdo_data, const uint8 u8_canopen_pdo_len,
		te_CanopenStatus * const pe_canopen_exchange_status);
Std_ReturnType ProCan_PdoManage_ProcessFrame(const ts_CanopenCanFrame * const ps_canopen_can_frame, uint8 * const u8_canopen_node_idx,
		ts_CanopenData * const ps_canopen_data, te_CanopenStatus * const pe_canopen_exchange_status, te_CanopenFrameProcessStatus * const pe_frame_processed);
Std_ReturnType ProCan_PdoManage_CheckTimeout(const uint8 u8_canopen_node_idx, const uint8 u8_canopen_tpdo_idx, te_CanopenStatus * const pe_canopen_exchange_status);
