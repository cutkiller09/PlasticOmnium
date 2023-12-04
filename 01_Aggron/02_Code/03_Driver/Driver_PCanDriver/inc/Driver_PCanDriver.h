/*============================================================================*/
/*                        	ROBOSOFT                                      */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*/
/*!
 * $Source: Driver_PCanDriver.h
 * $Author: MDEGA
 * $Date: 2015/12/28
 */
/*============================================================================*/
/* DESCRIPTION :                                                              */
/** \file  Driver_PCanDriver.h
  * \brief Fichier Header du Service PCanDriver
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

/* Constants and types 							      */
/*============================================================================*/

/*
 * Controller Area Network Identifier structure
 *
 * bit 0-10	: CAN identifier (11 bit)
 * bit 28-11: reserved (0)
 * bit 29	: error message frame flag (0 = data frame, 1 = error message)
 * bit 30	: remote transmission request flag (1 = rtr frame)
 * bit 31	: reserved (0)
 */
typedef uint32 t_canid;

/* Special address description flags for the CAN_ID */
#define K_DRVPCAN_CAN_RTR_FLAG 0x40000000U /* remote transmission request */
#define K_DRVPCAN_CAN_ERR_FLAG 0x20000000U /* error message frame */

/* Valid bits in CAN ID for frame formats */
#define K_DRVPCAN_CAN_SFF_MASK 0x000007FFU /* standard frame format (SFF) */
#define K_DRVPCAN_CAN_ERR_MASK 0x1FFFFFFFU /* omit EFF, RTR, ERR flags */


/* CAN payload length and DLC definitions according to ISO 11898-1 */
#define K_DRVPCAN_MAX_DLC 8
#define K_DRVPCAN_MAX_DLEN 8

/**
 * Basic CAN frame structure used to communicate with PCan driver
 * @can_id:  CAN ID of the frame and CAN_*_FLAG flags, see canid_t definition
 * @can_dlc: frame payload length in byte (0 .. 8) aka data length code
 *           N.B. the DLC field from ISO 11898-1 Chapter 8.4.2.3 has a 1:1
 *           mapping of the 'data length code' to the real payload length
 * @data:    CAN frame payload (up to 8 byte)
 */
typedef struct {
	/* 32 bit CAN_ID + RTR/ERR flags */
	t_canid t_can_id;
	/* frame payload length in byte (0 .. K_DRVPCAN_MAX_DLEN) */
	uint8   u8_can_dlc;
	uint8   au8_data[K_DRVPCAN_MAX_DLEN] __attribute__((aligned(8)));
} ts_PCanFrame;
#define K_DRVPCAN_CAN_MTU		(sizeof(ts_PCanFrame))


/* Definition of CAN handler type */
typedef sint32 t_CanDrvHandler;

/* Uninitialized PCAN handle */
#define K_DRVPCAN_CLOSED_HANDLE		-1


/* Exported Variables 							      */
/*============================================================================*/

/* Exported functions prototypes 					      */
/*============================================================================*/

Std_ReturnType DrvPCan_Initialize(void);
Std_ReturnType DrvPCan_Open(const uint8 * const pu8_can_ifname, t_CanDrvHandler * const pt_can_handler);
Std_ReturnType DrvPCan_Close(t_CanDrvHandler * const pt_can_handler);
Std_ReturnType DrvPCan_Receive(const t_CanDrvHandler t_can_handler, ts_PCanFrame * const ps_can_frame, bool * const pb_can_data_avail);
Std_ReturnType DrvPCan_Send(const t_CanDrvHandler t_can_handler, const ts_PCanFrame * const ps_can_frame);
