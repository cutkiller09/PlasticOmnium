/*============================================================================*/
/*                        	ROBOSOFT                                      */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*/
/*! 
 * $Source: Library_PrivateCanOpenPdoManage.c
 * $Author: BRobin 
 * $Date: 2015/12/08  
 */
/*============================================================================*/
/* DESCRIPTION :                                                              */
/** \file  Library_PrivateCanOpenPdoManage.c
  * \brief Fichier Source du Service PrivateCanOpenPdoManage
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
#include "Library_PrivateCanOpenPdoManage.h"
#include "Library_PrivateCanOpenComWrapper.h"
#include <stdio.h>
#include <string.h>

/* Constants and types  						      */
/*============================================================================*/ 

/**************************************/
/* CANopen general */

/* Broadcast node identifier */
#define K_CANOPEN_BROADCAST_NODE_ID			0

/* CANopen node identifier mask within COB-ID */
#define K_CANOPEN_NODE_ID_FROM_CANID		0x07F

/* CANopen function code mask within COB-ID */
#define K_CANOPEN_FUNC_CODE_FROM_CANID		0x780


/**************************************/
/* PDO controller */

/* Indication of free RPDO/TPDO slot: CAN identifier 0xFFFF does not exist */
#define K_CANOPEN_FREE_PDO_SLOT		0xFFFF

/* Definition of a CANopen NMT controller */
typedef struct
{
	/* Transmit PDO COB-ID configuration */
	uint16	u16_tpdo_canid[K_CANOPEN_PDO_MAX_NR];

	/* Receive PDO COB-ID configuration */
	uint16	u16_rpdo_canid[K_CANOPEN_PDO_MAX_NR];

	/* For time-out checking: max duration between two consecutive received TPDO */
	uint8	u8_tpdo_timeout_value[K_CANOPEN_PDO_MAX_NR];
	uint8	u8_tpdo_timeout_ctr[K_CANOPEN_PDO_MAX_NR];
} ts_CanopenPdoController;


/* Variables 								      */
/*============================================================================*/ 
static Std_ReturnType gt_InitCanOpen_PdoManage_Status = E_NOT_OK; /* Global Initialization variable */
static Std_ReturnType gt_CfgCanOpen_PdoManage_Status = E_NOT_OK; /* Global Configuration variable */


/* Current states of the CANopen protocol exchanges on the CAN networks */
static ts_CanopenPdoController gas_canopen_pdo_ctrl[K_CANOPEN_NODES_NR];
static uint8 gu8_canopen_nodes_nr;

/* Lower layer communication configuration */
static te_CanopenWrapperType ge_canopen_com_wrapper;

/* Private functions prototypes 					      */
/*============================================================================*/ 


/* Inline functions 							      */
/*============================================================================*/

/* Public functions 							      */
/*============================================================================*/


/**
 * \fn  Std_ReturnType ProCan_PdoManage_Initialize(void)
 * \brief CANopen library initialization
 *
 * \param  void
 *
 * \return Std_ReturnType :
 *         - E_OK if module is initialized
 *         - E_NOT_OK if not
 */
Std_ReturnType ProCan_PdoManage_Initialize(void)
{
    /*! Declaration */

    /*! Initialization */

    /*! Function Body */
	gu8_canopen_nodes_nr = 0;
	gt_InitCanOpen_PdoManage_Status = E_OK;

    /*! return code */
    return(gt_InitCanOpen_PdoManage_Status);
}

/**
 * \fn  Std_ReturnType ProCan_PdoManage_Configure(const ts_CanopenCfg * const ps_canopen_cfg)
 * \brief CANopen ports configuration
 *
 * \param <ts_CanopenCfg *ps_canopen_cfg> { CANopen library configuration }
 *
 * \return Std_ReturnType :
 *         - E_OK if module is initialized and configuration is successful
 *         - E_NOT_OK if not
 */
Std_ReturnType ProCan_PdoManage_Configure(const ts_CanopenCfg * const ps_canopen_cfg)
{
    /*! Declaration */
	uint8 u8_node_idx;
	uint8 u8_pdo_idx;

    /*! Initialization */
	gt_CfgCanOpen_PdoManage_Status = E_NOT_OK;

    /*! Function Body */
	if ((E_OK == gt_InitCanOpen_PdoManage_Status) && (NULL_PTR != ps_canopen_cfg))
	{
		if (ps_canopen_cfg->u8_canopen_nodes_nr_cfg <= K_CANOPEN_NODES_NR)
		{
			/* Write the new CANopen configuration */
			gu8_canopen_nodes_nr = ps_canopen_cfg->u8_canopen_nodes_nr_cfg;

			/* Initialize the configuration status to OK */
			gt_CfgCanOpen_PdoManage_Status = E_OK;

			for (u8_node_idx = 0; u8_node_idx < gu8_canopen_nodes_nr; u8_node_idx++)
			{
				/* Configure TPDOs and RPDOs of the node */
				for (u8_pdo_idx = 0; u8_pdo_idx < K_CANOPEN_PDO_MAX_NR; u8_pdo_idx++)
				{
					gas_canopen_pdo_ctrl[u8_node_idx].u16_tpdo_canid[u8_pdo_idx] = K_CANOPEN_FREE_PDO_SLOT;
					gas_canopen_pdo_ctrl[u8_node_idx].u16_rpdo_canid[u8_pdo_idx] = K_CANOPEN_FREE_PDO_SLOT;
				}
			}

			/* Communication wrapper configuration */
			ge_canopen_com_wrapper = ps_canopen_cfg->e_canopen_wrapper_cfg;
		} else {
			/* Invalid number of CANopen nodes */
		}
	} else {
		/* Module is not initialized, invalid number of CANopen nodes or null pointer encountered, Halt */
		;
	}

    /*! return code */
    return(gt_CfgCanOpen_PdoManage_Status);
}

/**
 * \fn  Std_ReturnType ProCan_PdoManage_RegisterTPdo(const uint8 u8_canopen_node_idx, const uint16 u16_canopen_tpdo_can_id, const uint8 u8_canopen_tpdo_timeout_val, uint8 * const pu8_canopen_pdo_channel)
 * \brief Register a TPDO in reception (only available with CAN communication layer)
 *
 * \param  <uint8 u8_canopen_node_idx> {the node index on the CAN network on which the CANopen command must be launched (0..gu8_canopen_nodes_nr - 1)
 * \param  <uint16 u16_canopen_tpdo_can_id> {the TPDO CAN identifier}
 * \param  <uint8 u8_canopen_tpdo_timeout_val> {the TPDO timeout value (number of CANopen library scheduling ticks between two consecutive TPDO receptions)}
 * \param  <uint8 *pu8_canopen_pdo_idx> {the TPDO index of the CANopen node (0..K_CANOPEN_PDO_MAX_NR - 1)}
 *
 * \return Std_ReturnType :
 *         - E_OK if command is successful
 *         - E_NOT_OK if not
 */
Std_ReturnType ProCan_PdoManage_RegisterTPdo(const uint8 u8_canopen_node_idx, const uint16 u16_canopen_tpdo_can_id, const uint8 u8_canopen_tpdo_timeout_val, uint8 * const pu8_canopen_pdo_channel)
{
    /*! Declaration */
    Std_ReturnType t_Register_Status;
    uint8 u8_pdo_idx;

    /*! Initialization */
    t_Register_Status = E_NOT_OK;

    /*! Function Body */
    if ((E_OK == gt_CfgCanOpen_PdoManage_Status) && (u8_canopen_node_idx < gu8_canopen_nodes_nr) && (NULL_PTR != pu8_canopen_pdo_channel) && (E_CANOPENWRP_SERIAL_XTPP != ge_canopen_com_wrapper))
    {
    	/* Check if a TPDO slot is available */
    	for (u8_pdo_idx = 0; (u8_pdo_idx < K_CANOPEN_PDO_MAX_NR) && (K_CANOPEN_FREE_PDO_SLOT != gas_canopen_pdo_ctrl[u8_canopen_node_idx].u16_tpdo_canid[u8_pdo_idx]); u8_pdo_idx++)
    	{
    		/* Check next TPDO slot */
    	}

    	if (K_CANOPEN_FREE_PDO_SLOT == gas_canopen_pdo_ctrl[u8_canopen_node_idx].u16_tpdo_canid[u8_pdo_idx])
    	{
    		/* TPDO slot available: register the new TPDO */
    		gas_canopen_pdo_ctrl[u8_canopen_node_idx].u16_tpdo_canid[u8_pdo_idx] = u16_canopen_tpdo_can_id;
    		gas_canopen_pdo_ctrl[u8_canopen_node_idx].u8_tpdo_timeout_value[u8_pdo_idx] = u8_canopen_tpdo_timeout_val;
    		gas_canopen_pdo_ctrl[u8_canopen_node_idx].u8_tpdo_timeout_ctr[u8_pdo_idx] = u8_canopen_tpdo_timeout_val + 1;
    		*pu8_canopen_pdo_channel = u8_pdo_idx;

    		t_Register_Status = E_OK;
    	} else {
    		/* No slot available */
    	}
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
 * \fn  Std_ReturnType ProCan_PdoManage_RegisterRPdo(const uint8 u8_canopen_node_idx, const uint16 u16_canopen_rpdo_can_id, uint8 * const pu8_canopen_pdo_channel)
 * \brief Register a RPDO in emission (only available with CAN communication layer)
 *
 * \param  <uint8 u8_canopen_node_idx> {the node index on the CAN network on which the CANopen command must be launched (0..gu8_canopen_nodes_nr - 1)
 * \param  <uint16 u16_canopen_rpdo_can_id> {the RPDO CAN identifier}
 * \param  <uint8 *pu8_canopen_pdo_idx> {the RPDO index of the CANopen node (0..K_CANOPEN_PDO_MAX_NR - 1)}
 *
 * \return Std_ReturnType :
 *         - E_OK if command is successful
 *         - E_NOT_OK if not
 */
Std_ReturnType ProCan_PdoManage_RegisterRPdo(const uint8 u8_canopen_node_idx, const uint16 u16_canopen_rpdo_can_id, uint8 * const pu8_canopen_pdo_channel)
{
    /*! Declaration */
    Std_ReturnType t_Register_Status;
    uint8 u8_pdo_idx;

    /*! Initialization */
    t_Register_Status = E_NOT_OK;

    /*! Function Body */
    if ((E_OK == gt_CfgCanOpen_PdoManage_Status) && (u8_canopen_node_idx < gu8_canopen_nodes_nr) && (NULL_PTR != pu8_canopen_pdo_channel) && (E_CANOPENWRP_SERIAL_XTPP != ge_canopen_com_wrapper))
    {
    	/* Check if a RPDO slot is available */
    	for (u8_pdo_idx = 0; (u8_pdo_idx < K_CANOPEN_PDO_MAX_NR) && (K_CANOPEN_FREE_PDO_SLOT != gas_canopen_pdo_ctrl[u8_canopen_node_idx].u16_rpdo_canid[u8_pdo_idx]); u8_pdo_idx++)
    	{
    		/* Check next RPDO slot */
    	}

    	if (K_CANOPEN_FREE_PDO_SLOT == gas_canopen_pdo_ctrl[u8_canopen_node_idx].u16_rpdo_canid[u8_pdo_idx])
    	{
    		/* RPDO slot available: register the new RPDO */
    		gas_canopen_pdo_ctrl[u8_canopen_node_idx].u16_rpdo_canid[u8_pdo_idx] = u16_canopen_rpdo_can_id;
    		*pu8_canopen_pdo_channel = u8_pdo_idx;

    		t_Register_Status = E_OK;
    	} else {
    		/* No slot available */
    	}
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
 * \fn  Std_ReturnType ProCan_PdoManage_Send(const uint8 u8_canopen_node_idx, const uint8 u8_canopen_pdo_idx, const uint8 * const pu8_canopen_pdo_data, const uint8 u8_canopen_pdo_len,
 *		te_CanopenStatus * const pe_canopen_exchange_status)
 * \brief Send a RPDO
 *
 * \param  <uint8 u8_canopen_node_idx> {the node index on the CAN network on which the CANopen command must be launched (0..gu8_canopen_nodes_nr - 1)
 * \param  <uint8 u8_canopen_pdo_idx> {the RPDO index of the CANopen node (0..K_CANOPEN_PDO_MAX_NR - 1)}
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
Std_ReturnType ProCan_PdoManage_Send(const uint8 u8_canopen_node_idx, const uint8 u8_canopen_pdo_idx, const uint8 * const pu8_canopen_pdo_data, const uint8 u8_canopen_pdo_len,
		te_CanopenStatus * const pe_canopen_exchange_status)
{
    /*! Declaration */
    Std_ReturnType t_Send_Status;
	/* Status from the write service called */
    te_CanopenWrapperStatus e_can_status;
	/* Data exchanged with the underlying communication layer */
    ts_CanopenCanFrame s_canopen_frame;

    /*! Initialization */
    e_can_status=E_CANOPENWRP_NO_DATA;
    t_Send_Status=E_NOT_OK;

    /*! Function Body */
    if ((E_OK == gt_CfgCanOpen_PdoManage_Status) && (u8_canopen_node_idx < gu8_canopen_nodes_nr) && (NULL_PTR != pe_canopen_exchange_status)
    		&& (K_CANOPEN_FREE_PDO_SLOT != gas_canopen_pdo_ctrl[u8_canopen_node_idx].u16_rpdo_canid[u8_canopen_pdo_idx]) && (u8_canopen_pdo_len <= K_CANOPEN_DATA_FRAME_LEN))
    {
    	/* Build NMT control nodes frame */
    	s_canopen_frame.e_can_frame_type = E_CANOPEN_NORMAL_FRAME;
    	s_canopen_frame.u32_can_id = gas_canopen_pdo_ctrl[u8_canopen_node_idx].u16_rpdo_canid[u8_canopen_pdo_idx];

    	/* The CAN data payload must contain the PDO data */
    	memcpy(s_canopen_frame.au8_can_data, pu8_canopen_pdo_data, u8_canopen_pdo_len);
    	s_canopen_frame.u8_can_dlc = u8_canopen_pdo_len;

    	/* Send the message */
    	t_Send_Status = ComWrapperCanOpen_Send(&s_canopen_frame, &e_can_status);

		if ((E_OK == t_Send_Status) &&
			(E_CANOPENWRP_DATA_OK == e_can_status))
		{
			/* Request correctly sent */
			*pe_canopen_exchange_status = E_CANOPEN_TRANSFER_COMPLETE;
		}
		else
		{
			if ((E_OK == t_Send_Status) &&
				(E_CANOPENWRP_DATA_OK != e_can_status))
			{
				/* Low level driver error */
				*pe_canopen_exchange_status = E_CANOPEN_NETWORK_INTERFACE_ERROR;
			}
			else
			{
				/* Internal driver error */
			}
		}
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
 * \fn  Std_ReturnType ProCan_PdoManage_ProcessFrame(const ts_CanopenCanFrame * const ps_canopen_can_frame, uint8 * const u8_canopen_node_idx,
 * 						ts_CanopenData * const ps_canopen_data, te_CanopenStatus * const pe_canopen_exchange_status, te_CanopenFrameProcessStatus * const pe_frame_processed)
 * \brief Process received CANopen frames
 *
 * \param <ts_CanopenCanFrame *ps_canopen_frame> {the CAN frame to decode}
 * \param <uint8 *u8_canopen_node_idx> {the node index on the CAN network from which transfer is complete or got a protocol error}
 * \param <ts_CanopenData *ps_canopen_data> {the buffer where to store the received PDO (when return code is E_CANOPEN_TRANSFER_COMPLETE)}
 * \param <te_CanopenStatus *pe_canopen_exchange_status> {status of the CANopen command:
 *	   				- E_CANOPEN_TRANSFER_COMPLETE: PDO receive successful}
 * \param <te_CanopenFrameProcessStatus *pe_frame_processed> {the status of the CANopen frame processing}
 *
 * \return Std_ReturnType :
 *         - E_OK if command is successful
 *         - E_NOT_OK if not
 */
Std_ReturnType ProCan_PdoManage_ProcessFrame(const ts_CanopenCanFrame * const ps_canopen_can_frame, uint8 * const u8_canopen_node_idx,
		ts_CanopenData * const ps_canopen_data, te_CanopenStatus * const pe_canopen_exchange_status, te_CanopenFrameProcessStatus * const pe_frame_processed)
{
    /*! Declaration */
    Std_ReturnType t_Process_Frames_Status;
	/* Node index of the received frame */
	uint8 u8_node_idx;
	/* PDO index */
	uint8 u8_pdo_idx;

    /*! Initialization */
	t_Process_Frames_Status = E_NOT_OK;

    /*! Function Body */
    if ((E_OK == gt_CfgCanOpen_PdoManage_Status) && (NULL_PTR != ps_canopen_data) && (NULL_PTR != u8_canopen_node_idx) && (NULL_PTR != pe_canopen_exchange_status))
    {
		/* Check if the CAN identifier is a PDO slave emission */
		if ((E_CANOPEN_NORMAL_FRAME == ps_canopen_can_frame->e_can_frame_type) && (ps_canopen_can_frame->u8_can_dlc <= K_CANOPEN_DATA_FRAME_LEN))
		{
			/* Retrieve the CANopen node index from the CAN identifier */
			*pe_frame_processed = E_CANOPEN_FRAME_NOT_PROCESSED;
			for (u8_node_idx = 0; (u8_node_idx < gu8_canopen_nodes_nr) && (E_CANOPEN_FRAME_NOT_PROCESSED == *pe_frame_processed); u8_node_idx++)
			{
				for (u8_pdo_idx = 0; (u8_pdo_idx < K_CANOPEN_PDO_MAX_NR) && (E_CANOPEN_FRAME_NOT_PROCESSED == *pe_frame_processed); u8_pdo_idx++)
				{
					if (ps_canopen_can_frame->u32_can_id == gas_canopen_pdo_ctrl[u8_node_idx].u16_tpdo_canid[u8_pdo_idx])
					{
						/* PDO received from this CANopen node in this TPDO channel index */
						*u8_canopen_node_idx = u8_node_idx;
						ps_canopen_data->e_data_kind = E_CANOPEN_PDO;
						ps_canopen_data->u8_tpdo_idx = u8_pdo_idx;

						/* Copy the PDO data into the CANopen data */
						memcpy(ps_canopen_data->au8_string_val, ps_canopen_can_frame->au8_can_data, ps_canopen_can_frame->u8_can_dlc);
						ps_canopen_data->u32_integer_val_str_len = ps_canopen_can_frame->u8_can_dlc;

						/* Reset the time-out counter */
						gas_canopen_pdo_ctrl[u8_node_idx].u8_tpdo_timeout_ctr[u8_pdo_idx] = gas_canopen_pdo_ctrl[u8_node_idx].u8_tpdo_timeout_value[u8_pdo_idx];

						*pe_canopen_exchange_status = E_CANOPEN_TRANSFER_COMPLETE;
						*pe_frame_processed = E_CANOPEN_FRAME_PROCESSED_WITH_DATA;
					}
				}
			}
		} else {
			/* Not an PDO frame */
			/* No data processed */
			*pe_frame_processed = E_CANOPEN_FRAME_NOT_PROCESSED;
		}

		t_Process_Frames_Status = E_OK;
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
 * \fn  Std_ReturnType ProCan_PdoManage_CheckTimeout(const uint8 u8_canopen_node_idx, const uint8 u8_canopen_tpdo_idx, te_CanopenStatus * const pe_canopen_exchange_status)
 * \brief Check server response time-out
 *
 * \param <uint8 u8_canopen_node_idx> {the node index on the CAN network on which the timeout must be checked (0..gu8_canopen_nodes_nr - 1)}
 * \param <uint8 u8_canopen_tpdo_idx> {the TPDO index of the CAN node on which the CANopen command must be launched (0..K_CANOPEN_PDO_MAX_NR - 1)}
 * \param <te_CanopenStatus *pe_canopen_exchange_status> {status of the CANopen command:
 *	   				- E_CANOPEN_TRANSFER_IN_PROGRESS: no time-out on receive PDOs
 *	   				- E_CANOPEN_TIMEOUT_ERROR: server response time-out occurred on a receive PDO}
 *
 * \return Std_ReturnType :
 *         - E_OK if command is successful
 *         - E_NOT_OK if not
 */
Std_ReturnType ProCan_PdoManage_CheckTimeout(const uint8 u8_canopen_node_idx, const uint8 u8_canopen_tpdo_idx, te_CanopenStatus * const pe_canopen_exchange_status)
{
    /*! Declaration */
    Std_ReturnType t_Check_PdoTimeout_Status;

    /*! Initialization */
    t_Check_PdoTimeout_Status = E_NOT_OK;

    /*! Function Body */
    if ((E_OK == gt_CfgCanOpen_PdoManage_Status) &&
    	(NULL_PTR != pe_canopen_exchange_status) &&
		(u8_canopen_node_idx < gu8_canopen_nodes_nr))
    {
		if (K_CANOPEN_FREE_PDO_SLOT != gas_canopen_pdo_ctrl[u8_canopen_node_idx].u16_tpdo_canid[u8_canopen_tpdo_idx])
		{
				/* Update the time-out timer of the TPDO */
				gas_canopen_pdo_ctrl[u8_canopen_node_idx].u8_tpdo_timeout_ctr[u8_canopen_tpdo_idx]--;
				if (gas_canopen_pdo_ctrl[u8_canopen_node_idx].u8_tpdo_timeout_ctr[u8_canopen_tpdo_idx] == 0)
				{
					/* Time-out occurred */
					*pe_canopen_exchange_status = E_CANOPEN_TIMEOUT_ERROR;
				}
				else
				{
					/* No time-out, transfer in progress */
					*pe_canopen_exchange_status = E_CANOPEN_TRANSFER_IN_PROGRESS;
				}
		}
		else
		{
			/* Free TPDO slot: no timeout check */
			*pe_canopen_exchange_status = E_CANOPEN_TRANSFER_COMPLETE;
		}

		t_Check_PdoTimeout_Status = E_OK;
    }
    else
    {
    	/* Module is not initialized, null pointer encountered or CANopen node index is out of range */
	;
    }

    /*! return code */
    return(t_Check_PdoTimeout_Status);
}

/* Private functions 							      */
/*============================================================================*/
