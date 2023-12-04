/*============================================================================*/
/*                        	ROBOSOFT                                      */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*/
/*! 
 * $Source: Library_PrivateCanOpenSyncManage.c
 * $Author: BRobin 
 * $Date: 2015/12/08  
 */
/*============================================================================*/
/* DESCRIPTION :                                                              */
/** \file  Library_PrivateCanOpenSyncManage.c
  * \brief Fichier Source du Service PrivateCanOpenSyncManage
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
#include "Library_PrivateCanOpenSyncManage.h"
#include "Library_PrivateCanOpenComWrapper.h"
#include <stdio.h>
#include <string.h>

/* Constants and types  						      */
/*============================================================================*/ 

/**************************************/
/* CANopen general */


/**************************************/
/* SYNC management */

/* SYNC request CAN identifier */
#define K_CANOPEN_SYNC_STATE_CTRL_CANID		0x080

/* Size of SYNC request */
#define K_CANOPEN_SYNC_STATE_CTRL_FRAME_DLC	0


/* Variables 								      */
/*============================================================================*/ 
static Std_ReturnType gt_InitCanOpen_SyncManage_Status = E_NOT_OK; /* Global Initialization variable */
static Std_ReturnType gt_CfgCanOpen_SyncManage_Status = E_NOT_OK; /* Global Configuration variable */


/* Private functions prototypes 					      */
/*============================================================================*/ 


/* Inline functions 							      */
/*============================================================================*/

/* Public functions 							      */
/*============================================================================*/


/**
 * \fn  Std_ReturnType ProCan_SyncManage_Initialize(void)
 * \brief CANopen library initialization
 *
 * \param  void
 *
 * \return Std_ReturnType :
 *         - E_OK if module is initialized
 *         - E_NOT_OK if not
 */
Std_ReturnType ProCan_SyncManage_Initialize(void)
{
    /*! Declaration */

    /*! Initialization */

    /*! Function Body */
	gt_InitCanOpen_SyncManage_Status = E_OK;

    /*! return code */
    return(gt_InitCanOpen_SyncManage_Status);
}

/**
 * \fn  Std_ReturnType ProCan_SyncManage_Configure(void)
 * \brief CANopen ports configuration
 *
 * \param <ts_CanopenCfg *ps_canopen_cfg> { CANopen library configuration }
 *
 * \return Std_ReturnType :
 *         - E_OK if module is initialized and configuration is successful
 *         - E_NOT_OK if not
 */
Std_ReturnType ProCan_SyncManage_Configure(const ts_CanopenCfg * const ps_canopen_cfg)
{
    /*! Declaration */

    /*! Initialization */

    /*! Function Body */
	if ((E_OK == gt_InitCanOpen_SyncManage_Status) && (NULL_PTR != ps_canopen_cfg))
	{
		gt_CfgCanOpen_SyncManage_Status = E_OK;
	}
	else
	{
		/* Module is not initialized, Halt */
		gt_CfgCanOpen_SyncManage_Status = E_NOT_OK;
	}

    /*! return code */
    return(gt_CfgCanOpen_SyncManage_Status);
}


/**
 * \fn  Std_ReturnType ProCan_SyncManage_Send(te_CanopenStatus * const pe_canopen_exchange_status)
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
Std_ReturnType ProCan_SyncManage_Send(te_CanopenStatus * const pe_canopen_exchange_status)
{
    /*! Declaration */
    Std_ReturnType t_Send_Status;
	/* Status from the write service called */
    te_CanopenWrapperStatus e_can_status;
	/* Data exchanged with the underlying communication layer */
    ts_CanopenCanFrame s_canopen_frame;

    /*! Initialization */
    e_can_status = E_CANOPENWRP_NO_DATA;

    /*! Function Body */
    if ((E_OK == gt_CfgCanOpen_SyncManage_Status) && (NULL_PTR != pe_canopen_exchange_status))
    {
    	/* Build NMT control nodes frame */
    	s_canopen_frame.e_can_frame_type = E_CANOPEN_RTR_FRAME;
    	s_canopen_frame.u32_can_id = K_CANOPEN_SYNC_STATE_CTRL_CANID;

    	/* The CAN data payload must contain no data */
    	s_canopen_frame.u8_can_dlc = K_CANOPEN_SYNC_STATE_CTRL_FRAME_DLC;

    	/* Send the message */
    	t_Send_Status = ComWrapperCanOpen_Send(&s_canopen_frame, &e_can_status);

		if ((E_OK == t_Send_Status) && (E_CANOPENWRP_DATA_OK == e_can_status))
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
        t_Send_Status = E_NOT_OK;
    }

    /*! return code */
    return(t_Send_Status);
}

/* Private functions 							      */
/*============================================================================*/
