/*============================================================================*/
/*                        	ROBOSOFT                                      */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*/
/*! 
 * $Source: Protocol_CanOpen.c 
 * $Author: SLoisel 
 * $Date: 2015/12/08  
 */
/*============================================================================*/
/* DESCRIPTION :                                                              */
/** \file  Protocol_CanOpen.c 
  * \brief Source of CanOpen Protocol
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
#include "Protocol_CanOpen.h"
#include "Library_PrivateCanOpenSdoManage.h"
#include "Library_PrivateCanOpenNmtManage.h"
#include "Library_PrivateCanOpenPdoManage.h"
#include "Library_PrivateCanOpenSyncManage.h"
#include "Library_PrivateCanOpenComWrapper.h"
#include <stdio.h>
#include <string.h>

/* Constants and types  						      */
/*============================================================================*/ 

/* Variables 								      */
/*============================================================================*/ 
static Std_ReturnType gt_InitCanOpen_Status = E_NOT_OK; /* Global Initialization variable */
static Std_ReturnType gt_CfgCanOpen_Status = E_NOT_OK; /* Global Configuration variable */

/* Private functions prototypes 					      */
/*============================================================================*/ 

/* Inline functions 							      */
/*============================================================================*/

/* Public functions 							      */
/*============================================================================*/

/**
 * \fn  Std_ReturnType ProCan_Initialize(void)
 * \brief CANopen library initialization
 *
 * \param  void
 *
 * \return Std_ReturnType :
 *         - E_OK if module is initialized
 *         - E_NOT_OK if not
 */
Std_ReturnType ProCan_Initialize(void)
{
    /*! Declaration */

    /*! Initialization */

    /*! Function Body */
	/* Dependencies initialization */
	gt_InitCanOpen_Status = ProCan_SdoManage_Initialize();
	gt_InitCanOpen_Status |= ProCan_PdoManage_Initialize();
	gt_InitCanOpen_Status |= ProCan_NmtManage_Initialize();
	gt_InitCanOpen_Status |= ProCan_SyncManage_Initialize();
	gt_InitCanOpen_Status |= ComWrapperCanOpen_Init();

    /*! return code */
    return(gt_InitCanOpen_Status);
}

/**
 * \fn  Std_ReturnType ProCan_Configure(const ts_CanopenCfg * const ps_canopen_cfg)
 * \brief CANopen ports configuration
 *
 * \param <ts_CanopenCfg *ps_canopen_cfg> { CANopen library configuration }
 *
 * \return Std_ReturnType :
 *         - E_OK if module is initialized and configuration is successful
 *         - E_NOT_OK if not
 */
Std_ReturnType ProCan_Configure(const ts_CanopenCfg * const ps_canopen_cfg)
{
    /*! Declaration */

    /*! Initialization */

    /*! Function Body */
	if ((E_OK == gt_InitCanOpen_Status) && (NULL_PTR != ps_canopen_cfg))
	{
		/* Initialize configuration status to OK */
		gt_CfgCanOpen_Status = E_OK;

		/* Dependencies configuration */
		gt_CfgCanOpen_Status |= ProCan_SdoManage_Configure(ps_canopen_cfg);
		gt_CfgCanOpen_Status |= ProCan_PdoManage_Configure(ps_canopen_cfg);
		gt_CfgCanOpen_Status |= ProCan_NmtManage_Configure(ps_canopen_cfg);
		gt_CfgCanOpen_Status |= ProCan_SyncManage_Configure(ps_canopen_cfg);
		gt_CfgCanOpen_Status |= ComWrapperCanOpen_Configure(ps_canopen_cfg);
	} else {
		/* Module is not initialized or null pointer encountered, Halt */
		;
	}

    /*! return code */
    return(gt_CfgCanOpen_Status);
}

/**
 * \fn  Std_ReturnType ProCan_Close(const ts_CanopenCfg * const ps_canopen_cfg)
 * \brief Close all CANopen ports
 *
 * \param <ts_CanopenCfg *ps_canopen_cfg> { CANopen library configuration }
 *
 * \return Std_ReturnType :
 *         - E_OK if module is initialized and configuration is successful
 *         - E_NOT_OK if not
 */
Std_ReturnType ProCan_Close(const ts_CanopenCfg * const ps_canopen_cfg)
{
	/*! Declaration */
	Std_ReturnType t_Close_Status;

	/*! Initialization */
	t_Close_Status = E_NOT_OK;

	/*! Function Body */
	if ((E_OK == gt_CfgCanOpen_Status) && (NULL_PTR != ps_canopen_cfg))
	{
		t_Close_Status = ComWrapperCanOpen_Close(ps_canopen_cfg);
	} else {
		/* Module is not initialized or null pointer encountered, Halt */
	}

	/*! return code */
	return(t_Close_Status);
}

/**
 * \fn  Std_ReturnType ProCan_Start_SdoUpload(const uint8 u8_canopen_node_idx, const uint16 u16_canopen_object_idx, const uint8 u8_canopen_object_subidx, te_CanopenStatus * const pe_canopen_exchange_status)
 * \brief Start a CANopen SDO upload request
 *
 * \param  <uint8 u8_canopen_node_idx> {the node index on the CAN network on which the CANopen command must be launched (0..K_CANOPEN_NODES_NR - 1)}
 * \param  <uint16 u16_canopen_object_idx> {the requested CANopen object index}
 * \param  <uint8 u8_canopen_subidx> {the requested CANopen object subindex}
 * \param  <te_CanopenStatus * pe_canopen_exchange_status> {status of the CANopen command:
 *  				   	- E_CANOPEN_TRANSFER_IN_PROGRESS: SDO transfer initiated successfully and is in progress
 *     					- E_CANOPEN_NETWORK_INTERFACE_ERROR: general error received from the underlying communication driver }
 *
 * \return Std_ReturnType :
 *         - E_OK if command is successful
 *         - E_NOT_OK if not
 */
Std_ReturnType ProCan_Start_SdoUpload(const uint8 u8_canopen_node_idx, const uint16 u16_canopen_object_idx, const uint8 u8_canopen_object_subidx, te_CanopenStatus * const pe_canopen_exchange_status)
{
    /*! Declaration */     
    Std_ReturnType t_Start_SdoUpload_Status;

    /*! Initialization */
    t_Start_SdoUpload_Status = E_NOT_OK;

    /*! Function Body */
    if ((E_OK == gt_CfgCanOpen_Status) && (NULL_PTR != pe_canopen_exchange_status) && (u8_canopen_node_idx < K_CANOPEN_NODES_NR))
    {
    	t_Start_SdoUpload_Status = ProCan_SdoManage_StartUpload(u8_canopen_node_idx, u16_canopen_object_idx, u8_canopen_object_subidx, pe_canopen_exchange_status);
    } else {
    	/* Module is not initialized, null pointer encountered or invalid CANopen index */
    }

    /*! return code */
    return(t_Start_SdoUpload_Status);
}

/**
 * \fn  Std_ReturnType ProCan_Start_SdoDownload(const uint8 u8_canopen_node_idx, const uint16 u16_canopen_object_idx, const uint8 u8_canopen_object_subidx,
 *		ts_CanopenData * const ps_canopen_data, te_CanopenStatus * const pe_canopen_exchange_status)
 * \brief Start a CANopen SDO download request
 *
 * \param <uint8 u8_canopen_node_idx> {the node index on the CAN network on which the CANopen command must be launched (0..K_CANOPEN_NODES_NR - 1)}
 * \param <uint16 u16_canopen_object_idx> {the requested CANopen object index}
 * \param <uint8 u8_canopen_subidx> {the requested CANopen object subindex}
 * \param <ts_CanopenData *ps_canopen_data> {the buffer providing the data to be sent from the driver interface caller:
 * 			 			  t_data_kind = E_CANOPEN_INTxx => u32_integer_val_str_len = data
 *     					  t_data_kind = E_CANOPEN_STR   => not supported}
 * \param <te_CanopenStatus *pe_canopen_exchange_status> {status of the CANopen command:
 *     				- E_CANOPEN_TRANSFER_IN_PROGRESS: SDO transfer initiated successfully and is in progress
 *     				- E_CANOPEN_NETWORK_INTERFACE_ERROR: general error received from the underlying communication driver }
 *
 * \return Std_ReturnType :
 *         - E_OK if command is successful
 *         - E_NOT_OK if not
 */
Std_ReturnType ProCan_Start_SdoDownload(const uint8 u8_canopen_node_idx, const uint16 u16_canopen_object_idx, const uint8 u8_canopen_object_subidx,
										ts_CanopenData * const ps_canopen_data, te_CanopenStatus * const pe_canopen_exchange_status)
{
    /*! Declaration */     
    Std_ReturnType t_Start_SdoDownload_Status;

    /*! Initialization */
    t_Start_SdoDownload_Status = E_NOT_OK;

    /*! Function Body */
    if ((E_OK == gt_CfgCanOpen_Status) && (NULL_PTR != ps_canopen_data) && (NULL_PTR != pe_canopen_exchange_status) && (u8_canopen_node_idx < K_CANOPEN_NODES_NR))
    {
    	t_Start_SdoDownload_Status = ProCan_SdoManage_StartDownload(u8_canopen_node_idx, u16_canopen_object_idx, u8_canopen_object_subidx, ps_canopen_data, pe_canopen_exchange_status);
    } else {
    	/* Module is not initialized, null pointer encountered or invalid CANopen index */
    }

    /*! return code */
    return(t_Start_SdoDownload_Status);
}

/**
 * \fn  Std_ReturnType ProCan_RegisterTPdo(const uint8 u8_canopen_node_idx, const uint16 u16_canopen_tpdo_can_id, const uint8 u8_canopen_tpdo_timeout_val, uint8 * const pu8_canopen_pdo_channel)
 * \brief Register a TPDO in reception (only available with CAN communication layer)
 *
 * \param  <uint8 u8_canopen_node_idx> {the node index on the CAN network on which the CANopen command must be launched (0..K_CANOPEN_NODES_NR - 1)
 * \param  <uint16 u16_canopen_tpdo_can_id> {the TPDO CAN identifier}
 * \param  <uint8 u8_canopen_tpdo_timeout_val> {the TPDO timeout value (number of CANopen library scheduling ticks between two consecutive TPDO receptions)}
 * \param  <uint8 *pu8_canopen_pdo_idx> {the TPDO index of the CANopen node (0..K_CANOPEN_PDO_MAX_NR - 1)}
 *
 * \return Std_ReturnType :
 *         - E_OK if command is successful
 *         - E_NOT_OK if not
 */
Std_ReturnType ProCan_RegisterTPdo(const uint8 u8_canopen_node_idx, const uint16 u16_canopen_tpdo_can_id, const uint8 u8_canopen_tpdo_timeout_val, uint8 * const pu8_canopen_pdo_channel)
{
    /*! Declaration */
    Std_ReturnType t_Register_Status;

    /*! Initialization */
    t_Register_Status = E_NOT_OK;

    /*! Function Body */
    if ((E_OK == gt_CfgCanOpen_Status) && (u8_canopen_node_idx < K_CANOPEN_NODES_NR) && (NULL_PTR != pu8_canopen_pdo_channel))
    {
    	t_Register_Status = ProCan_PdoManage_RegisterTPdo(u8_canopen_node_idx, u16_canopen_tpdo_can_id, u8_canopen_tpdo_timeout_val, pu8_canopen_pdo_channel);
	}
    else
    {
	/* Module is not initialized, null pointer encountered or CANopen node index is out of range */
	;
    }

    /*! return code */
    return(t_Register_Status);
}

/**
 * \fn  Std_ReturnType ProCan_RegisterRPdo(const uint8 u8_canopen_node_idx, const uint16 u16_canopen_rpdo_can_id, uint8 * const pu8_canopen_pdo_channel)
 * \brief Register a RPDO in emission (only available with CAN communication layer)
 *
 * \param  <uint8 u8_canopen_node_idx> {the node index on the CAN network on which the CANopen command must be launched (0..K_CANOPEN_NODES_NR - 1)
 * \param  <uint16 u16_canopen_rpdo_can_id> {the RPDO CAN identifier}
 * \param  <uint8 *pu8_canopen_pdo_idx> {the RPDO index of the CANopen node (0..K_CANOPEN_PDO_MAX_NR - 1)}
 *
 * \return Std_ReturnType :
 *         - E_OK if command is successful
 *         - E_NOT_OK if not
 */
Std_ReturnType ProCan_RegisterRPdo(const uint8 u8_canopen_node_idx, const uint16 u16_canopen_rpdo_can_id, uint8 * const pu8_canopen_pdo_channel)
{
    /*! Declaration */
    Std_ReturnType t_Register_Status;

    /*! Initialization */
    t_Register_Status = E_NOT_OK;

    /*! Function Body */
    if ((E_OK == gt_CfgCanOpen_Status) && (u8_canopen_node_idx < K_CANOPEN_NODES_NR) && (NULL_PTR != pu8_canopen_pdo_channel))
    {
    	t_Register_Status = ProCan_PdoManage_RegisterRPdo(u8_canopen_node_idx, u16_canopen_rpdo_can_id, pu8_canopen_pdo_channel);
	}
    else
    {
    	/* Module is not initialized, null pointer encountered, CANopen node index is out of range or communication layer does not support PDOs */
	;
    }

    /*! return code */
    return(t_Register_Status);
}

/**
 * \fn  Std_ReturnType ProCan_PProCan_SendRPdodoManage_Send(const uint8 u8_canopen_node_idx, const uint8 u8_canopen_pdo_idx, const uint8 * const pu8_canopen_pdo_data, const uint8 u8_canopen_pdo_len,
 *		te_CanopenStatus * const pe_canopen_exchange_status)
 * \brief Send a PDO
 *
 * \param  <uint8 u8_canopen_node_idx> {the node index on the CAN network on which the CANopen command must be launched (0..gu8_canopen_nodes_nr - 1)
 * \param  <uint8 u8_canopen_pdo_idx> {the TPDO index of the CANopen node (0..K_CANOPEN_PDO_MAX_NR - 1)}
 * \param  <uint8 *pu8_canopen_pdo_data> {the transmitted PDO data}
 * \param  <uint8 u8_canopen_pdo_len> {the length of the transmitted PDO data}
 * \param  <te_CanopenStatus * pe_canopen_exchange_status> {status of the CANopen command:
 *  				   	- E_CANOPEN_TRANSFER_COMPLETE: PDO successfully sent
 *     					- E_CANOPEN_NETWORK_INTERFACE_ERROR: general error received from the underlying communication driver }
 *
 * \return Std_ReturnType :
 *         - E_OK if command is successful
 *         - E_NOT_OK if not
 */
Std_ReturnType ProCan_SendRPdo(const uint8 u8_canopen_node_idx, const uint8 u8_canopen_pdo_idx, const uint8 * const pu8_canopen_pdo_data, const uint8 u8_canopen_pdo_len,
		te_CanopenStatus * const pe_canopen_exchange_status)
{
    /*! Declaration */
    Std_ReturnType t_Send_Status;

    /*! Initialization */
    t_Send_Status = E_NOT_OK;

    /*! Function Body */
    if ((E_OK == gt_CfgCanOpen_Status) && (u8_canopen_node_idx < K_CANOPEN_NODES_NR) && (NULL_PTR != pe_canopen_exchange_status))
    {
    	t_Send_Status = ProCan_PdoManage_Send(u8_canopen_node_idx, u8_canopen_pdo_idx, pu8_canopen_pdo_data, u8_canopen_pdo_len, pe_canopen_exchange_status);
	}
    else
    {
	/* Module is not initialized, null pointer encountered or CANopen node index is out of range */
	;
    }

    /*! return code */
    return(t_Send_Status);
}


/**
 * \fn  Std_ReturnType ProCan_ControlNmtState(const uint8 u8_canopen_node_idx, const te_CanopenNmtCommands e_canopen_nmt_cmd, te_CanopenStatus * const pe_canopen_exchange_status)
 * \brief Control the communication state of network nodes
 *
 * \param  <uint8 u8_canopen_node_idx> {the node index on the CAN network on which the CANopen command must be launched (0..K_CANOPEN_NODES_NR - 1) or the broadcast value}
 * \param  <te_CanopenNmtCommands e_canopen_nmt_cmd> {the requested CANopen NMT command}
 * \param  <te_CanopenStatus * pe_canopen_exchange_status> {status of the CANopen command:
 *  				   	- E_CANOPEN_TRANSFER_COMPLETE: NMT command successfully sent
 *     					- E_CANOPEN_NETWORK_INTERFACE_ERROR: general error received from the underlying communication driver }
 *
 * \return Std_ReturnType :
 *         - E_OK if command is successful
 *         - E_NOT_OK if not
 */
Std_ReturnType ProCan_ControlNmtState(const uint8 u8_canopen_node_idx, const te_CanopenNmtCommands e_canopen_nmt_cmd, te_CanopenStatus * const pe_canopen_exchange_status)
{
    /*! Declaration */
    Std_ReturnType t_Send_NMTCtrl_Status;

    /*! Initialization */
    t_Send_NMTCtrl_Status = E_NOT_OK;

    /*! Function Body */
    if ((E_OK == gt_CfgCanOpen_Status) && ((u8_canopen_node_idx < K_CANOPEN_NODES_NR) || (K_CANOPEN_NMT_BROADCAST_ID == u8_canopen_node_idx)) && (NULL_PTR != pe_canopen_exchange_status))
    {
    	t_Send_NMTCtrl_Status = ProCan_NmtManage_StateControl(u8_canopen_node_idx, e_canopen_nmt_cmd, pe_canopen_exchange_status);
	}
    else
    {
	/* Module is not initialized, null pointer encountered or CANopen node index is out of range */
	;
    }

    /*! return code */
    return(t_Send_NMTCtrl_Status);
}

/**
 * \fn  Std_ReturnType ProCan_RequestNmtState(const uint8 u8_canopen_node_idx, te_CanopenStatus * const pe_canopen_exchange_status)
 * \brief Receive the communication state of a network node
 *
 * \param  <uint8 u8_canopen_node_idx> {the node index on the CAN network on which the CANopen command must be launched (0..K_CANOPEN_NODES_NR - 1)
 * \param  <te_CanopenStatus * pe_canopen_exchange_status> {status of the CANopen command:
 *  				   	- E_CANOPEN_TRANSFER_COMPLETE: NMT command successfully sent
 *     					- E_CANOPEN_NETWORK_INTERFACE_ERROR: general error received from the underlying communication driver }
 *
 * \return Std_ReturnType :
 *         - E_OK if command is successful
 *         - E_NOT_OK if not
 */
Std_ReturnType ProCan_RequestNmtState(const uint8 u8_canopen_node_idx, te_CanopenStatus * const pe_canopen_exchange_status)
{
    /*! Declaration */
    Std_ReturnType t_Send_NMTCtrl_Status;

    /*! Initialization */
    t_Send_NMTCtrl_Status = E_NOT_OK;

    /*! Function Body */
    if ((E_OK == gt_CfgCanOpen_Status) && (u8_canopen_node_idx < K_CANOPEN_NODES_NR) && (NULL_PTR != pe_canopen_exchange_status))
    {
    	t_Send_NMTCtrl_Status = ProCan_NmtManage_StateRequest(u8_canopen_node_idx, pe_canopen_exchange_status);
	}
    else
    {
	/* Module is not initialized, null pointer encountered or CANopen node index is out of range */
	;
    }

    /*! return code */
    return(t_Send_NMTCtrl_Status);
}


/**
 * \fn  Std_ReturnType ProCan_SendSync(te_CanopenStatus * const pe_canopen_exchange_status)
 * \brief Send the SYNC message
 *
 * \param  <te_CanopenStatus * pe_canopen_exchange_status> {status of the CANopen command:
 *  				   	- E_CANOPEN_TRANSFER_COMPLETE: SYNC command successfully sent
 *     					- E_CANOPEN_NETWORK_INTERFACE_ERROR: general error received from the underlying communication driver }
 *
 * \return Std_ReturnType :
 *         - E_OK if command is successful
 *         - E_NOT_OK if not
 */
Std_ReturnType ProCan_SendSync(te_CanopenStatus * const pe_canopen_exchange_status)
{
    /*! Declaration */
    Std_ReturnType t_Send_Status;

    /*! Initialization */
    t_Send_Status = E_NOT_OK;

    /*! Function Body */
    if ((E_OK == gt_CfgCanOpen_Status) && (NULL_PTR != pe_canopen_exchange_status))
    {
    	t_Send_Status = ProCan_SyncManage_Send(pe_canopen_exchange_status);
	}
    else
    {
	/* Module is not initialized, null pointer encountered or CANopen node index is out of range */
	;
    }

    /*! return code */
    return(t_Send_Status);
}

/**
 * \fn  Std_ReturnType ProCan_Process_Frames(Std_ReturnType ProCan_Process_Frames(uint8 * const pu8_canopen_node_idx, ts_CanopenData * const ps_canopen_data,
 *		te_CanopenStatus * const pe_canopen_exchange_status, te_CanopenFrameProcessStatus * const pe_frame_processed))
 * \brief Process received CANopen frames
 *
 * \param <uint8 *pu8_canopen_node_idx> {the node index on the CAN network from which transfer is complete or got a protocol error}
 * \param <ts_CanopenData *ps_canopen_data> {the buffer where to store the received data:
 *           when return code is E_CANOPEN_TRANSFER_IN_PROGRESS: contains a string segment along with its length
 *           when return code is E_CANOPEN_TRANSFER_COMPLETE: contains the received integer or the final string segment along with its length
 *           when return code is E_CANOPEN_TRANSFER_ERROR: contains the error information
 *           				e_data_kind = E_CANOPEN_NMT_STATE => NMT toggle bit error
 *           				e_data_kind = E_CANOPEN_SDO_INT32 => u32_integer_val_str_len = CANopen error code}
 * \param <te_CanopenStatus *pe_canopen_exchange_status> {status of the CANopen command:
 *     				- E_CANOPEN_TRANSFER_IN_PROGRESS: SDO transfer still in progress
 *	   				- E_CANOPEN_TRANSFER_COMPLETE: SDO, NMT or PDO transfer successful, data is ready according to the status of the CANopen frame processing
 *     				- E_CANOPEN_TRANSFER_ERROR: protocol error during SDO or NMT transfer (additional information in the CANopen data)
 *     				- E_CANOPEN_NETWORK_INTERFACE_ERROR: general error received from the underlying communication driver}
 * \param <te_CanopenFrameProcessStatus *pe_frame_processed> {the status of the CANopen frame processing}
 *
 * \return Std_ReturnType :
 *         - E_OK if command is successful
 *         - E_NOT_OK if not
 */
Std_ReturnType ProCan_Process_Frames(uint8 * const pu8_canopen_node_idx, ts_CanopenData * const ps_canopen_data,
		te_CanopenStatus * const pe_canopen_exchange_status, te_CanopenFrameProcessStatus * const pe_frame_processed)
{
    /*! Declaration */
    Std_ReturnType t_Process_Frames_Status;
	/* Status from the read service called */
    te_CanopenWrapperStatus e_data_read_status;
	/* Received CAN id and CANopen data / Segment request frame */
    ts_CanopenCanFrame s_canopen_frame;

    /*! Initialization */
	t_Process_Frames_Status = E_NOT_OK;
	e_data_read_status = E_CANOPENWRP_NO_DATA;

    /*! Function Body */
    if ((E_OK == gt_CfgCanOpen_Status) && (NULL_PTR != ps_canopen_data) && (NULL_PTR != pu8_canopen_node_idx) && (NULL_PTR != pe_canopen_exchange_status))
    {
    	/* Default status: transfer still in progress */
    	*pe_canopen_exchange_status = E_CANOPEN_TRANSFER_IN_PROGRESS;

    	/* Check if a CANopen frame has been received */
		t_Process_Frames_Status = ComWrapperCanOpen_Receive(&s_canopen_frame, &e_data_read_status);
		if ((E_OK == t_Process_Frames_Status) && (E_CANOPENWRP_DATA_OK == e_data_read_status))
		{
			/* Frame received */
			/* Check if it is an SDO frame */
			t_Process_Frames_Status = ProCan_SdoManage_ProcessFrame(&s_canopen_frame, pu8_canopen_node_idx, ps_canopen_data, pe_canopen_exchange_status, pe_frame_processed);
			if ((E_OK == t_Process_Frames_Status) && (E_CANOPEN_FRAME_NOT_PROCESSED == *pe_frame_processed))
			{
				/* Check if it is an NMT frame */
				t_Process_Frames_Status = ProCan_NmtManage_ProcessFrame(&s_canopen_frame, pu8_canopen_node_idx, ps_canopen_data, pe_canopen_exchange_status, pe_frame_processed);
			}

			if ((E_OK == t_Process_Frames_Status) && (E_CANOPEN_FRAME_NOT_PROCESSED == *pe_frame_processed))
			{
				/* Check if it is a PDO frame */
				t_Process_Frames_Status = ProCan_PdoManage_ProcessFrame(&s_canopen_frame, pu8_canopen_node_idx, ps_canopen_data, pe_canopen_exchange_status, pe_frame_processed);
			}
		} else if ((E_OK == t_Process_Frames_Status) && (E_CANOPENWRP_NO_DATA != e_data_read_status))
		{
			/* General low level driver error */
			*pe_canopen_exchange_status = E_CANOPEN_NETWORK_INTERFACE_ERROR;
			/* Simulate the reception of a frame with no data, so that the status is considered by the caller */
			*pe_frame_processed = E_CANOPEN_FRAME_PROCESSED_WITH_DATA;
			ps_canopen_data->e_data_kind = E_CANOPEN_NODATA;
		}
		else
		{
			if ((E_OK == t_Process_Frames_Status) &&
				(E_CANOPENWRP_NO_DATA == e_data_read_status))
			{
				/* No frame received, no status to consider */
				*pe_frame_processed = E_CANOPEN_FRAME_NOT_RECEIVED;
			}
			else
			{
				/* Internal driver error */
			}
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
 * \fn  Std_ReturnType ProCan_Check_SdoTimeout(const uint8 u8_canopen_node_idx, te_CanopenStatus * const pe_canopen_exchange_status)
 * \brief Check server response time-out
 *
 * \param <uint8 u8_canopen_node_idx> {the node index on the CAN network on which the CANopen command must be launched (0..K_CANOPEN_NODES_NR - 1)}
 * \param <te_CanopenStatus *pe_canopen_exchange_status> {status of the CANopen command:
 *	   				- E_CANOPEN_TRANSFER_IN_PROGRESS: SDO transfer is on-going
 *	   				- E_CANOPEN_TRANSFER_COMPLETE: SDO transfer complete or no SDO transfer is on-going
 *	   				- E_CANOPEN_TIMEOUT_ERROR: server response time-out occurred}
 *
 * \return Std_ReturnType :
 *         - E_OK if command is successful
 *         - E_NOT_OK if not
 */
Std_ReturnType ProCan_Check_SdoTimeout(const uint8 u8_canopen_node_idx, te_CanopenStatus * const pe_canopen_exchange_status)
{
    /*! Declaration */
    Std_ReturnType t_Check_SdoTimeout_Status;

    /*! Initialization */
	t_Check_SdoTimeout_Status = E_NOT_OK;

    /*! Function Body */
    if ((E_OK == gt_CfgCanOpen_Status) && (NULL_PTR != pe_canopen_exchange_status) && (u8_canopen_node_idx < K_CANOPEN_NODES_NR))
    {
		t_Check_SdoTimeout_Status = ProCan_SdoManage_CheckTimeout(u8_canopen_node_idx, pe_canopen_exchange_status);
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
 * \fn  Std_ReturnType ProCan_Check_NmtTimeout(const uint8 u8_canopen_node_idx, te_CanopenStatus * const pe_canopen_exchange_status)
 * \brief Check server response time-out
 *
 * \param <uint8 u8_canopen_node_idx> {the node index on the CAN network on which the CANopen command must be launched (0..K_CANOPEN_NODES_NR - 1)}
 * \param <te_CanopenStatus *pe_canopen_exchange_status> {status of the CANopen command:
 *	   				- E_CANOPEN_TRANSFER_IN_PROGRESS: NMT request is on-going
 *	   				- E_CANOPEN_TRANSFER_COMPLETE: NMT request complete or no NMT transfer is on-going
 *	   				- E_CANOPEN_TIMEOUT_ERROR: slave response time-out occurred}
 *
 * \return Std_ReturnType :
 *         - E_OK if command is successful
 *         - E_NOT_OK if not
 */
Std_ReturnType ProCan_Check_NmtTimeout(const uint8 u8_canopen_node_idx, te_CanopenStatus * const pe_canopen_exchange_status)
{
    /*! Declaration */
    Std_ReturnType t_Check_NmtTimeout_Status;

    /*! Initialization */
	t_Check_NmtTimeout_Status = E_NOT_OK;

    /*! Function Body */
    if ((E_OK == gt_CfgCanOpen_Status) && (NULL_PTR != pe_canopen_exchange_status) && (u8_canopen_node_idx < K_CANOPEN_NODES_NR))
    {
		t_Check_NmtTimeout_Status = ProCan_NmtManage_CheckTimeout(u8_canopen_node_idx, pe_canopen_exchange_status);
    }
    else
    {
    	/* Module is not initialized, null pointer encountered or CANopen node index is out of range */
	;
    }

    /*! return code */
    return(t_Check_NmtTimeout_Status);
}

/**
 * \fn  Std_ReturnType ProCan_Check_PdoTimeout(const uint8 u8_canopen_node_idx, const uint8 u8_canopen_tpdo_idx, te_CanopenStatus * const pe_canopen_exchange_status)
 * \brief Check server response time-out on TPDO
 *
 * \param <uint8 u8_canopen_node_idx> {the node index on the CAN network on which the CANopen command must be launched (0..K_CANOPEN_NODES_NR - 1)}
 * \param <uint8 u8_canopen_tpdo_idx> {the TPDO index of the CAN node on which the CANopen command must be launched (0..K_CANOPEN_PDO_MAX_NR - 1)}
 * \param <te_CanopenStatus *pe_canopen_exchange_status> {status of the CANopen command:
 *	   				- E_CANOPEN_TRANSFER_IN_PROGRESS: no receive TPDO time-out
 *	   				- E_CANOPEN_TIMEOUT_ERROR: receive TPDO time-out occurred}
 *
 * \return Std_ReturnType :
 *         - E_OK if command is successful
 *         - E_NOT_OK if not
 */
Std_ReturnType ProCan_Check_PdoTimeout(const uint8 u8_canopen_node_idx, const uint8 u8_canopen_tpdo_idx, te_CanopenStatus * const pe_canopen_exchange_status)
{
    /*! Declaration */
    Std_ReturnType t_Check_PdoTimeout_Status;

    /*! Initialization */
	t_Check_PdoTimeout_Status = E_NOT_OK;

    /*! Function Body */
    if ((E_OK == gt_CfgCanOpen_Status) && (NULL_PTR != pe_canopen_exchange_status) && (u8_canopen_node_idx < K_CANOPEN_NODES_NR))
    {
		t_Check_PdoTimeout_Status = ProCan_PdoManage_CheckTimeout(u8_canopen_node_idx, u8_canopen_tpdo_idx, pe_canopen_exchange_status);
    }
    else
    {
    	/* Module is not initialized, null pointer encountered or CANopen node index is out of range */
	;
    }

    /*! return code */
    return(t_Check_PdoTimeout_Status);
}

/**
 * \fn  Std_ReturnType ProCan_Abort_SdoTransfer(const uint8 u8_canopen_node_idx, const canopen_sdo_abort_reason_type canopen_errinfo_abort_t, te_CanopenStatus * const pe_canopen_exchange_status)
 * \brief Abort the current SDO transfer
 *
 * \param <uint8 u8_canopen_node_idx> {the node index on the CAN network on which the CANopen command must be launched (0..K_CANOPEN_NODES_NR - 1)}
 * \param <canopen_sdo_abort_reason_type canopen_errinfo_abort_t> {the error information about the abort reason}
 *
 * \return Std_ReturnType :
 *         - E_OK if command is successful
 *         - E_NOT_OK if not
 */
Std_ReturnType ProCan_Abort_SdoTransfer(const uint8 u8_canopen_node_idx, const te_CanopenSdoAbortReason e_canopen_errinfo_abort)
{
    /*! Declaration */
    Std_ReturnType t_Abort_SdoTransfer_Status;

    /*! Initialization */
    t_Abort_SdoTransfer_Status = E_NOT_OK;

    /*! Function Body */
    if ((E_OK == gt_CfgCanOpen_Status) && (u8_canopen_node_idx < K_CANOPEN_NODES_NR))
    {
		t_Abort_SdoTransfer_Status = ProCan_SdoManage_AbortTransfer(u8_canopen_node_idx, e_canopen_errinfo_abort);
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


