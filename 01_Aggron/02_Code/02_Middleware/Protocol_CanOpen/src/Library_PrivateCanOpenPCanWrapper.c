/*============================================================================*/
/*                        	ROBOSOFT                                      */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*/
/*! 
 * $Source: Library_PCanWrapperCanOpen.c
 * $Author: BRobin 
 * $Date: 2015/12/08  
 */
/*============================================================================*/
/* DESCRIPTION :                                                              */
/** \file  Library_PCanWrapperCanOpen.c
  * \brief Fichier Source du Service PCanWrapperCanOpen
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
#include "Library_PrivateCanOpenPCanWrapper.h"
#include "Driver_PCanDriver.h"
#include <stdio.h>
#include <string.h>

/* Constants and types  						      */
/*============================================================================*/ 
#define K_DEBUG
   
/* Variables 								      */
/*============================================================================*/ 
static Std_ReturnType gt_InitPCanWrapperCanOpen_Status = E_NOT_OK; /* Global Initialization variable */
static Std_ReturnType gt_CfgPCanWrapperCanOpen_Status = E_NOT_OK; /* Global Initialization variable */

/* PCan handler used to communicate with XtrapulsPac motors */
static t_CanDrvHandler gt_can_socket;

/* Private functions prototypes 					      */
/*============================================================================*/ 

/* Inline functions 							      */
/*============================================================================*/

/* Public functions 							      */
/*============================================================================*/

/**
 * \fn  Std_ReturnType PCanWrapperCanOpen_Init(void)
 * \brief PCan driver wrapper initialization
 *
 * \param  void
 *
 * \return Std_ReturnType :
 *         - E_OK if command is successful
 *         - E_NOT_OK if not
 */
Std_ReturnType PCanWrapperCanOpen_Init(void)
{
    /*! Declaration */

    /*! Initialization */
	gt_can_socket = K_DRVPCAN_CLOSED_HANDLE;

	gt_InitPCanWrapperCanOpen_Status = E_OK;

    /*! return code */
	return gt_InitPCanWrapperCanOpen_Status;
}


/**
 * \fn  Std_ReturnType PCanWrapperCanOpen_Configure(const uint8 * const pu8_canopen_pcan_ifname_cfg)
 * \brief PCan driver wrapper CANopen configuration
 *
 * \param <uint8 *pu8_canopen_pcan_ifname_cfg> { PCAN driver interface name }
 *
 * \return Std_ReturnType :
 *         - E_OK if command is successful
 *         - E_NOT_OK if not
 */
Std_ReturnType PCanWrapperCanOpen_Configure(const uint8 * const pu8_canopen_pcan_ifname_cfg)
{
    /*! Declaration */

    /*! Initialization */
    gt_CfgPCanWrapperCanOpen_Status = E_NOT_OK;

	if ((E_OK == gt_InitPCanWrapperCanOpen_Status) && (NULL_PTR != pu8_canopen_pcan_ifname_cfg))
	{
		gt_CfgPCanWrapperCanOpen_Status = DrvPCan_Open(pu8_canopen_pcan_ifname_cfg, &gt_can_socket);
	}
	else
    {
    	/* Module is not initialized or error on a function parameter, Halt */
    }

    /*! return code */
	return gt_CfgPCanWrapperCanOpen_Status;
}


/**
 * \fn  Std_ReturnType PCanWrapperCanOpen_Read(ts_CanopenCanFrame * const ps_can_frame, te_CanopenWrapperStatus * const pe_can_data_read_status)
 * \brief Receive data from a CAN network with PCan driver
 *
 * \param <ts_CanopenCanFrame *ps_can_frame> { the received CAN id and CAN data }
 * \param <te_CanopenWrapperStatus *pe_can_data_read_status>  { E_CANOPENWRP_DATA_OK if a frame was received, E_CANOPENWRP_NO_DATA if no frame was received,
 * 																E_CANOPENWRP_DRV_ERR if PCan driver error received }
 *
 * \return Std_ReturnType :
 *         - E_OK if command is successful
 *         - E_NOT_OK if not
 */
Std_ReturnType PCanWrapperCanOpen_Read(ts_CanopenCanFrame * const ps_can_frame, te_CanopenWrapperStatus * const pe_can_data_read_status)
{
    /*! Declaration */     
    Std_ReturnType t_Read_Status;
	/* Status of the PCan read operation */
    bool b_can_frame_avail;
	/* Frame reception */
    ts_PCanFrame s_can_frame;

    /*! Initialization */
    t_Read_Status = E_NOT_OK;
    b_can_frame_avail = K_FALSE;
    s_can_frame.u8_can_dlc=0;

    /*! Function Body */
    if ((E_OK == gt_CfgPCanWrapperCanOpen_Status) && (NULL_PTR != ps_can_frame) && (NULL_PTR != pe_can_data_read_status))
    {
    	/* Read a frame from the PCan driver */
    	t_Read_Status = DrvPCan_Receive(
    			gt_can_socket,
				&s_can_frame,
    			&b_can_frame_avail);

    	if (E_OK == t_Read_Status)
    	{
    		if ((K_FALSE != b_can_frame_avail) &&
				(s_can_frame.u8_can_dlc <= K_CANOPEN_DATA_FRAME_LEN))
    		{
				/* No error during read and data is available: copy the received data */
    			ps_can_frame->u32_can_id = s_can_frame.t_can_id & K_DRVPCAN_CAN_SFF_MASK;
				memcpy(ps_can_frame->au8_can_data, s_can_frame.au8_data, s_can_frame.u8_can_dlc);
				ps_can_frame->u8_can_dlc = s_can_frame.u8_can_dlc;

    			if (K_DRVPCAN_CAN_ERR_FLAG == (s_can_frame.t_can_id & K_DRVPCAN_CAN_ERR_FLAG))
    			{
    				/* CAN error frame received */
    				ps_can_frame->e_can_frame_type = E_CANOPEN_ERR_FRAME;
    			} else if (K_DRVPCAN_CAN_RTR_FLAG == (s_can_frame.t_can_id & K_DRVPCAN_CAN_RTR_FLAG))
    			{
    				/* Remote transmission request */
    				ps_can_frame->e_can_frame_type = E_CANOPEN_RTR_FRAME;
    			} else {
    				/* Normal CAN frame */
    				ps_can_frame->e_can_frame_type = E_CANOPEN_NORMAL_FRAME;
				}

				*pe_can_data_read_status = E_CANOPENWRP_DATA_OK;
			}
    		else
    		{
    			if ((K_FALSE != b_can_frame_avail) &&
					(s_can_frame.u8_can_dlc > K_CANOPEN_DATA_FRAME_LEN))
				{
					/* No error during read, data is available, but the received length is wrong */
	#ifdef K_DEBUG
					printf("PCanWrapperCanOpen_Read: invalid CAN frame length (%d)\n", s_can_frame.u8_can_dlc);
	#endif
					*pe_can_data_read_status = E_CANOPENWRP_DRV_ERR;
				}
				else
				{
					/* No error during read, but no data available */
					*pe_can_data_read_status = E_CANOPENWRP_NO_DATA;
				}
    		}
    	}
    	else
    	{
			/* Error during PCan channel read */
#ifdef K_DEBUG
			printf("PCanWrapperCanOpen_Read: error during PCan channel read\n");
#endif
			*pe_can_data_read_status = E_CANOPENWRP_DRV_ERR;
    	}

        t_Read_Status = E_OK;
    }
    else
    {
    	/* Module is not initialized or null pointer encountered, Halt */
    }

    /*! return code */
    return(t_Read_Status);
}


/**
 * \fn  Std_ReturnType PCanWrapperCanOpen_Write(const ts_CanopenCanFrame * const ps_can_frame, te_CanopenWrapperStatus * const pe_can_data_write_status)
 * \brief Send data on a CAN network with PCan driver
 *
 * \param <ts_CanopenCanFrame *ps_can_frame> { the CAN id and CAN data to send }
 * \param <te_CanopenWrapperStatus *pe_can_data_read_status>  { E_CANOPENWRP_DATA_OK if the frame was successfully sent,
 * 																E_CANOPENWRP_DRV_ERR if PCan driver error received }
 *
 * \return Std_ReturnType :
 *         - E_OK if command is successful
 *         - E_NOT_OK if not
 */
Std_ReturnType PCanWrapperCanOpen_Write(const ts_CanopenCanFrame * const ps_can_frame, te_CanopenWrapperStatus * const pe_can_data_write_status)
{
    /*! Declaration */     
    Std_ReturnType t_Write_Status;
	/* Frame encapsulation */
    ts_PCanFrame s_can_frame;


    /*! Initialization */
    t_Write_Status = E_NOT_OK;

    /*! Function Body */
    if ((E_OK == gt_CfgPCanWrapperCanOpen_Status) && (NULL_PTR != ps_can_frame) && (NULL_PTR != pe_can_data_write_status))
    {
    	/* Frame construction */
    	s_can_frame.t_can_id = ps_can_frame->u32_can_id;
    	s_can_frame.u8_can_dlc = ps_can_frame->u8_can_dlc;
    	memcpy(s_can_frame.au8_data, ps_can_frame->au8_can_data, ps_can_frame->u8_can_dlc);
    	switch(ps_can_frame->e_can_frame_type)
    	{
    	case E_CANOPEN_NORMAL_FRAME:
    		/* Normal frame: no change */
    	    t_Write_Status = E_OK;
    		break;

    	case E_CANOPEN_RTR_FRAME:
    		/* Remote transmission frame */
    		s_can_frame.t_can_id |= K_DRVPCAN_CAN_RTR_FLAG;
    	    t_Write_Status = E_OK;
    		break;

    	case E_CANOPEN_ERR_FRAME:
    		/* Error frame */
    	default:
#ifdef K_DEBUG
    		printf("PCanWrapperCanOpen_Write: invalid CAN frame type (%d)\n", ps_can_frame->e_can_frame_type);
#endif
    		break;
    	}

    	/* Write the data on the serial port */
    	t_Write_Status |= DrvPCan_Send(gt_can_socket, &s_can_frame);

    	if (E_OK == t_Write_Status)
    	{
    		/* Write complete */
			*pe_can_data_write_status = E_CANOPENWRP_DATA_OK;
    	} else {
			/* Problem during the write */
#ifdef K_DEBUG
			printf("PCanWrapperCanOpen_Write: error during PCan channel write\n");
#endif

			*pe_can_data_write_status = E_CANOPENWRP_DRV_ERR;
    	}

    	t_Write_Status = E_OK;
    }
    else
    {
    	/* Module is not initialized or error on a function parameter, Halt */
    }

    /*! return code */
    return(t_Write_Status);
}


/**
 * \fn  Std_ReturnType PCanWrapperCanOpen_Close(void)
 * \brief Terminate the PCAN driver
 *
 * \param void
 *
 * \return Std_ReturnType :
 *         - E_OK if command is successful
 *         - E_NOT_OK if not
 */
Std_ReturnType PCanWrapperCanOpen_Close(void)
{
    /*! Declaration */
    Std_ReturnType t_Close_Status;

    /*! Initialization */
    t_Close_Status = E_OK;

	if (E_OK == gt_CfgPCanWrapperCanOpen_Status)
	{
		if (K_DRVPCAN_CLOSED_HANDLE != gt_can_socket)
		{
			t_Close_Status |= DrvPCan_Close( &gt_can_socket );
		}
	}
	else
    {
    	/* Module is not initialized or error on a function parameter, Halt */
		t_Close_Status = E_NOT_OK ;
    }

    /*! return code */
	return t_Close_Status;
}


/* Private functions 							      */
/*============================================================================*/

