/*============================================================================*/
/*                        	ROBOSOFT                                      */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*/
/*! 
 * $Source: Library_PrivateCanOpenSdoManage.c
 * $Author: BRobin 
 * $Date: 2015/12/08  
 */
/*============================================================================*/
/* DESCRIPTION :                                                              */
/** \file  Library_PrivateCanOpenSdoManage.c
  * \brief Fichier Source du Service PrivateCanOpenSdoManage
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

/* Includes 								      */
/*============================================================================*/ 
#include "types.h"
#include "Library_StdLib.h"
#include "Library_PrivateCanOpenSdoManage.h"
#include "Library_PrivateCanOpenComWrapper.h"
#include <stdio.h>
#include <string.h>

/* Constants and types  						      */
/*============================================================================*/ 
#define K_DEBUG

/******************************/
/* SDO management             */

/* CANopen node identifier mask within COB-ID */
#define K_CANOPEN_NODE_ID_FROM_CANID		0x07F

/* CANopen function code mask within COB-ID */
#define K_CANOPEN_FUNC_CODE_FROM_CANID		0x780

/* Default SDO client-to-server CAN identifier */
/* COB-ID = 11-bit CAN identifier = 110 0|node slave identifier */
#define K_CANOPEN_SDO_CLIENT_CANID			0x600

/* Default SDO server-to-client CAN identifier */
/* COB-ID = 11-bit CAN identifier = 101 1|node slave identifier */
#define K_CANOPEN_SDO_SERVER_CANID			0x580

/* Size of SDO frames */
#define K_CANOPEN_SDO_FRAME_DLC				8

/* SDO initiate upload response : command = 010xxxxx */
#define K_CANOPEN_SDO_INIT_UPLOAD			0x40

/* SDO upload segment request : command = 011xxxxx */
#define K_CANOPEN_SDO_SEG_UPLOAD			0x60

/* SDO upload segment response : command = 000xxxxx */
#define K_CANOPEN_SDO_SEG_DOWNLOAD			0x00

/* SDO initiate download request : command = 001xxxxx */
#define K_CANOPEN_SDO_INIT_DOWNLOAD			0x20

/* SDO abort : command = 100xxxxx */
#define K_CANOPEN_SDO_ABORT_REQUEST			0x80

/* SDO segmented transfer not complete bit value */
#define K_CANOPEN_SDO_SEG_TRANSFER_NOT_COMPLETE	0

/* SDO segmented transfer complete bit value */
#define K_CANOPEN_SDO_SEG_TRANSFER_COMPLETE		1


/**************************************/
/* Error management */

/* Error codes */
#define K_CANOPEN_SERVICE_ERRCLASS			0x05000000
#define K_CANOPEN_ACCESS_ERRCLASS			0x06000000
#define K_CANOPEN_OTHER_ERRCLASS			0x08000000

#define K_CANOPEN_WRONGOBJACCESS_ERRTYPE	0x00010000
#define K_CANOPEN_NOTEXISTOBJ_ERRTYPE		0x00020000
#define K_CANOPEN_INCONSISTENTPARAM_ERRTYPE	0x00030000
#define K_CANOPEN_ILLEGALPARAM_ERRTYPE		0x00040000
#define K_CANOPEN_TYPECONFLICT_ERRTYPE		0x00070000

#define K_CANOPEN_OBJLENHIGH_ADDCODE		0x00000012
#define K_CANOPEN_OBJLENLOW_ADDCODE			0x00000013
#define K_CANOPEN_INTERNALERR_ADDCODE		0x00000044

/**************************************/
/* SDO controller */

/* State of a CANopen SDO exchange */
typedef enum {
	E_SDO_IDLE = 0,
	E_SDO_UPLOAD_SENT = 1,
	E_SDO_UPLOAD_SEG0 = 2,
	E_SDO_UPLOAD_SEG1 = 3,
	E_SDO_DOWNLOAD_SENT = 4,
	E_SDO_DOWNLOAD_SEG0 = 5,
	E_SDO_DOWNLOAD_SEG1 = 6,
	E_SDO_ABORT_REQ = 7
} te_CanopenSdoState;

/* Context of a CANopen exchange */
typedef struct
{
	/* CANopen node SDO CAN identifiers */
	uint16 u16_canopen_tx_can_id;
	uint16 u16_canopen_rx_can_id;

	/* SDO communication state */
	te_CanopenSdoState	e_sdo_state;

	/* CANopen object data */
	uint16	u16_sdo_object_id;
	uint8	u8_sdo_object_subid;

	/* CANopen information error in case of ABORT */
	uint32	u32_sdo_errinfo;

	/* For response time-out checking: time since SDO request emission */
	uint8	u8_sdo_timeout_value;
	uint8	u8_sdo_timeout_ctr;
} ts_CanopenSdoController;

/* SDO transfer no error mask */
#define K_CANOPEN_SDO_ERRINFO_NO_ERROR		0

/* Definitions of received SDO frames types */
typedef enum {
	E_CANOPEN_SDO_UPLOAD_RESP = 0,
	E_CANOPEN_SDO_SEG_UPLOAD_0 = 1,
	E_CANOPEN_SDO_SEG_UPLOAD_1 = 2,
	E_CANOPEN_SDO_DOWNLOAD_RESP = 3,
	E_CANOPEN_SDO_ABORT = 4,
	E_CANOPEN_SDO_UNKNOWN_CMD = 5
} te_CanopenSdoFrameKind;


/* Variables 								      */
/*============================================================================*/ 
static Std_ReturnType gt_InitCanOpen_SdoManage_Status = E_NOT_OK; /* Global Initialization variable */
static Std_ReturnType gt_CfgCanOpen_SdoManage_Status = E_NOT_OK; /* Global Configuration variable */


/* Current states of the CANopen protocol exchanges on the CAN networks */
static ts_CanopenSdoController gas_canopen_sdo_ctrl[K_CANOPEN_NODES_NR];
static uint8 gu8_canopen_nodes_nr;


/* Private functions prototypes 					      */
/*============================================================================*/ 

static Std_ReturnType canopen_update_rcv_data_and_sdo_ctrl(ts_CanopenSdoController * const ps_canopen_sdo_ctrl, const ts_CanopenCanFrame * const ps_canopen_frame,
			ts_CanopenData * const ps_canopen_data, te_CanopenStatus * const pe_canopen_exchange_status, te_CanopenFrameProcessStatus * const pe_frame_processed);
static Std_ReturnType canopen_process_new_sdo_ctrl_state(const ts_CanopenSdoController * const ps_canopen_sdo_ctrl, te_CanopenStatus * const pe_canopen_exchange_status);
static void canopen_decode_sdo_frame(const ts_CanopenCanFrame * const ps_canopen_data, uint16 * const pu16_canopen_object_idx_or_tc, uint8 * const pu8_canopen_object_subidx,
				ts_CanopenData * const ps_canopen_object_data, te_CanopenSdoFrameKind * const pe_sdo_frame_kind);
static void canopen_read_int_data(const uint8 au8_canopen_data[K_CANOPEN_DATA_FRAME_LEN], uint16 * const pu16_canopen_object_idx, uint8 * const pu8_canopen_object_subidx,
				uint32 * const pu32_canopen_object_data);


/* Inline functions 							      */
/*============================================================================*/

/* Public functions 							      */
/*============================================================================*/


/**
 * \fn  Std_ReturnType ProCan_SdoManage_Initialize(void)
 * \brief CANopen library initialization
 *
 * \param  void
 *
 * \return Std_ReturnType :
 *         - E_OK if module is initialized
 *         - E_NOT_OK if not
 */
Std_ReturnType ProCan_SdoManage_Initialize(void)
{
    /*! Declaration */

    /*! Initialization */

    /*! Function Body */
	gu8_canopen_nodes_nr = 0;
	gt_InitCanOpen_SdoManage_Status = E_OK;

    /*! return code */
    return(gt_InitCanOpen_SdoManage_Status);
}

/**
 * \fn  Std_ReturnType ProCan_SdoManage_Configure(const ts_CanopenCfg * const ps_canopen_cfg)
 * \brief CANopen ports configuration
 *
 * \param <ts_CanopenCfg *ps_canopen_cfg> { CANopen library configuration }
 *
 * \return Std_ReturnType :
 *         - E_OK if module is initialized and configuration is successful
 *         - E_NOT_OK if not
 */
Std_ReturnType ProCan_SdoManage_Configure(const ts_CanopenCfg * const ps_canopen_cfg)
{
    /*! Declaration */
	uint8 u8_node_idx;

    /*! Initialization */
	gt_CfgCanOpen_SdoManage_Status = E_NOT_OK;

    /*! Function Body */
	if ((E_OK == gt_InitCanOpen_SdoManage_Status) && (NULL_PTR != ps_canopen_cfg))
	{
		if (ps_canopen_cfg->u8_canopen_nodes_nr_cfg <= K_CANOPEN_NODES_NR)
		{
			/* Write the new CANopen configuration */
			gu8_canopen_nodes_nr = ps_canopen_cfg->u8_canopen_nodes_nr_cfg;
			for (u8_node_idx = 0; u8_node_idx < gu8_canopen_nodes_nr; u8_node_idx++)
			{
				/* CANopen CAN identifiers creation */
				gas_canopen_sdo_ctrl[u8_node_idx].u16_canopen_tx_can_id = K_CANOPEN_SDO_CLIENT_CANID | ps_canopen_cfg->as_canopen_ports_cfg[u8_node_idx].u8_canopen_node_id_cfg;
				gas_canopen_sdo_ctrl[u8_node_idx].u16_canopen_rx_can_id = K_CANOPEN_SDO_SERVER_CANID | ps_canopen_cfg->as_canopen_ports_cfg[u8_node_idx].u8_canopen_node_id_cfg;

				/* Server response time-out */
				gas_canopen_sdo_ctrl[u8_node_idx].u8_sdo_timeout_value = ps_canopen_cfg->as_canopen_ports_cfg[u8_node_idx].u8_canopen_sdo_timeout_cfg;

				/* Initialize each SDO controller */
				gas_canopen_sdo_ctrl[u8_node_idx].e_sdo_state = E_SDO_IDLE;
			}

			gt_CfgCanOpen_SdoManage_Status = E_OK;
		} else {
			/* Invalid number of CANopen nodes */
		}
	} else {
		/* Module is not initialized, Halt */
		;
	}

    /*! return code */
    return(gt_CfgCanOpen_SdoManage_Status);
}


/**
 * \fn  Std_ReturnType ProCan_SdoManage_StartUpload(const uint8 u8_canopen_node_idx, const uint16 u16_canopen_object_idx, const uint8 u8_canopen_object_subidx, te_CanopenStatus * const pe_canopen_exchange_status)
 * \brief Start a CANopen SDO upload request
 *
 * \param  <uint8 u8_canopen_node_idx> {the node index on the CAN network on which the CANopen command must be launched (0..gu8_canopen_nodes_nr - 1)}
 * \param  <uint16 u16_canopen_object_idx> {the requested CANopen object index}
 * \param  <uint8 u8_canopen_object_subidx> {the requested CANopen object subindex}
 * \param  <te_CanopenStatus * pe_canopen_exchange_status> {status of the CANopen command:
 *  				   	- E_CANOPEN_TRANSFER_IN_PROGRESS: SDO transfer initiated successfully and is in progress
 *     					- E_CANOPEN_NETWORK_INTERFACE_ERROR: general error received from the underlying communication driver }
 *
 * \return Std_ReturnType :
 *         - E_OK if command is successful
 *         - E_NOT_OK if not
 */
Std_ReturnType ProCan_SdoManage_StartUpload(const uint8 u8_canopen_node_idx, const uint16 u16_canopen_object_idx, const uint8 u8_canopen_object_subidx, te_CanopenStatus * const pe_canopen_exchange_status)
{
    /*! Declaration */
    Std_ReturnType t_Start_SdoUpload_Status;
    /* Network command status */
    te_CanopenWrapperStatus e_can_status;
	/* Data exchanged with the underlying communication layer */
    ts_CanopenCanFrame s_canopen_frame;

    /*! Initialization */
    e_can_status= E_CANOPENWRP_NO_DATA;

    /*! Function Body */
    if ((E_OK == gt_CfgCanOpen_SdoManage_Status) && (NULL_PTR != pe_canopen_exchange_status)
    		&& (u8_canopen_node_idx < gu8_canopen_nodes_nr) && (E_SDO_IDLE == gas_canopen_sdo_ctrl[u8_canopen_node_idx].e_sdo_state))
    {

		/* Building of the CANopen COB-ID and frame for an SDO upload request */
    	s_canopen_frame.e_can_frame_type = E_CANOPEN_NORMAL_FRAME;

		/* COB-ID selection */
    	s_canopen_frame.u32_can_id = gas_canopen_sdo_ctrl[u8_canopen_node_idx].u16_canopen_tx_can_id;

    	/* For an SDO upload request: */
    	/* CANopen command byte = 010 00000 (ccs = 2: initiate upload response, others parameters not used) */
    	/* 8-byte CAN frame = |Command (1 byte)|Object idx (2 bytes)|Object subindex (1 byte)| 0 (4 bytes)| */
    	s_canopen_frame.au8_can_data[0] = K_CANOPEN_SDO_INIT_UPLOAD;
    	/* Object index in little endian format */
    	s_canopen_frame.au8_can_data[1] = (uint8)(u16_canopen_object_idx & 0xFF);
    	s_canopen_frame.au8_can_data[2] = (uint8)(u16_canopen_object_idx >> 8);
    	/* Object subindex */
    	s_canopen_frame.au8_can_data[3] = u8_canopen_object_subidx;
    	/* 4 bytes of 0 */
    	memset(&s_canopen_frame.au8_can_data[4], 0, K_CANOPEN_SDO_FRAME_DLC - 4);

    	s_canopen_frame.u8_can_dlc = K_CANOPEN_SDO_FRAME_DLC;

		/* Request sending */
		t_Start_SdoUpload_Status = ComWrapperCanOpen_Send(&s_canopen_frame, &e_can_status);

		if ((E_OK == t_Start_SdoUpload_Status) &&
			(E_CANOPENWRP_DATA_OK == e_can_status))
		{
			/* Request correctly sent: update the driver internal state */
			gas_canopen_sdo_ctrl[u8_canopen_node_idx].e_sdo_state = E_SDO_UPLOAD_SENT;
			gas_canopen_sdo_ctrl[u8_canopen_node_idx].u16_sdo_object_id = u16_canopen_object_idx;
			gas_canopen_sdo_ctrl[u8_canopen_node_idx].u8_sdo_object_subid = u8_canopen_object_subidx;
			gas_canopen_sdo_ctrl[u8_canopen_node_idx].u32_sdo_errinfo = K_CANOPEN_SDO_ERRINFO_NO_ERROR;
			gas_canopen_sdo_ctrl[u8_canopen_node_idx].u8_sdo_timeout_ctr = gas_canopen_sdo_ctrl[u8_canopen_node_idx].u8_sdo_timeout_value;

			*pe_canopen_exchange_status = E_CANOPEN_TRANSFER_IN_PROGRESS;
		}
		else
		{
			if (E_OK == t_Start_SdoUpload_Status)
			{
				/* Low level driver error */
				*pe_canopen_exchange_status = E_CANOPEN_NETWORK_INTERFACE_ERROR;
			}
			else
			{
				;/* Internal driver error */
			}
		}
    }
    else
    {
    	/* Module is not initialized, null pointer encountered, CANopen node index is out of range or wrong internal state for command process */
    	t_Start_SdoUpload_Status=E_NOT_OK;
    }

    /*! return code */
    return(t_Start_SdoUpload_Status);
}

/**
 * \fn  Std_ReturnType ProCan_SdoManage_StartDownload(const uint8 u8_canopen_node_idx, const uint16 u16_canopen_object_idx, const uint8 u8_canopen_object_subidx,
 *		ts_CanopenData * const ps_canopen_data, te_CanopenStatus * const pe_canopen_exchange_status)
 * \brief Start a CANopen SDO download request
 *
 * \param <uint8 u8_canopen_node_idx> {the node index on the CAN network on which the CANopen command must be launched (0..gu8_canopen_nodes_nr - 1)}
 * \param <uint16 u16_canopen_object_idx> {the requested CANopen object index}
 * \param <uint8 u8_canopen_object_subidx> {the requested CANopen object subindex}
 * \param <ts_CanopenData *ps_canopen_data> {the buffer providing the data to be sent from the driver interface caller:
 * 			 			  e_data_kind = E_CANOPEN_INTxx => u32_integer_val_str_len = data
 *     					  e_data_kind = E_CANOPEN_STR   => not supported}
 * \param <te_CanopenStatus *pe_canopen_exchange_status> {status of the CANopen command:
 *     				- E_CANOPEN_TRANSFER_IN_PROGRESS: SDO transfer initiated successfully and is in progress
 *     				- E_CANOPEN_NETWORK_INTERFACE_ERROR: general error received from the underlying communication driver }
 *
 * \return Std_ReturnType :
 *         - E_OK if command is successful
 *         - E_NOT_OK if not
 */
Std_ReturnType ProCan_SdoManage_StartDownload(const uint8 u8_canopen_node_idx, const uint16 u16_canopen_object_idx, const uint8 u8_canopen_object_subidx,
										ts_CanopenData * const ps_canopen_data, te_CanopenStatus * const pe_canopen_exchange_status)
{
    /*! Declaration */
    Std_ReturnType t_Start_SdoDownload_Status;
    /* Network command status */
    te_CanopenWrapperStatus e_can_status;
	/* Data exchanged with the underlying communication layer */
    ts_CanopenCanFrame s_canopen_frame;
	/* Data size in case of integer download request */
	uint8 u32_data_size;
	/* Data value of integer download request */
	uint32 u32_data_value;
	/* Index */
	uint8 u8_idx;

    /*! Initialization */
    t_Start_SdoDownload_Status = E_NOT_OK;
    e_can_status= E_CANOPENWRP_NO_DATA;

    /*! Function Body */
    if ((E_OK == gt_CfgCanOpen_SdoManage_Status) && (NULL_PTR != ps_canopen_data) && (NULL_PTR != pe_canopen_exchange_status)
    		&& (u8_canopen_node_idx < gu8_canopen_nodes_nr) && (E_SDO_IDLE == gas_canopen_sdo_ctrl[u8_canopen_node_idx].e_sdo_state))
    {
    	/* Building of the CANopen COB-ID and frame for an SDO upload request */
    	s_canopen_frame.e_can_frame_type = E_CANOPEN_NORMAL_FRAME;

    	/* COB-ID selection */
    	s_canopen_frame.u32_can_id = gas_canopen_sdo_ctrl[u8_canopen_node_idx].u16_canopen_tx_can_id;

		switch(ps_canopen_data->e_data_kind)
		{
		case E_CANOPEN_SDO_INT8:
			u32_data_size = 1;
	    	t_Start_SdoDownload_Status = E_OK;
			break;

		case E_CANOPEN_SDO_INT16:
			u32_data_size = 2;
	    	t_Start_SdoDownload_Status = E_OK;
			break;

		case E_CANOPEN_SDO_INT24:
			u32_data_size = 3;
	    	t_Start_SdoDownload_Status = E_OK;
			break;

		case E_CANOPEN_SDO_INT32:
			u32_data_size = 4;
	    	t_Start_SdoDownload_Status = E_OK;
			break;

		case E_CANOPEN_SDO_STR:
#ifdef K_DEBUG
			printf("canopen_send_sdo_download: string download request not supported\n");
#endif
			break;

		default:
#ifdef K_DEBUG
			printf("canopen_send_sdo_download: invalid data kind (%d)\n", ps_canopen_data->e_data_kind);
#endif
			break;
		}

		if (E_OK == t_Start_SdoDownload_Status)
		{
			/* For an expedited SDO download request: */
			/* CANopen command byte = 001 0cc11 (ccs = 1: initiate download request */
			/* 8-byte CAN frame = |Command (1 byte)|Object idx (2 bytes)|Object subindex (1 byte)| Data padded with 0 (4 bytes)| */
			s_canopen_frame.au8_can_data[0] = K_CANOPEN_SDO_INIT_DOWNLOAD | ((4 - u32_data_size) << 2) | 0x03;
			/* Object index in little endian format */
			s_canopen_frame.au8_can_data[1] = (uint8)(u16_canopen_object_idx & 0xFF);
			s_canopen_frame.au8_can_data[2] = (uint8)(u16_canopen_object_idx >> 8);
			/* Object subindex */
			s_canopen_frame.au8_can_data[3] = u8_canopen_object_subidx;
			/* Up to 4 bytes in little endian format, padded with 0 */
			u32_data_value = ps_canopen_data->u32_integer_val_str_len;
			memset(&s_canopen_frame.au8_can_data[4], 0, K_CANOPEN_SDO_FRAME_DLC - 4);
			for (u8_idx = 0; u8_idx < u32_data_size; u8_idx++)
			{
				s_canopen_frame.au8_can_data[4 + u8_idx] = (uint8)(u32_data_value & 0xFF);
				u32_data_value >>= 8;
			}

			s_canopen_frame.u8_can_dlc = K_CANOPEN_SDO_FRAME_DLC;

			/* Request sending */
			t_Start_SdoDownload_Status = ComWrapperCanOpen_Send(&s_canopen_frame, &e_can_status);

			if ((E_OK == t_Start_SdoDownload_Status) &&
				(E_CANOPENWRP_DATA_OK == e_can_status))
			{
				/* Request correctly sent: update the driver internal state */
				gas_canopen_sdo_ctrl[u8_canopen_node_idx].e_sdo_state = E_SDO_DOWNLOAD_SENT;
				gas_canopen_sdo_ctrl[u8_canopen_node_idx].u16_sdo_object_id = u16_canopen_object_idx;
				gas_canopen_sdo_ctrl[u8_canopen_node_idx].u8_sdo_object_subid = u8_canopen_object_subidx;
				gas_canopen_sdo_ctrl[u8_canopen_node_idx].u32_sdo_errinfo = K_CANOPEN_SDO_ERRINFO_NO_ERROR;
				gas_canopen_sdo_ctrl[u8_canopen_node_idx].u8_sdo_timeout_ctr = gas_canopen_sdo_ctrl[u8_canopen_node_idx].u8_sdo_timeout_value;

				*pe_canopen_exchange_status = E_CANOPEN_TRANSFER_IN_PROGRESS;
			}
			else
			{
				if (E_OK == t_Start_SdoDownload_Status)
				{
					/* Low level driver error */
					*pe_canopen_exchange_status = E_CANOPEN_NETWORK_INTERFACE_ERROR;
				}
				else
				{
					;/* Internal driver error */
				}
			}
		}
		else
		{
			;/* Internal driver error */
		}
	}
    else
    {
    	/* Module is not initialized, null pointer encountered, CANopen node index is out of range or wrong internal state for command process */
    	;
    }

    /*! return code */
    return(t_Start_SdoDownload_Status);
}

/**
 * \fn  Std_ReturnType ProCan_SdoManage_ProcessFrame(uint8 * const u8_canopen_node_idx, ts_CanopenData * const ps_canopen_data,
 * 				te_CanopenStatus * const pe_canopen_exchange_status, te_CanopenFrameProcessStatus * const pe_frame_processed)
 * \brief Process received CANopen frames
 *
 * \param <ts_CanopenCanFrame *ps_canopen_frame> {the CAN frame to decode}
 * \param <uint8 *u8_canopen_node_idx> {the node index on the CAN network from which transfer is complete or got a protocol error}
 * \param <ts_CanopenData *ps_canopen_data> {the buffer where to store the received data:
 *           when return code is E_CANOPEN_TRANSFER_IN_PROGRESS: contains a string segment along with its length
 *           when return code is E_CANOPEN_TRANSFER_COMPLETE: contains the received integer or the final string segment along with its length
 *           when return code is E_CANOPEN_TRANSFER_ERROR: contains the error information}
 * \param <te_CanopenStatus *pe_canopen_exchange_status> {status of the CANopen command:
 *     				- E_CANOPEN_TRANSFER_IN_PROGRESS: SDO transfer still in progress
 *	   				- E_CANOPEN_TRANSFER_COMPLETE: SDO transfer successful, for upload, data is ready in the buffer provided on ProCan_Start_SdoUpload call
 *     				- E_CANOPEN_TRANSFER_ERROR: protocol error during SDO transfer
 *     				- E_CANOPEN_NETWORK_INTERFACE_ERROR: general error received from the underlying communication driver}
 * \param <te_CanopenFrameProcessStatus *pe_frame_processed> {the status of the CANopen frame processing}
 *
 * \return Std_ReturnType :
 *         - E_OK if command is successful
 *         - E_NOT_OK if not
 */
Std_ReturnType ProCan_SdoManage_ProcessFrame(const ts_CanopenCanFrame * const ps_canopen_can_frame, uint8 * const u8_canopen_node_idx,
		ts_CanopenData * const ps_canopen_data, te_CanopenStatus * const pe_canopen_exchange_status, te_CanopenFrameProcessStatus * const pe_frame_processed)
{
    /*! Declaration */
    Std_ReturnType t_Process_Frames_Status;
	/* Node index of the received frame */
	uint8 u8_node_idx;

    /*! Initialization */
	t_Process_Frames_Status = E_NOT_OK;

    /*! Function Body */
    if ((E_OK == gt_CfgCanOpen_SdoManage_Status) && (NULL_PTR != ps_canopen_data) && (NULL_PTR != u8_canopen_node_idx) && (NULL_PTR != pe_canopen_exchange_status))
    {
		/* Check if the CAN identifier is an SDO server-to-client exchange */
		/* COB-ID = 11-bit CAN identifier = 101 1|server identifier */
		if ((E_CANOPEN_NORMAL_FRAME == ps_canopen_can_frame->e_can_frame_type)
				&& (K_CANOPEN_SDO_SERVER_CANID == (ps_canopen_can_frame->u32_can_id & K_CANOPEN_FUNC_CODE_FROM_CANID)) && (K_CANOPEN_SDO_FRAME_DLC == ps_canopen_can_frame->u8_can_dlc))
		{
			/* Retrieve the received CAN identifier within the configured CANopen RX CAN identifiers */
			for (u8_node_idx = 0; (u8_node_idx < gu8_canopen_nodes_nr) && (gas_canopen_sdo_ctrl[u8_node_idx].u16_canopen_rx_can_id != ps_canopen_can_frame->u32_can_id); u8_node_idx++)
			{
				/* Check the next node index */
			}
			if (u8_node_idx < gu8_canopen_nodes_nr)
			{
				/* Server found: process the received frame */
				*u8_canopen_node_idx = u8_node_idx;

				/* Process SDO frame, then new SDO controller state */
				t_Process_Frames_Status = canopen_update_rcv_data_and_sdo_ctrl(&gas_canopen_sdo_ctrl[u8_node_idx], ps_canopen_can_frame, ps_canopen_data, pe_canopen_exchange_status, pe_frame_processed);
			} else {
				/* Frame is from a unmanaged server node: ignore it */

				/* SDO frame processed without data */
				*pe_frame_processed = E_CANOPEN_FRAME_PROCESSED_NO_DATA;
				t_Process_Frames_Status = E_OK;
			}
		} else {
			/* Not an SDO server-to-client frame */
			/* No data processed */
			*pe_frame_processed = E_CANOPEN_FRAME_NOT_PROCESSED;
			t_Process_Frames_Status = E_OK;
		}
    }
    else
    {
	/* Module is not initialized or null pointer encountered */
	;
    }

    /*! return code */
    return(t_Process_Frames_Status);
}


/**
 * \fn  Std_ReturnType ProCan_SdoManage_CheckTimeout(const uint8 u8_canopen_node_idx, te_CanopenStatus * const pe_canopen_exchange_status)
 * \brief Check server response time-out
 *
 * \param <uint8 u8_canopen_node_idx> {the node index on the CAN network on which the CANopen command must be launched (0..gu8_canopen_nodes_nr - 1)}
 * \param <te_CanopenStatus *pe_canopen_exchange_status> {status of the CANopen command:
 *	   				- E_CANOPEN_TRANSFER_IN_PROGRESS: SDO transfer is on-going
 *	   				- E_CANOPEN_TRANSFER_COMPLETE: SDO transfer complete or no SDO transfer is on-going
 *	   				- E_CANOPEN_TIMEOUT_ERROR: server response time-out occurred}
 *
 * \return Std_ReturnType :
 *         - E_OK if command is successful
 *         - E_NOT_OK if not
 */
Std_ReturnType ProCan_SdoManage_CheckTimeout(const uint8 u8_canopen_node_idx, te_CanopenStatus * const pe_canopen_exchange_status)
{
    /*! Declaration */
    Std_ReturnType t_Check_SdoTimeout_Status;

    /*! Initialization */
	t_Check_SdoTimeout_Status = E_NOT_OK;

    /*! Function Body */
    if ((E_OK == gt_CfgCanOpen_SdoManage_Status) && (NULL_PTR != pe_canopen_exchange_status) && (u8_canopen_node_idx < gu8_canopen_nodes_nr))
    {
		if (E_SDO_IDLE != gas_canopen_sdo_ctrl[u8_canopen_node_idx].e_sdo_state)
		{
			/* SDO transfer is on-going: check the timer */
			/* Update the time-out timer */
			gas_canopen_sdo_ctrl[u8_canopen_node_idx].u8_sdo_timeout_ctr--;
			if (gas_canopen_sdo_ctrl[u8_canopen_node_idx].u8_sdo_timeout_ctr == 0)
			{
				/* Time-out occurred: abort the transfer */
				gas_canopen_sdo_ctrl[u8_canopen_node_idx].e_sdo_state = E_SDO_IDLE;
				*pe_canopen_exchange_status = E_CANOPEN_TIMEOUT_ERROR;
			} else {
				/* No time-out, transfer in progress */
				*pe_canopen_exchange_status = E_CANOPEN_TRANSFER_IN_PROGRESS;
			}
		} else {
			/* Transfer complete or no on-going transfer */
			*pe_canopen_exchange_status = E_CANOPEN_TRANSFER_COMPLETE;
		}

		t_Check_SdoTimeout_Status = E_OK;
    }
    else
    {
    	/* Module is not initialized, null pointer encountered or CANopen node index is out of range */
	;
    }

    /*! return code */
    return(t_Check_SdoTimeout_Status);
}

/**
 * \fn  Std_ReturnType ProCan_SdoManage_AbortTransfer(const uint8 u8_canopen_node_idx, const canopen_sdo_abort_reason_type canopen_errinfo_abort_t)
 * \brief Abort the current SDO transfer
 *
 * \param <uint8 u8_canopen_node_idx> {the node index on the CAN network on which the CANopen command must be launched (0..gu8_canopen_nodes_nr - 1)}
 * \param <canopen_sdo_abort_reason_type e_canopen_errinfo_abort> {the error information about the abort reason}
 *
 * \return Std_ReturnType :
 *         - E_OK if command is successful
 *         - E_NOT_OK if not
 */
Std_ReturnType ProCan_SdoManage_AbortTransfer(const uint8 u8_canopen_node_idx, const te_CanopenSdoAbortReason e_canopen_errinfo_abort)
{
    /*! Declaration */
    Std_ReturnType t_Abort_SdoTransfer_Status;

    /*! Initialization */
    t_Abort_SdoTransfer_Status = E_NOT_OK;

    /*! Function Body */
    if ((E_OK == gt_CfgCanOpen_SdoManage_Status) && (u8_canopen_node_idx < gu8_canopen_nodes_nr))
    {
		t_Abort_SdoTransfer_Status = E_OK;

		if ((E_SDO_IDLE != gas_canopen_sdo_ctrl[u8_canopen_node_idx].e_sdo_state) && (E_SDO_ABORT_REQ != gas_canopen_sdo_ctrl[u8_canopen_node_idx].e_sdo_state))
		{
			/* Process the abort request */
			switch(e_canopen_errinfo_abort)
			{
			case E_CANOPEN_OBJ_LENGTH_TOO_HIGH:
				gas_canopen_sdo_ctrl[u8_canopen_node_idx].u32_sdo_errinfo = K_CANOPEN_ACCESS_ERRCLASS | K_CANOPEN_TYPECONFLICT_ERRTYPE | K_CANOPEN_OBJLENHIGH_ADDCODE;
				gas_canopen_sdo_ctrl[u8_canopen_node_idx].e_sdo_state = E_SDO_ABORT_REQ;
				break;

			case E_CANOPEN_OBJ_LENGTH_TOO_LOW:
				gas_canopen_sdo_ctrl[u8_canopen_node_idx].u32_sdo_errinfo = K_CANOPEN_ACCESS_ERRCLASS | K_CANOPEN_TYPECONFLICT_ERRTYPE | K_CANOPEN_OBJLENLOW_ADDCODE;
				gas_canopen_sdo_ctrl[u8_canopen_node_idx].e_sdo_state = E_SDO_ABORT_REQ;
				break;

			case E_CANOPEN_WRONG_SEQUENCE:
				gas_canopen_sdo_ctrl[u8_canopen_node_idx].u32_sdo_errinfo = K_CANOPEN_SERVICE_ERRCLASS | K_CANOPEN_ILLEGALPARAM_ERRTYPE;
				gas_canopen_sdo_ctrl[u8_canopen_node_idx].e_sdo_state = E_SDO_ABORT_REQ;
				break;

			case E_CANOPEN_INTERNAL_ERROR:
				gas_canopen_sdo_ctrl[u8_canopen_node_idx].u32_sdo_errinfo = K_CANOPEN_ACCESS_ERRCLASS | K_CANOPEN_ILLEGALPARAM_ERRTYPE | K_CANOPEN_INTERNALERR_ADDCODE;
				gas_canopen_sdo_ctrl[u8_canopen_node_idx].e_sdo_state = E_SDO_ABORT_REQ;
				break;

			default:
#ifdef K_DEBUG
				printf("ProCan_Abort_SdoTransfer: invalid abort reason (%d)\n", e_canopen_errinfo_abort);
#endif

				t_Abort_SdoTransfer_Status = E_NOT_OK;
				break;
			}
		}
	}
    else
    {
	/* Module is not initialized or CANopen node index is out of range */
	;
    }

    /*! return code */
    return(t_Abort_SdoTransfer_Status);
}


/* Private functions 							      */
/*============================================================================*/


/* Updates the CANopen SDO controller and the data sent back to the CANopen caller according to the received SDO server response
 * Inputs:
 *     - ps_canopen_frame: the received frame
 *
 * Inputs/Outputs:
 *     - ps_canopen_sdo_ctrl: the CANopen SDO controller
 *
 * Outputs:
 *     - ps_canopen_data: the buffer where to store the received data:
 *           when return code is E_CANOPEN_TRANSFER_IN_PROGRESS: contains a string segment along with its length
 *           when return code is E_CANOPEN_TRANSFER_COMPLETE: contains the received integer or the final string segment along with its length
 *           when return code is E_CANOPEN_TRANSFER_ERROR: contains the error information
 *     - pe_canopen_exchange_status: status of the CANopen command
 *     		E_CANOPEN_TRANSFER_IN_PROGRESS: SDO transfer still in progress
 *	   		E_CANOPEN_TRANSFER_COMPLETE: SDO transfer successful, for upload, data is ready in the buffer provided on canopen_send_sdo_upload call
 *     		E_CANOPEN_TRANSFER_ERROR: protocol error during SDO transfer
 *     		E_CANOPEN_SDO_ABORT_COMPLETE: CANopen SDO abort sent to the server
 *     		E_CANOPEN_NETWORK_INTERFACE_ERROR: general error received from the underlying communication driver
 *     - pe_frame_processed: the status of the CANopen frame processing
 *
 * Return code :
 *    - E_OK if command is successful
 *    - E_NOT_OK if not
 *
 */
static Std_ReturnType canopen_update_rcv_data_and_sdo_ctrl(ts_CanopenSdoController * const ps_canopen_sdo_ctrl, const ts_CanopenCanFrame * const ps_canopen_frame,
			ts_CanopenData * const ps_canopen_data, te_CanopenStatus * const pe_canopen_exchange_status, te_CanopenFrameProcessStatus * const pe_frame_processed)
{
    /*! Declaration */
	Std_ReturnType t_returned_status = E_OK;
	/* Data received */
	te_CanopenSdoFrameKind e_received_frame_kind;
	uint16 u16_object_idx_or_tc;
	uint8 u8_object_subidx;
	ts_CanopenData s_can_data;

    /*! Initialization */
	/* Next state */
	te_CanopenSdoState e_next_state = ps_canopen_sdo_ctrl->e_sdo_state;
	e_received_frame_kind=E_CANOPEN_SDO_UNKNOWN_CMD;

	/* Decode the received CANopen frame */
	canopen_decode_sdo_frame(ps_canopen_frame, &u16_object_idx_or_tc, &u8_object_subidx, &s_can_data, &e_received_frame_kind);
	switch(e_received_frame_kind)
	{
		case E_CANOPEN_SDO_UPLOAD_RESP:
			/* Check if frame is expected: CANopen node exchange state and received object index and subindex must be correct */
			if (E_SDO_UPLOAD_SENT != ps_canopen_sdo_ctrl->e_sdo_state)
			{
				/* Invalid state: log a warning */
	#ifdef K_DEBUG
				printf("canopen_update_data_and_state: warning: upload frame received in an unexpected CANopen driver state (%d)\n", ps_canopen_sdo_ctrl->e_sdo_state);
	#endif
				/* Ignore the frame: no data received */
				*pe_frame_processed = E_CANOPEN_FRAME_PROCESSED_NO_DATA;
			} else if ((u16_object_idx_or_tc == ps_canopen_sdo_ctrl->u16_sdo_object_id) && (u8_object_subidx == ps_canopen_sdo_ctrl->u8_sdo_object_subid))
			{
				/* The received object id and subid are equal to the requested ones */
				if (E_CANOPEN_SDO_STR != s_can_data.e_data_kind)
				{
					/* Object data is an integer: copy it to the final data buffer */
					ps_canopen_data->e_data_kind = E_CANOPEN_SDO_INT32;
					ps_canopen_data->u32_integer_val_str_len = s_can_data.u32_integer_val_str_len;
					/* Transfer is complete */
					e_next_state = E_SDO_IDLE;
					*pe_canopen_exchange_status = E_CANOPEN_TRANSFER_COMPLETE;
					*pe_frame_processed = E_CANOPEN_FRAME_PROCESSED_WITH_DATA;
				} else {
					/* Object data is a segmented string */
					/* data_t.integer_val_str_len_u32 is the length of the full segmented data */
					/* Report the full string length to the caller */
					ps_canopen_data->e_data_kind = E_CANOPEN_SDO_INT32;
					ps_canopen_data->u32_integer_val_str_len = s_can_data.u32_integer_val_str_len;

					/* Next state is segment transfer with toggle bit set to 0 */
					e_next_state = E_SDO_UPLOAD_SEG0;
					*pe_canopen_exchange_status = E_CANOPEN_TRANSFER_IN_PROGRESS;
					*pe_frame_processed = E_CANOPEN_FRAME_PROCESSED_WITH_DATA;
				}
			} else {
				/* The received object id and subid are not equal to the requested ones: log a warning */
	#ifdef K_DEBUG
				printf("canopen_update_data_and_state: warning: object index or subindex in the received frame do not match the requested ones (expected: %x.%x, received: %x.%x)\n",
						(int)ps_canopen_sdo_ctrl->u16_sdo_object_id, (int)ps_canopen_sdo_ctrl->u8_sdo_object_subid, (int)u16_object_idx_or_tc, (int)u8_object_subidx);
	#endif
				/* Ignore the frame: no data received */
				*pe_frame_processed = E_CANOPEN_FRAME_PROCESSED_NO_DATA;
			}
			break;

		case E_CANOPEN_SDO_SEG_UPLOAD_0:
		case E_CANOPEN_SDO_SEG_UPLOAD_1:
			/* Check if frame is expected: CANopen node exchange state must be correct */
			if ((E_SDO_UPLOAD_SEG0 != ps_canopen_sdo_ctrl->e_sdo_state) && (E_SDO_UPLOAD_SEG1 != ps_canopen_sdo_ctrl->e_sdo_state))
			{
				/* Invalid state: log a warning */
	#ifdef K_DEBUG
				printf("canopen_update_data_and_state: warning: segment frame received in an unexpected CANopen driver state (%d)\n", ps_canopen_sdo_ctrl->e_sdo_state);
	#endif
				/* Ignore the frame: no data received */
				*pe_frame_processed = E_CANOPEN_FRAME_PROCESSED_NO_DATA;
			}
			else
			/* Check the toggle bit */
			if (((E_SDO_UPLOAD_SEG0 == ps_canopen_sdo_ctrl->e_sdo_state) && (E_CANOPEN_SDO_SEG_UPLOAD_0 != e_received_frame_kind))
			||  ((E_SDO_UPLOAD_SEG1 == ps_canopen_sdo_ctrl->e_sdo_state) && (E_CANOPEN_SDO_SEG_UPLOAD_1 != e_received_frame_kind)))
			{
				/* Unexpected kind of frame: log an error */
	#ifdef K_DEBUG
				printf("canopen_update_data_and_state: invalid toggle bit\n");
	#endif
				/* Report the error to the server and abort the transfer */
				ps_canopen_data->e_data_kind = E_CANOPEN_SDO_INT32;
				ps_canopen_data->u32_integer_val_str_len = K_CANOPEN_SERVICE_ERRCLASS | K_CANOPEN_INCONSISTENTPARAM_ERRTYPE;
				ps_canopen_sdo_ctrl->u32_sdo_errinfo = ps_canopen_data->u32_integer_val_str_len;
				*pe_canopen_exchange_status = E_CANOPEN_TRANSFER_ERROR;
				*pe_frame_processed = E_CANOPEN_FRAME_PROCESSED_WITH_DATA;
			} else
			/* Only a string must be received in that state */
			if (E_CANOPEN_SDO_STR != s_can_data.e_data_kind)
			{
				/* Unexpected kind of data: log a driver error */

	#ifdef K_DEBUG
				printf("canopen_update_data_and_state: CANopen frame kind is segmented string upload, expected a string data, got a integer one\n");
	#endif
				/* Internal driver error */
				t_returned_status = E_NOT_OK;
			} else {
				/* Copy the received string to the destination buffer along with its length */
				ps_canopen_data->e_data_kind = E_CANOPEN_SDO_STR;
				ps_canopen_data->u32_integer_val_str_len = s_can_data.u32_integer_val_str_len;
				memcpy(ps_canopen_data->au8_string_val, s_can_data.au8_string_val, s_can_data.u32_integer_val_str_len);

				/* Check if final segment has been received */
				/* u16_object_idx_or_tc = 0 => transfer not complete, u16_object_idx_or_tc = 1 => transfer complete */
				if (K_CANOPEN_SDO_SEG_TRANSFER_COMPLETE == u16_object_idx_or_tc)
				{
					e_next_state = E_SDO_IDLE;
					*pe_canopen_exchange_status = E_CANOPEN_TRANSFER_COMPLETE;
				} else {
					/* Request the next segment */
					if (E_SDO_UPLOAD_SEG0 == ps_canopen_sdo_ctrl->e_sdo_state)
					{
						/* With a toggle bit set to 1 */
						e_next_state = E_SDO_UPLOAD_SEG1;
					} else {
						/* With a toggle bit set to 0 */
						e_next_state = E_SDO_UPLOAD_SEG0;
					}
					*pe_canopen_exchange_status = E_CANOPEN_TRANSFER_IN_PROGRESS;
				}
				*pe_frame_processed = E_CANOPEN_FRAME_PROCESSED_WITH_DATA;
			}
			break;

		case E_CANOPEN_SDO_DOWNLOAD_RESP:
			/* Check if frame is expected: CANopen node exchange state and received object index and subindex must be correct */
			if (E_SDO_DOWNLOAD_SENT != ps_canopen_sdo_ctrl->e_sdo_state)
			{
				/* Invalid state: log a warning */
	#ifdef K_DEBUG
				printf("canopen_update_data_and_state: warning: download frame received in an unexpected CANopen driver state (%d)\n", ps_canopen_sdo_ctrl->e_sdo_state);
	#endif
				/* Ignore the frame: no data received */
				*pe_frame_processed = E_CANOPEN_FRAME_PROCESSED_NO_DATA;
			} else if ((u16_object_idx_or_tc == ps_canopen_sdo_ctrl->u16_sdo_object_id) &&  (u8_object_subidx == ps_canopen_sdo_ctrl->u8_sdo_object_subid))
			{
				/* The received object id and subid are equal to the requested ones: download is complete */
				/* No data received */
				ps_canopen_data->e_data_kind = E_CANOPEN_NODATA;
				e_next_state = E_SDO_IDLE;
				*pe_canopen_exchange_status = E_CANOPEN_TRANSFER_COMPLETE;
				*pe_frame_processed = E_CANOPEN_FRAME_PROCESSED_WITH_DATA;
			} else {
				/* The received object id and subid are not equal to the requested ones: log a warning */

	#ifdef K_DEBUG
				printf("canopen_update_data_and_state: warning: object index or subindex in the received frame do not match the requested ones (expected: %x.%x, received: %x.%x)\n",
						(int)ps_canopen_sdo_ctrl->u16_sdo_object_id, (int)ps_canopen_sdo_ctrl->u8_sdo_object_subid, (int)u16_object_idx_or_tc, (int)u8_object_subidx);
	#endif
				/* Ignore the frame: no data received */
				*pe_frame_processed = E_CANOPEN_FRAME_PROCESSED_NO_DATA;
			}
			break;

		case E_CANOPEN_SDO_ABORT:
			/* Abort request from the server: log an error */

	#ifdef K_DEBUG
			printf("canopen_update_data_and_state: abort request received from server (additional information: %x)\n", (int)s_can_data.u32_integer_val_str_len);
	#endif
			/* Report the error and abort the transfer */
			ps_canopen_data->e_data_kind = E_CANOPEN_SDO_INT32;
			ps_canopen_data->u32_integer_val_str_len = s_can_data.u32_integer_val_str_len;
			e_next_state = E_SDO_IDLE;
			*pe_canopen_exchange_status = E_CANOPEN_TRANSFER_ERROR;
			*pe_frame_processed = E_CANOPEN_FRAME_PROCESSED_WITH_DATA;
			break;

		case E_CANOPEN_SDO_UNKNOWN_CMD:
		default:
			/* Unmanaged SDO command */
			/* Ignore the frame: no data received */
			*pe_frame_processed = E_CANOPEN_FRAME_PROCESSED_NO_DATA;
			break;

//		default:
//			/* Unknown type of frame: log an error */
//	#ifdef K_DEBUG
//			printf("canopen_update_data_and_state: unknown frame type (%d)\n", e_received_frame_kind);
//	#endif
//			/* Internal driver error */
//			t_returned_status = E_NOT_OK;
//			break;
	}

	if (E_OK == t_returned_status)
	{
		/* Update state */
		ps_canopen_sdo_ctrl->e_sdo_state = e_next_state;

		/* Process the new CANopen state */
		if ((E_CANOPEN_TRANSFER_IN_PROGRESS == *pe_canopen_exchange_status) || (K_CANOPEN_SDO_ERRINFO_NO_ERROR != ps_canopen_sdo_ctrl->u32_sdo_errinfo))
		{
			t_returned_status = canopen_process_new_sdo_ctrl_state(ps_canopen_sdo_ctrl, pe_canopen_exchange_status);
			if ((E_OK == t_returned_status) && (E_CANOPEN_TRANSFER_IN_PROGRESS != *pe_canopen_exchange_status))
			{
				/* Abort request sent or network error: return to idle */
				ps_canopen_sdo_ctrl->e_sdo_state = E_SDO_IDLE;
			} else if (E_OK == t_returned_status)
			{
				/* New frame sent to the server: update the time-out timer */
				ps_canopen_sdo_ctrl->u8_sdo_timeout_ctr = ps_canopen_sdo_ctrl->u8_sdo_timeout_value;
			} else {
				/* Internal driver error */
			}
		}
	} else {
		/* Internal driver error */
	}

	return t_returned_status;
}


/**
 * Decode a CANopen SDO frame
 *
 * Inputs:
 *     - ps_canopen_data: the received CANopen SDO frame
 *
 * Outputs:
 *     - pu16_canopen_object_idx_or_tc: the object index read from the frame (case E_CANOPEN_SDO_UPLOAD_RESP only)
 *                                      0 for segment transfer not complete, 1 otherwise (case E_CANOPEN_SDO_SEG_TOGGLEx only)
 *     - pu8_canopen_object_subidx: the object subindex read from the frame (case E_CANOPEN_SDO_UPLOAD_RESP only)
 *     - ps_canopen_object_data: the integer or string data read from the frame
 *     - pe_sdo_frame_kind: the received SDO frame type
 *
 * return void
 */
static void canopen_decode_sdo_frame(const ts_CanopenCanFrame * const ps_canopen_data, uint16 * const pu16_canopen_object_idx_or_tc, uint8 * const pu8_canopen_object_subidx,
				ts_CanopenData * const ps_canopen_object_data, te_CanopenSdoFrameKind * const pe_sdo_frame_kind)
{
    /*! Declaration */
	/* SDO command byte */
	uint8 u8_cmd_byte;
	/* Dummy data value (used for frame not holding data) */
	uint32 u32_dummy_value;

    /*! Initialization */
   	u8_cmd_byte = ps_canopen_data->au8_can_data[0];

    /*! Function Body */
   	/* command byte = 0100 cce1 => upload frame */
   	if ((u8_cmd_byte & 0xF1) == (K_CANOPEN_SDO_INIT_UPLOAD | 0x01))
    	{
		/* Upload request response */
		/* e = 0: segmented transfer : received data is the length of the object data */
		/* e = 1: not segmented transfer : received data is the object data value */
		canopen_read_int_data(ps_canopen_data->au8_can_data, pu16_canopen_object_idx_or_tc, pu8_canopen_object_subidx, &ps_canopen_object_data->u32_integer_val_str_len);
		if ((u8_cmd_byte & 0x02) == 0)
		{
			/* Segmented string transfer: next state must be the segment request with toggle bit set to 0 */
			ps_canopen_object_data->e_data_kind = E_CANOPEN_SDO_STR;
		} else {
			/* Normal integer transfer: transfer complete, next state must be idle */
			ps_canopen_object_data->e_data_kind = E_CANOPEN_SDO_INT32;
		}
		*pe_sdo_frame_kind = E_CANOPEN_SDO_UPLOAD_RESP;
	}
	/* command byte = 100 00000 => abort frame */
	else if (K_CANOPEN_SDO_ABORT_REQUEST == u8_cmd_byte)
	{
		/* Abort request: data is error codes */
		canopen_read_int_data(ps_canopen_data->au8_can_data, pu16_canopen_object_idx_or_tc, pu8_canopen_object_subidx, &ps_canopen_object_data->u32_integer_val_str_len);

		*pe_sdo_frame_kind = E_CANOPEN_SDO_ABORT;
	}
	/* command byte = 0110 0000 => download frame */
	else if (K_CANOPEN_SDO_SEG_UPLOAD == u8_cmd_byte)
	{
		/* Download request response: no data */
		canopen_read_int_data(ps_canopen_data->au8_can_data, pu16_canopen_object_idx_or_tc, pu8_canopen_object_subidx, &u32_dummy_value);

		*pe_sdo_frame_kind = E_CANOPEN_SDO_DOWNLOAD_RESP;
	}
	/* command byte = 000 b ccce => segment download frame */
	else if ((u8_cmd_byte & 0xE0) == K_CANOPEN_SDO_SEG_DOWNLOAD)
	{
		/* Read the string data present in the frame */
		ps_canopen_object_data->e_data_kind = E_CANOPEN_SDO_STR;
		/* Format of the CANopen frame */
		/* 8-byte CAN frame = |Command (1 byte)|String data padded with 0 (7 bytes)| */
		/* Length of the integer data is given by the command byte = xxxx cccx: ccc = number of data bytes that do NOT contain data */
		ps_canopen_object_data->u32_integer_val_str_len = K_CANOPEN_SDO_FRAME_DLC - 1 - ((u8_cmd_byte >> 1) & 7);
		memcpy(ps_canopen_object_data->au8_string_val, &ps_canopen_data->au8_can_data[1], ps_canopen_object_data->u32_integer_val_str_len);

		/* e = 0: segmented transfer not complete */
		/* e = 1: segmented transfer complete */
		if ((u8_cmd_byte & 1) == 0)
		{
			/* Segmented transfer not complete */
			*pu16_canopen_object_idx_or_tc = K_CANOPEN_SDO_SEG_TRANSFER_NOT_COMPLETE;
		} else {
			/* Segmented transfer complete */
			*pu16_canopen_object_idx_or_tc = K_CANOPEN_SDO_SEG_TRANSFER_COMPLETE;
		}
		/* b = segment toggle bit */
		if ((u8_cmd_byte & 0x10) == 0)
		{
			/* Toggle bit set to 0 */
			*pe_sdo_frame_kind = E_CANOPEN_SDO_SEG_UPLOAD_0;
		} else {
			/* Toggle bit set to 1 */
			*pe_sdo_frame_kind = E_CANOPEN_SDO_SEG_UPLOAD_1;
		}
	} else {
		/* Unknown command byte */
#ifdef K_DEBUG
		printf("privateCanOpen_Decode_Sdo_Response: invalid command byte (0x%x)\n", u8_cmd_byte);
#endif
		*pe_sdo_frame_kind = E_CANOPEN_SDO_UNKNOWN_CMD;
	}

    /*! return code */
}

/* Process a new CANopen SDO controller state: send abort to the server or request the next data segment from the server
 * Inputs:
 *     - ps_canopen_sdo_ctrl: the CANopen SDO controller
 *     - pe_canopen_exchange_status: status of the CANopen command
 *     		E_CANOPEN_TRANSFER_IN_PROGRESS: CANopen SDO state actions complete (transfer still in progress)
 *     		E_CANOPEN_SDO_ABORT_COMPLETE: CANopen SDO abort sent to the server
 *     		E_CANOPEN_NETWORK_INTERFACE_ERROR: general error received from the underlying communication driver
 *
  * Return code :
 *    - E_OK if command is successful
 *    - E_NOT_OK if not
 */
static Std_ReturnType canopen_process_new_sdo_ctrl_state(const ts_CanopenSdoController * const ps_canopen_sdo_ctrl, te_CanopenStatus * const pe_canopen_exchange_status)
{
	/*! Declaration */
	Std_ReturnType t_returned_status;
    /* Network command status */
    te_CanopenWrapperStatus e_can_status;
	/* Segment request frame */
    ts_CanopenCanFrame s_canopen_frame;

    /*! Initialization */
    e_can_status=E_CANOPENWRP_NO_DATA;

	if (K_CANOPEN_SDO_ERRINFO_NO_ERROR != ps_canopen_sdo_ctrl->u32_sdo_errinfo)
	{
		/* Abort request must be sent */
		/* Building of the CANopen COB-ID and frame for an SDO abort request */
		s_canopen_frame.e_can_frame_type = E_CANOPEN_NORMAL_FRAME;

    	/* COB-ID selection */
    	s_canopen_frame.u32_can_id = ps_canopen_sdo_ctrl->u16_canopen_tx_can_id;

    	/* For an SDO abort request: */
    	/* CANopen command byte = 100 00000 (ccs = 4: abort domain transfer, others parameters not used) */
    	/* 8-byte CAN frame = |Command (1 byte)|Object idx (2 bytes)|Object subindex (1 byte)| Additional code (2 bytes)|Error code (1 byte)|Error class (1 byte) */
		s_canopen_frame.au8_can_data[0] = K_CANOPEN_SDO_ABORT_REQUEST;
    	/* Object index in little endian format */
		s_canopen_frame.au8_can_data[1] = (uint8)(ps_canopen_sdo_ctrl->u16_sdo_object_id & 0xFF);
		s_canopen_frame.au8_can_data[2] = (uint8)(ps_canopen_sdo_ctrl->u16_sdo_object_id >> 8);
    	/* Object subindex */
		s_canopen_frame.au8_can_data[3] = ps_canopen_sdo_ctrl->u16_sdo_object_id;
    	/* Error class, Error code, Additional code in little endian format */
		s_canopen_frame.au8_can_data[4] = (uint8)(ps_canopen_sdo_ctrl->u32_sdo_errinfo & 0xFF);
		s_canopen_frame.au8_can_data[5] = (uint8)((ps_canopen_sdo_ctrl->u32_sdo_errinfo >> 8) & 0xFF);
		s_canopen_frame.au8_can_data[6] = (uint8)((ps_canopen_sdo_ctrl->u32_sdo_errinfo >> 16) & 0xFF);
		s_canopen_frame.au8_can_data[7] = (uint8)((ps_canopen_sdo_ctrl->u32_sdo_errinfo >> 24) & 0xFF);

		s_canopen_frame.u8_can_dlc = K_CANOPEN_SDO_FRAME_DLC;

		/* Request sending */
		t_returned_status = ComWrapperCanOpen_Send(&s_canopen_frame, &e_can_status);

		if ((E_OK == t_returned_status) &&
			(E_CANOPENWRP_DATA_OK != e_can_status))
		{
			/* Low level driver error */
			*pe_canopen_exchange_status = E_CANOPEN_NETWORK_INTERFACE_ERROR;
		}
		else
		{
			if (E_OK == t_returned_status)
			{
				/* Abort request sent */
				*pe_canopen_exchange_status = E_CANOPEN_SDO_ABORT_COMPLETE;
			}
			else
			{
				;/* Internal driver error */
			}
		}
	}
	else
	{
		switch(ps_canopen_sdo_ctrl->e_sdo_state)
		{
		case E_SDO_UPLOAD_SEG0:
		case E_SDO_UPLOAD_SEG1:
			/* Segment request must be sent */
			/* Building of the CANopen COB-ID and frame for and SDO upload segment request */
			s_canopen_frame.e_can_frame_type = E_CANOPEN_NORMAL_FRAME;

	    	/* COB-ID selection */
	    	s_canopen_frame.u32_can_id = ps_canopen_sdo_ctrl->u16_canopen_tx_can_id;

			/* SDO upload segment request: */
	    	/* CANopen command byte = 011 b0000 (ccs = 3: upload segment response, b: toggle bit, others parameters not used) */
	    	/* 8-byte CAN frame = |Command (1 byte)|Object idx (2 bytes)|Object subindex (1 byte)| 0 (4 bytes)| */
			s_canopen_frame.au8_can_data[0] = K_CANOPEN_SDO_SEG_UPLOAD;
	    	if (E_SDO_UPLOAD_SEG0 != ps_canopen_sdo_ctrl->e_sdo_state)
	    	{
	    		/* Set the toggle bit */
	    		s_canopen_frame.au8_can_data[0] |= 0x10;
	    	}
	    	/* Object index in little endian format */
	    	s_canopen_frame.au8_can_data[1] = (uint8)(ps_canopen_sdo_ctrl->u16_sdo_object_id & 0xFF);
	    	s_canopen_frame.au8_can_data[2] = (uint8)(ps_canopen_sdo_ctrl->u16_sdo_object_id >> 8);
	    	/* Object subindex */
	    	s_canopen_frame.au8_can_data[3] = ps_canopen_sdo_ctrl->u16_sdo_object_id;
	    	/* 4 bytes of 0 */
	    	memset(&s_canopen_frame.au8_can_data[4], 0, K_CANOPEN_SDO_FRAME_DLC - 4);

	    	s_canopen_frame.u8_can_dlc = K_CANOPEN_SDO_FRAME_DLC;

			/* Request sending */
			t_returned_status = ComWrapperCanOpen_Send(&s_canopen_frame, &e_can_status);

			if ((E_OK == t_returned_status) && (E_CANOPENWRP_DATA_OK != e_can_status))
			{
				/* Low level driver error */
				*pe_canopen_exchange_status = E_CANOPEN_NETWORK_INTERFACE_ERROR;
			} else if (E_OK == t_returned_status)
			{
				/* Abort request sent */
				*pe_canopen_exchange_status = E_CANOPEN_TRANSFER_IN_PROGRESS;
			} else {
				/* Internal driver error */
			}
			break;

		default:
			/* Nothing to do for other states*/
			*pe_canopen_exchange_status = E_CANOPEN_TRANSFER_IN_PROGRESS;
			t_returned_status = E_OK;
			break;
		}
	}

	return t_returned_status;
}


/* Read object index, object subindex and object integer data in a received CANopen frame
 * Inputs:
 *     - au8_canopen_data: received CANopen data frame
 *
 * Outputs:
 *     - pu16_canopen_object_idx: the object index read from the frame
 *     - pu8_canopen_object_subidx: the object subindex read from the frame
 *     - pu32_canopen_object_data: the integer data read from the frame
 *
 * Return code: none
 */
static void canopen_read_int_data(const uint8 au8_canopen_data[K_CANOPEN_DATA_FRAME_LEN], uint16 * const pu16_canopen_object_idx, uint8 * const pu8_canopen_object_subidx,
		uint32 * const pu32_canopen_object_data)
{
	uint8 data_len_u8;

	/* Format of the CANopen frame */
	/* 8-byte CAN frame = |Command (1 byte)|Object idx (2 bytes)|Object subindex (1 byte)|Object data (4 bytes)| */
	/* Object index in little endian format */
	*pu16_canopen_object_idx = (uint16)(au8_canopen_data[2] << 8) | (uint16)au8_canopen_data[1];
	/* Object subindex */
	*pu8_canopen_object_subidx = au8_canopen_data[3];
	/* Object data in little endian format */

	/* Length of the integer data is given by the command byte = xxxx ccxx: cc = number of data bytes that do NOT contain data */
	data_len_u8 = K_CANOPEN_SDO_FRAME_DLC - 4 - ((au8_canopen_data[0] >> 2) & 3);
	*pu32_canopen_object_data = 0;
	while (data_len_u8 > 0)
	{
		*pu32_canopen_object_data = ((*pu32_canopen_object_data) << 8) | au8_canopen_data[3 + data_len_u8];
		data_len_u8--;
	}
}
