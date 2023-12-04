/*============================================================================*/
/*                        	ROBOSOFT                                      */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*/
/*! 
 * $Source: Library_PrivateCanOpenNmtManage.c
 * $Author: BRobin 
 * $Date: 2015/12/08  
 */
/*============================================================================*/
/* DESCRIPTION :                                                              */
/** \file  Library_PrivateCanOpenNmtManage.c
  * \brief Fichier Source du Service PrivateCanOpenNmtManage
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
#include "Library_PrivateCanOpenNmtManage.h"
#include "Library_PrivateCanOpenComWrapper.h"
#include <stdio.h>
#include <string.h>

/* Constants and types  						      */
/*============================================================================*/ 
#define K_DEBUG

/**************************************/
/* CANopen general */

/* Broadcast node identifier */
#define K_CANOPEN_BROADCAST_NODE_ID			0

/* CANopen node identifier mask within COB-ID */
#define K_CANOPEN_NODE_ID_FROM_CANID		0x07F

/* CANopen function code mask within COB-ID */
#define K_CANOPEN_FUNC_CODE_FROM_CANID		0x780


/**************************************/
/* NMT management */

/* NMT state control request CAN identifier */
#define K_CANOPEN_NMT_STATE_CTRL_CANID		0x000

/* Size of NMT state control request */
#define K_CANOPEN_NMT_STATE_CTRL_FRAME_DLC	2

/* NMT state request CAN identifier */
#define K_CANOPEN_NMT_STATE_REQ_CANID		0x700

/* Size of NMT state request */
#define K_CANOPEN_NMT_STATE_REQ_FRAME_DLC	0

/* Size of NMT state response */
#define K_CANOPEN_NMT_STATE_RESP_FRAME_DLC	1

/* NMT state byte values */
#define K_CANOPEN_NMT_STATE_BOOTUP		0x00
#define K_CANOPEN_NMT_STATE_STOPPED		0x04
#define K_CANOPEN_NMT_STATE_OPE			0x05
#define K_CANOPEN_NMT_STATE_PREOPE		0x7F

/* NMT toggle bit/state byte masks */
#define K_CANOPEN_NMT_STATE_TOGGLE_BIT_MASK	0x80
#define K_CANOPEN_NMT_STATE_STATE_MASK		0x7F


/**************************************/
/* NMT controller */

/* State of a CANopen NMT exchange */
typedef enum {
	E_NMT_IDLE = 0,
	E_NMT_STATE_REQ_SENT = 1,
} te_CanopenNmtState;

/* Definition of a CANopen NMT controller */
typedef struct
{
	/* Node identifier */
	uint8 u8_canopen_node_id;

	/* NMT communication controller state */
	te_CanopenNmtState	e_nmt_state;

	/* Node guarding (NMT state request) toggle bit state */
	uint8 u8_canopen_node_guarding_toggle_bit;
	/* Indicate that the toggle bit state is not known yet */
	bool b_canopen_node_guarding_toggle_bit_undef;

	/* For response time-out checking: time since NMT request emission */
	uint8	u8_nmt_timeout_value;
	uint8	u8_nmt_timeout_ctr;
} ts_CanopenNmtController;


/* Variables 								      */
/*============================================================================*/ 
static Std_ReturnType gt_InitCanOpen_NmtManage_Status = E_NOT_OK; /* Global Initialization variable */
static Std_ReturnType gt_CfgCanOpen_NmtManage_Status = E_NOT_OK; /* Global Configuration variable */


/* Current states of the CANopen protocol exchanges on the CAN networks */
static ts_CanopenNmtController gas_canopen_nmt_ctrl[K_CANOPEN_NODES_NR];
static uint8 gu8_canopen_nodes_nr;

/* Private functions prototypes 					      */
/*============================================================================*/ 

static Std_ReturnType canopen_nmt_get_node_state(ts_CanopenNmtController * const ps_canopen_nmt_ctrl, const ts_CanopenCanFrame * const ps_canopen_frame,
		ts_CanopenData * const ps_canopen_data, te_CanopenStatus * const pe_canopen_exchange_status, te_CanopenFrameProcessStatus * const pe_frame_processed);


/* Inline functions 							      */
/*============================================================================*/

/* Public functions 							      */
/*============================================================================*/


/**
 * \fn  Std_ReturnType ProCan_NmtManage_Initialize(void)
 * \brief CANopen library initialization
 *
 * \param  void
 *
 * \return Std_ReturnType :
 *         - E_OK if module is initialized
 *         - E_NOT_OK if not
 */
Std_ReturnType ProCan_NmtManage_Initialize(void)
{
    /*! Declaration */

    /*! Initialization */

    /*! Function Body */
	gu8_canopen_nodes_nr = 0;
	gt_InitCanOpen_NmtManage_Status = E_OK;

    /*! return code */
    return(gt_InitCanOpen_NmtManage_Status);
}

/**
 * \fn  Std_ReturnType ProCan_NmtManage_Configure(const ts_CanopenCfg * const ps_canopen_cfg)
 * \brief CANopen ports configuration
 *
 * \param <ts_CanopenCfg *ps_canopen_cfg> { CANopen library configuration }
 *
 * \return Std_ReturnType :
 *         - E_OK if module is initialized and configuration is successful
 *         - E_NOT_OK if not
 */
Std_ReturnType ProCan_NmtManage_Configure(const ts_CanopenCfg * const ps_canopen_cfg)
{
    /*! Declaration */
	uint8 u8_node_idx;

    /*! Initialization */
	gt_CfgCanOpen_NmtManage_Status = E_NOT_OK;

    /*! Function Body */
	if ((E_OK == gt_InitCanOpen_NmtManage_Status) && (NULL_PTR != ps_canopen_cfg))
	{
		if (ps_canopen_cfg->u8_canopen_nodes_nr_cfg <= K_CANOPEN_NODES_NR)
		{
			/* Write the new CANopen configuration */
			gu8_canopen_nodes_nr = ps_canopen_cfg->u8_canopen_nodes_nr_cfg;
			for (u8_node_idx = 0; u8_node_idx < gu8_canopen_nodes_nr; u8_node_idx++)
			{
				/* CANopen node identifiers */
				gas_canopen_nmt_ctrl[u8_node_idx].u8_canopen_node_id = ps_canopen_cfg->as_canopen_ports_cfg[u8_node_idx].u8_canopen_node_id_cfg;

				/* Slave response time-out */
				gas_canopen_nmt_ctrl[u8_node_idx].u8_nmt_timeout_value = ps_canopen_cfg->as_canopen_ports_cfg[u8_node_idx].u8_canopen_nmt_timeout_cfg;

				/* Initialize the NMT controllers */
				gas_canopen_nmt_ctrl[u8_node_idx].e_nmt_state = E_NMT_IDLE;
				gas_canopen_nmt_ctrl[u8_node_idx].u8_canopen_node_guarding_toggle_bit = 0;
				gas_canopen_nmt_ctrl[u8_node_idx].b_canopen_node_guarding_toggle_bit_undef = K_TRUE;
			}

			gt_CfgCanOpen_NmtManage_Status = E_OK;
		} else {
			/* Invalid number of CANopen nodes */
		}
	} else {
		/* Module is not initialized, Halt */
		;
	}

    /*! return code */
    return(gt_CfgCanOpen_NmtManage_Status);
}


/**
 * \fn  Std_ReturnType ProCan_NmtManage_StateControl(const uint8 u8_canopen_node_idx, const te_CanopenNmtCommands e_canopen_nmt_cmd, te_CanopenStatus * const pe_canopen_exchange_status)
 * \brief Control the communication state of network nodes
 *
 * \param  <uint8 u8_canopen_node_idx> {the node index on the CAN network on which the CANopen command must be launched (0..gu8_canopen_nodes_nr - 1) or the broadcast value}
 * \param  <te_CanopenNmtCommands e_canopen_nmt_cmd> {the requested CANopen NMT command}
 * \param  <te_CanopenStatus * pe_canopen_exchange_status> {status of the CANopen command:
 *  				   	- E_CANOPEN_TRANSFER_COMPLETE: NMT command successfully sent
 *     					- E_CANOPEN_NETWORK_INTERFACE_ERROR: general error received from the underlying communication driver }
 *
 * \return Std_ReturnType :
 *         - E_OK if command is successful
 *         - E_NOT_OK if not
 */
Std_ReturnType ProCan_NmtManage_StateControl(const uint8 u8_canopen_node_idx, const te_CanopenNmtCommands e_canopen_nmt_cmd, te_CanopenStatus * const pe_canopen_exchange_status)
{
    /*! Declaration */
    Std_ReturnType t_Send_NMTCtrl_Status;
	/* Status from the write service called */
    te_CanopenWrapperStatus e_can_status;
	/* Data exchanged with the underlying communication layer */
    ts_CanopenCanFrame s_canopen_frame;

    /*! Initialization */
    t_Send_NMTCtrl_Status = E_NOT_OK;
    e_can_status = E_CANOPENWRP_NO_DATA;

    /*! Function Body */
    if ((E_OK == gt_CfgCanOpen_NmtManage_Status) && ((u8_canopen_node_idx < gu8_canopen_nodes_nr) || (K_CANOPEN_NMT_BROADCAST_ID == u8_canopen_node_idx)) && (NULL_PTR != pe_canopen_exchange_status))
    {
    	/* Build NMT control nodes frame */
    	s_canopen_frame.e_can_frame_type = E_CANOPEN_NORMAL_FRAME;
    	s_canopen_frame.u32_can_id = K_CANOPEN_NMT_STATE_CTRL_CANID;

    	/* The CAN data payload must contain 2 bytes */
    	/* | Byte 0: Requested state | Byte 1: Addressed node (0 = broadcast to all nodes) | */
    	switch(e_canopen_nmt_cmd)
    	{
    	case E_CANOPEN_NMT_START:
    		/* Go to NMT operational state */

    	case E_CANOPEN_NMT_STOP:
			/* Go to NMT stopped state */

    	case E_CANOPEN_NMT_PREOP:
    		/* Go to NMT pre-operational state */

    	case E_CANOPEN_NMT_RESET_NODE:
    		/* Go to NMT reset node state */

    	case E_CANOPEN_NMT_RESET_COM:
    		/* Go to NMT reset communication state */

    		s_canopen_frame.au8_can_data[0] = e_canopen_nmt_cmd;
    		t_Send_NMTCtrl_Status = E_OK;
    		break;

    	default:
#ifdef K_DEBUG
    		printf("ProCan_Send_NMTStateRequest: invalid NMT requested state (%d)\n", e_canopen_nmt_cmd);
#endif
    		break;
    	}

    	if (K_CANOPEN_NMT_BROADCAST_ID == u8_canopen_node_idx)
    	{
    		/* Broadcast the request to all nodes */
    	    s_canopen_frame.au8_can_data[1] = K_CANOPEN_BROADCAST_NODE_ID;
    	} else {
    		/* Specific node */
    		s_canopen_frame.au8_can_data[1] = gas_canopen_nmt_ctrl[u8_canopen_node_idx].u8_canopen_node_id;
    	}
    	s_canopen_frame.u8_can_dlc = K_CANOPEN_NMT_STATE_CTRL_FRAME_DLC;

    	/* Send the message */
    	t_Send_NMTCtrl_Status = ComWrapperCanOpen_Send(&s_canopen_frame, &e_can_status);

		if ((E_OK == t_Send_NMTCtrl_Status) && (E_CANOPENWRP_DATA_OK == e_can_status))
		{
			/* Request correctly sent */
			*pe_canopen_exchange_status = E_CANOPEN_TRANSFER_COMPLETE;
		} else if ((E_OK == t_Send_NMTCtrl_Status) && (E_CANOPENWRP_DATA_OK != e_can_status))
		{
			/* Low level driver error */
			*pe_canopen_exchange_status = E_CANOPEN_NETWORK_INTERFACE_ERROR;
		} else {
			/* Internal driver error */
		}
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
 * \fn  Std_ReturnType ProCan_NmtManage_StateRequest(const uint8 u8_canopen_node_idx, te_CanopenStatus * const pe_canopen_exchange_status)
 * \brief Control the state of network nodes
 *
 * \param  <uint8 u8_canopen_node_idx> {the node index on the CAN network on which the CANopen command must be launched (0..gu8_canopen_nodes_nr - 1)}
 * \param  <te_CanopenStatus * pe_canopen_exchange_status> {status of the CANopen command:
 *  				   	- E_CANOPEN_TRANSFER_IN_PROGRESS: NMT command successfully sent
 *     					- E_CANOPEN_NETWORK_INTERFACE_ERROR: general error received from the underlying communication driver }
 *
 * \return Std_ReturnType :
 *         - E_OK if command is successful
 *         - E_NOT_OK if not
 */
Std_ReturnType ProCan_NmtManage_StateRequest(const uint8 u8_canopen_node_idx, te_CanopenStatus * const pe_canopen_exchange_status)
{
    /*! Declaration */
    Std_ReturnType t_Send_NMTRequest_Status;
	/* Status from the write service called */
    te_CanopenWrapperStatus e_can_status;
	/* Data exchanged with the underlying communication layer */
    ts_CanopenCanFrame s_canopen_frame;
    e_can_status = E_CANOPENWRP_NO_DATA;

    /*! Initialization */
    t_Send_NMTRequest_Status = E_NOT_OK;

    /*! Function Body */
    if ((E_OK == gt_CfgCanOpen_NmtManage_Status) && (u8_canopen_node_idx < gu8_canopen_nodes_nr) && (NULL_PTR != pe_canopen_exchange_status))
    {
    	if (E_NMT_IDLE == gas_canopen_nmt_ctrl[u8_canopen_node_idx].e_nmt_state)
		{
			/* Build NMT control node state frame */
			s_canopen_frame.e_can_frame_type = E_CANOPEN_RTR_FRAME;
			s_canopen_frame.u32_can_id = K_CANOPEN_NMT_STATE_REQ_CANID | gas_canopen_nmt_ctrl[u8_canopen_node_idx].u8_canopen_node_id;

			/* No CAN data payload (Remote transmission request) */
			s_canopen_frame.u8_can_dlc = K_CANOPEN_NMT_STATE_REQ_FRAME_DLC;

			/* Send the message */
			t_Send_NMTRequest_Status = ComWrapperCanOpen_Send(&s_canopen_frame, &e_can_status);

			if ((E_OK == t_Send_NMTRequest_Status) && (E_CANOPENWRP_DATA_OK == e_can_status))
			{
				/* Request correctly sent */
				gas_canopen_nmt_ctrl[u8_canopen_node_idx].e_nmt_state = E_NMT_STATE_REQ_SENT;

				/* Reset the time-out counter */
				gas_canopen_nmt_ctrl[u8_canopen_node_idx].u8_nmt_timeout_ctr = gas_canopen_nmt_ctrl[u8_canopen_node_idx].u8_nmt_timeout_value;

				*pe_canopen_exchange_status = E_CANOPEN_TRANSFER_COMPLETE;
			}
			else
			{
				if ((E_OK == t_Send_NMTRequest_Status) &&
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
			/* Request already sent */
			*pe_canopen_exchange_status = E_CANOPEN_TRANSFER_COMPLETE;
			t_Send_NMTRequest_Status = E_OK;
		}
	}
    else
    {
	/* Module is not initialized, null pointer encountered, CANopen node index is out of range or CANopen NMT controller not in idle state */
	;
    }

    /*! return code */
    return(t_Send_NMTRequest_Status);
}


/**
 * \fn  Std_ReturnType ProCan_NmtManage_ProcessFrame(const ts_CanopenCanFrame * const ps_canopen_can_frame, uint8 * const u8_canopen_node_idx,
 * 						ts_CanopenData * const ps_canopen_data, te_CanopenStatus * const pe_canopen_exchange_status, te_CanopenFrameProcessStatus * const pe_frame_processed)
 * \brief Process received CANopen frames
 *
 * \param <ts_CanopenCanFrame *ps_canopen_frame> {the CAN frame to decode}
 * \param <uint8 *u8_canopen_node_idx> {the node index on the CAN network from which transfer is complete or got a protocol error}
 * \param <ts_CanopenData *ps_canopen_data> {the buffer where to store the received NMT state (when return code is E_CANOPEN_TRANSFER_COMPLETE)}
 * \param <te_CanopenStatus *pe_canopen_exchange_status> {status of the CANopen command:
 *	   				- E_CANOPEN_TRANSFER_COMPLETE: NMT request successful
 *     				- E_CANOPEN_TRANSFER_ERROR: invalid toggle bit in the slave's NMT request response}
 * \param <te_CanopenFrameProcessStatus *pe_frame_processed> {the status of the CANopen frame processing}
 *
 * \return Std_ReturnType :
 *         - E_OK if command is successful
 *         - E_NOT_OK if not
 */
Std_ReturnType ProCan_NmtManage_ProcessFrame(const ts_CanopenCanFrame * const ps_canopen_can_frame, uint8 * const u8_canopen_node_idx,
		ts_CanopenData * const ps_canopen_data, te_CanopenStatus * const pe_canopen_exchange_status, te_CanopenFrameProcessStatus * const pe_frame_processed)
{
    /*! Declaration */
    Std_ReturnType t_Process_Frames_Status;
	/* Server node identifier */
	uint8 u8_server_id;
	/* Node index of the received frame */
	uint8 u8_node_idx;

    /*! Initialization */
	t_Process_Frames_Status = E_NOT_OK;

    /*! Function Body */
    if ((E_OK == gt_CfgCanOpen_NmtManage_Status) && (NULL_PTR != ps_canopen_data) && (NULL_PTR != u8_canopen_node_idx) && (NULL_PTR != pe_canopen_exchange_status))
    {
		/* Check if the CAN identifier is an NMT slave response */
		/* COB-ID = 11-bit CAN identifier = 111 0|slave identifier */
		if ((E_CANOPEN_NORMAL_FRAME == ps_canopen_can_frame->e_can_frame_type)
				&& (K_CANOPEN_NMT_STATE_REQ_CANID == (ps_canopen_can_frame->u32_can_id & K_CANOPEN_FUNC_CODE_FROM_CANID)) && (K_CANOPEN_NMT_STATE_RESP_FRAME_DLC == ps_canopen_can_frame->u8_can_dlc))
		{
			/* Retrieve the CANopen node index from the CANopen node identifier */
			u8_server_id = ps_canopen_can_frame->u32_can_id & K_CANOPEN_NODE_ID_FROM_CANID;
			for (u8_node_idx = 0; (u8_node_idx < gu8_canopen_nodes_nr) && (gas_canopen_nmt_ctrl[u8_node_idx].u8_canopen_node_id != u8_server_id); u8_node_idx++)
			{
				/* Check the next node index */
			}
			if (u8_node_idx < gu8_canopen_nodes_nr)
			{
				/* Server found: process the received frame */
				*u8_canopen_node_idx = u8_node_idx;

				/* Process NMT frame */
				t_Process_Frames_Status = canopen_nmt_get_node_state(&gas_canopen_nmt_ctrl[u8_node_idx], ps_canopen_can_frame, ps_canopen_data,
						pe_canopen_exchange_status, pe_frame_processed);
			} else {
				/* Frame is from a unmanaged server node: ignore it */
				/* NMT frame processed without data */
				*pe_frame_processed = E_CANOPEN_FRAME_PROCESSED_NO_DATA;
				t_Process_Frames_Status = E_OK;
			}
		} else {
			/* Not an NMT frame */
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
 * \fn  Std_ReturnType ProCan_NmtManage_CheckTimeout(const uint8 u8_canopen_node_idx, te_CanopenStatus * const pe_canopen_exchange_status)
 * \brief Check slave response time-out
 *
 * \param <uint8 u8_canopen_node_idx> {the node index on the CAN network on which the CANopen command must be launched (0..gu8_canopen_nodes_nr - 1)}
 * \param <te_CanopenStatus *pe_canopen_exchange_status> {status of the CANopen command:
 *	   				- E_CANOPEN_TRANSFER_IN_PROGRESS: NMT state request is on-going
 *	   				- E_CANOPEN_TRANSFER_COMPLETE: NMT state request complete or no NMT state request is on-going
 *	   				- E_CANOPEN_TIMEOUT_ERROR: slave response time-out occurred}
 *
 * \return Std_ReturnType :
 *         - E_OK if command is successful
 *         - E_NOT_OK if not
 */
Std_ReturnType ProCan_NmtManage_CheckTimeout(const uint8 u8_canopen_node_idx, te_CanopenStatus * const pe_canopen_exchange_status)
{
    /*! Declaration */
    Std_ReturnType t_Check_NmtTimeout_Status;

    /*! Initialization */
	t_Check_NmtTimeout_Status = E_NOT_OK;

    /*! Function Body */
    if ((E_OK == gt_CfgCanOpen_NmtManage_Status) && (NULL_PTR != pe_canopen_exchange_status) && (u8_canopen_node_idx < gu8_canopen_nodes_nr))
    {
		if (E_NMT_IDLE != gas_canopen_nmt_ctrl[u8_canopen_node_idx].e_nmt_state)
		{
			/* NMT state request is on-going: check the timer */
			/* Update the time-out timer */
			gas_canopen_nmt_ctrl[u8_canopen_node_idx].u8_nmt_timeout_ctr--;
			if (gas_canopen_nmt_ctrl[u8_canopen_node_idx].u8_nmt_timeout_ctr == 0)
			{
				/* Time-out occurred: abort the transfer */
				gas_canopen_nmt_ctrl[u8_canopen_node_idx].e_nmt_state = E_NMT_IDLE;
				*pe_canopen_exchange_status = E_CANOPEN_TIMEOUT_ERROR;
			} else {
				/* No time-out, transfer in progress */
				*pe_canopen_exchange_status = E_CANOPEN_TRANSFER_IN_PROGRESS;
			}
		} else {
			/* Transfer complete or no on-going transfer */
			*pe_canopen_exchange_status = E_CANOPEN_TRANSFER_COMPLETE;
		}

		t_Check_NmtTimeout_Status = E_OK;
    }
    else
    {
    	/* Module is not initialized, null pointer encountered or CANopen node index is out of range */
	;
    }

    /*! return code */
    return(t_Check_NmtTimeout_Status);
}

/* Private functions 							      */
/*============================================================================*/

/* Updates the CANopen NMT controller and get the node state according to the received NMT slave response
 * Inputs:
 *     - ps_canopen_frame: the received frame
 *
 * Inputs/Outputs:
 *     - ps_canopen_nmt_ctrl: the CANopen NMT controller
 *
 * Outputs:
 *     - ps_canopen_data: the received NMT state
 *     - pe_canopen_exchange_status: the status of the CANopen command:
 *	   				- E_CANOPEN_TRANSFER_COMPLETE: NMT request successful
 *     				- E_CANOPEN_TRANSFER_ERROR: invalid toggle bit in the slave's NMT request response
 *     - pe_frame_processed: the status of the CANopen frame processing
 *
 * Return code :
 *    - E_OK if command is successful
 *    - E_NOT_OK if not
 *
 */
static Std_ReturnType canopen_nmt_get_node_state(ts_CanopenNmtController * const ps_canopen_nmt_ctrl, const ts_CanopenCanFrame * const ps_canopen_frame,
		ts_CanopenData * const ps_canopen_data, te_CanopenStatus * const pe_canopen_exchange_status, te_CanopenFrameProcessStatus * const pe_frame_processed)
{
	Std_ReturnType t_returned_status = E_OK;

	switch(ps_canopen_nmt_ctrl->e_nmt_state)
	{
	case E_NMT_STATE_REQ_SENT:
		/* NMT state request response received */
		ps_canopen_data->e_data_kind = E_CANOPEN_NMT_STATE;
		/* Check the toggle bit */
		if ((K_FALSE != ps_canopen_nmt_ctrl->b_canopen_node_guarding_toggle_bit_undef) || (ps_canopen_nmt_ctrl->u8_canopen_node_guarding_toggle_bit == (ps_canopen_frame->au8_can_data[0] & K_CANOPEN_NMT_STATE_TOGGLE_BIT_MASK)))
		{
			/* Compute the node state */
			switch(ps_canopen_frame->au8_can_data[0] & K_CANOPEN_NMT_STATE_STATE_MASK)
			{
			case K_CANOPEN_NMT_STATE_BOOTUP:
				/* Boot-up state */
				ps_canopen_data->e_canopen_state = E_CANOPEN_NMT_BOOTUP;
				break;

			case K_CANOPEN_NMT_STATE_STOPPED:
				/* Stopped state */
				ps_canopen_data->e_canopen_state = E_CANOPEN_NMT_STOPPED;
				break;

			case K_CANOPEN_NMT_STATE_OPE:
				/* Operational state */
				ps_canopen_data->e_canopen_state = E_CANOPEN_NMT_OPERATIONAL;
				break;

			case K_CANOPEN_NMT_STATE_PREOPE:
				/* Pre-operational state */
				ps_canopen_data->e_canopen_state = E_CANOPEN_NMT_PREOPE;
				break;

			default:
				/* Undefined state */
				ps_canopen_data->e_canopen_state = E_CANOPEN_NMT_UNDEF;
				break;
			}

			if (K_FALSE != ps_canopen_nmt_ctrl->b_canopen_node_guarding_toggle_bit_undef)
			{
				/* Initialize the toggle bit with the one received */
				ps_canopen_nmt_ctrl->u8_canopen_node_guarding_toggle_bit = ps_canopen_frame->au8_can_data[0] & K_CANOPEN_NMT_STATE_TOGGLE_BIT_MASK;
				ps_canopen_nmt_ctrl->b_canopen_node_guarding_toggle_bit_undef = K_TRUE;
			}

			/* NMT state request complete */
			*pe_canopen_exchange_status = E_CANOPEN_TRANSFER_COMPLETE;
		} else {
			/* NMT state request toggle bit error */
			*pe_canopen_exchange_status = E_CANOPEN_TRANSFER_ERROR;
		}

		/* Toggle the next request bit */
		ps_canopen_nmt_ctrl->u8_canopen_node_guarding_toggle_bit ^= K_CANOPEN_NMT_STATE_TOGGLE_BIT_MASK;

		/* Set the controller to idle state */
		ps_canopen_nmt_ctrl->e_nmt_state = E_NMT_IDLE;

		/* Frame processed with data */
		*pe_frame_processed = E_CANOPEN_FRAME_PROCESSED_WITH_DATA;
		break;

	case E_NMT_IDLE:
#ifdef K_DEBUG
		/* Frame not expected: log a warning */
		printf("canopen_nmt_get_node_state: warning: NMT state frame received in an unexpected CANopen driver idle state\n");
#endif
		/* Frame processed without data */
		*pe_frame_processed = E_CANOPEN_FRAME_PROCESSED_NO_DATA;
		break;

	default:
#ifdef K_DEBUG
		printf("canopen_nmt_get_node_state: invalid CANopen NMT controller state (%d)\n", ps_canopen_nmt_ctrl->e_nmt_state);
#endif
		t_returned_status = E_NOT_OK;
		break;
	}

	return t_returned_status;
}
