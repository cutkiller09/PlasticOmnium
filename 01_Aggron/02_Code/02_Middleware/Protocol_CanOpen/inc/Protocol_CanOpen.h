/*============================================================================*/
/*                        	ROBOSOFT                                      */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*/
/*! 
 * $Source: Protocol_CanOpen.h 
 * $Author: SLoisel 
 * $Date: 2015/12/08  
 */
/*============================================================================*/
/* DESCRIPTION :                                                              */
/** \file  Protocol_CanOpen.h 
  * \brief Header file of CanOpen Protocol
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

/* Constants and types 							      */
/*============================================================================*/  

/**************************************/
/* Library static configuration */

/**
 * @brief CANopen protocol configuration
 * Number of Number of CANopen physical nodes managed by the library
 */
#define K_CANOPEN_NODES_NR				4

#define K_MAX_PORT_NAME 16

/**************************************/
/* Lower layer interface description */

/* Definition of the CANopen data frame */
#define K_CANOPEN_DATA_FRAME_LEN		8

/* CAN frame kind */
typedef enum {
	E_CANOPEN_NORMAL_FRAME = 0,
	E_CANOPEN_ERR_FRAME = 1,
	E_CANOPEN_RTR_FRAME = 2,
} te_CanopenCanFrameType;

/* Definition of the data exchange between the CANopen driver and the underlying communication layer */
typedef struct
{
	/* Frame type */
	te_CanopenCanFrameType e_can_frame_type;

	/* Frame CAN identifier */
	uint32 u32_can_id;

	/* Frame data */
	uint8 au8_can_data[K_CANOPEN_DATA_FRAME_LEN];

	/* Frame data len */
	uint8 u8_can_dlc;
} ts_CanopenCanFrame;

/* Definition of the CANopen wrapper status */
typedef enum {
	E_CANOPENWRP_NO_DATA = 0,
	E_CANOPENWRP_DATA_OK = 1,
	E_CANOPENWRP_DRV_ERR = 2
} te_CanopenWrapperStatus;


/**************************************/
/* NMT management */

/* Node identifier to send an NMT broadcast message (i.e. to all CANopen nodes) */
#define K_CANOPEN_NMT_BROADCAST_ID		0xFF

/* Definition of the CANopen NMT commands */
typedef enum
{
	E_CANOPEN_NMT_START = 1,
	E_CANOPEN_NMT_STOP = 2,
	E_CANOPEN_NMT_PREOP = 128,
	E_CANOPEN_NMT_RESET_NODE = 129,
	E_CANOPEN_NMT_RESET_COM = 130
} te_CanopenNmtCommands;

/* Definition of the CANopen NMT states */
typedef enum
{
	E_CANOPEN_NMT_BOOTUP = 0,
	E_CANOPEN_NMT_STOPPED = 1,
	E_CANOPEN_NMT_OPERATIONAL = 2,
	E_CANOPEN_NMT_PREOPE = 3,
	E_CANOPEN_NMT_UNDEF = 4
} te_CanopenNmtStates;


/**************************************/
/* SDO management */

/* Definition of the data type exchanged in a CANopen SDO transfer */
typedef enum {
	E_CANOPEN_NODATA = 0,
	E_CANOPEN_SDO_STR  = 1,
	E_CANOPEN_SDO_INT8 = 2,
	E_CANOPEN_SDO_INT16 = 3,
	E_CANOPEN_SDO_INT24 = 4,
	E_CANOPEN_SDO_INT32 = 5,
	E_CANOPEN_NMT_STATE = 6,
	E_CANOPEN_PDO = 7
} te_CanopenTransferKind;

/* Definition of available abort reasons */
typedef enum {
	E_CANOPEN_OBJ_LENGTH_TOO_HIGH = 0,
	E_CANOPEN_OBJ_LENGTH_TOO_LOW = 1,
	E_CANOPEN_WRONG_SEQUENCE = 2,
	E_CANOPEN_INTERNAL_ERROR = 3
} te_CanopenSdoAbortReason;

/**************************************/
/* PDO management */

/* Maximum number of managed PDOs */
#define K_CANOPEN_PDO_MAX_NR		4


/**************************************/
/* Upper layer interface description */

/* Definition of data exchanged through CANopen protocol */
/* Data string length will be at most 7 bytes (CAN frame length minus command byte) */
typedef struct {
	/* e_data_kind = E_CANOPEN_INTxx   		=> u32_integer_val_str_len = data */
	/* e_data_kind = E_CANOPEN_SDO_STR     	=> au8_string_val = data and u32_integer_val_str_len = data string length */
	/* e_data_kind = E_CANOPEN_NMT_STATE	=> e_canopen_state = data */
	/* e_data_kind = E_CANOPEN_PDO			=> au8_string_val = PDO data and u32_integer_val_str_len = PDO data length */
	/*                                         u8_tpdo_idx = TPDO channel index (i.e. the TPDO CAN identifier given by the configuration of the CANopen channel */
	te_CanopenTransferKind	e_data_kind;
	uint32						u32_integer_val_str_len;
	uint8						au8_string_val[K_CANOPEN_DATA_FRAME_LEN];
	te_CanopenNmtStates 		e_canopen_state;
	uint8						u8_tpdo_idx;
} ts_CanopenData;

/* Definition of CANopen exchange status */
typedef enum {
	E_CANOPEN_TRANSFER_IN_PROGRESS = 0,
	E_CANOPEN_TRANSFER_COMPLETE = 1,
	E_CANOPEN_TRANSFER_ERROR = 2,
	E_CANOPEN_SDO_ABORT_COMPLETE = 3,
	E_CANOPEN_TIMEOUT_ERROR = 4,
	E_CANOPEN_NETWORK_INTERFACE_ERROR = 5
} te_CanopenStatus;

/* Definition of the CANopen frame processing status */
typedef enum {
	E_CANOPEN_FRAME_PROCESSED_NO_DATA = 0,
	E_CANOPEN_FRAME_PROCESSED_WITH_DATA = 1,
	E_CANOPEN_FRAME_NOT_PROCESSED = 2,
	E_CANOPEN_FRAME_NOT_RECEIVED = 3
} te_CanopenFrameProcessStatus;


/**************************************/
/* Library runtime configuration */

/* Definition of available CANopen wrappers */
typedef enum {
	E_CANOPENWRP_SERIAL_XTPP = 0,
	E_CANOPENWRP_PCAN = 1,
	E_CANOPENWRP_NR = 2		/* Number of CANopen wrappers */
} te_CanopenWrapperType;

/* Definition of a CANopen port configuration structure */
typedef struct
{
	/* CANopen identifier */
	uint8 	u8_canopen_node_id_cfg;

	/* SDO protocol timeout value */
	uint8 	u8_canopen_sdo_timeout_cfg;

	/* NMT protocol timeout value */
	uint8 	u8_canopen_nmt_timeout_cfg;

	/* Transmit PDO COB-ID configuration */
	uint16	u16_canopen_tpdo_canid_cfg[K_CANOPEN_PDO_MAX_NR];
	uint8	u8_canopen_tpdo_nr_cfg;

	/* Receive PDO COB-ID configuration */
	uint16	u16_canopen_rpdo_canid_cfg[K_CANOPEN_PDO_MAX_NR];
	uint8	u8_canopen_rpdo_timeout_cfg[K_CANOPEN_PDO_MAX_NR];
	uint8	u8_canopen_rpdo_nr_cfg;
} ts_CanopenPortCfg;


/* Definition of the CANopen configuration structure */
typedef struct {
	/* Configuration of XTPP serial wrapper if used, NULL_PTR otherwise */
	serialdrv_cfg_type s_canopen_xtpp_serial_cfg;

	/* Configuration of PCAN wrapper if used, NULL_PTR otherwise */
	uint8 au8_canopen_pcan_ifname_cfg[K_MAX_PORT_NAME];

	/* Wrapper used for communication */
	te_CanopenWrapperType e_canopen_wrapper_cfg;

	/* CANopen ports configuration */
	ts_CanopenPortCfg as_canopen_ports_cfg[K_CANOPEN_NODES_NR];
	uint8 u8_canopen_nodes_nr_cfg;
} ts_CanopenCfg;



/* Exported Variables 							      */
/*============================================================================*/

/* Exported functions prototypes 					      */
/*============================================================================*/    
 
/* CANopen library initialization and termination */
Std_ReturnType ProCan_Initialize(void);
Std_ReturnType ProCan_Configure(const ts_CanopenCfg * const ps_canopen_cfg);
Std_ReturnType ProCan_Close(const ts_CanopenCfg * const ps_canopen_cfg);

/* Start a CANopen SDO upload request */
Std_ReturnType ProCan_Start_SdoUpload(const uint8 u8_canopen_node_idx, const uint16 u16_canopen_object_idx, const uint8 u8_canopen_object_subidx, te_CanopenStatus * const pe_canopen_exchange_status);

/* Start a CANopen SDO download request */
Std_ReturnType ProCan_Start_SdoDownload(const uint8 u8_canopen_node_idx, const uint16 u16_canopen_object_idx, const uint8 u8_canopen_object_subidx,
										ts_CanopenData * const ps_canopen_data, te_CanopenStatus * const pe_canopen_exchange_status);

/* Process received CANopen frames */
Std_ReturnType ProCan_Process_Frames(uint8 * const pu8_canopen_node_idx, ts_CanopenData * const ps_canopen_data,
		te_CanopenStatus * const pe_canopen_exchange_status, te_CanopenFrameProcessStatus * const pe_frame_processed);

/* Register PDOs */
Std_ReturnType ProCan_RegisterTPdo(const uint8 u8_canopen_node_idx, const uint16 u16_canopen_tpdo_can_id, const uint8 u8_canopen_tpdo_timeout_val, uint8 * const pu8_canopen_pdo_channel);
Std_ReturnType ProCan_RegisterRPdo(const uint8 u8_canopen_node_idx, const uint16 u16_canopen_rpdo_can_id, uint8 * const pu8_canopen_pdo_channel);

/* Send RPDO data */
Std_ReturnType ProCan_SendRPdo(const uint8 u8_canopen_node_idx, const uint8 u8_canopen_pdo_idx, const uint8 * const pu8_canopen_pdo_data, const uint8 u8_canopen_pdo_len,
		te_CanopenStatus * const pe_canopen_exchange_status);


/* Control the communication state of network nodes */
Std_ReturnType ProCan_ControlNmtState(const uint8 u8_canopen_node_idx, const te_CanopenNmtCommands e_canopen_nmt_cmd, te_CanopenStatus * const pe_canopen_exchange_status);
/* Receive the communication state of a network node */
Std_ReturnType ProCan_RequestNmtState(const uint8 u8_canopen_node_idx, te_CanopenStatus * const pe_canopen_exchange_status);

/* Send the SYNC message */
Std_ReturnType ProCan_SendSync(te_CanopenStatus * const pe_canopen_exchange_status);


/* Check SDO server response time-out */
Std_ReturnType ProCan_Check_SdoTimeout(const uint8 u8_canopen_node_idx, te_CanopenStatus * const pe_canopen_exchange_status);
/* Check PDO server response time-out */
Std_ReturnType ProCan_Check_PdoTimeout(const uint8 u8_canopen_node_idx, const uint8 u8_canopen_tpdo_idx, te_CanopenStatus * const pe_canopen_exchange_status);
/* Check NMT slave response time-out */
Std_ReturnType ProCan_Check_NmtTimeout(const uint8 u8_canopen_node_idx, te_CanopenStatus * const pe_canopen_exchange_status);

/* Abort the current SDO transfer */
Std_ReturnType ProCan_Abort_SdoTransfer(const uint8 u8_canopen_node_idx, const te_CanopenSdoAbortReason e_canopen_errinfo_abort);
