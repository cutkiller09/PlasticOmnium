/*============================================================================*/
/*                        	ROBOSOFT                                      */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*/
/*! 
 * $Source: Library_PrivateCanOpenComWrapper.c
 * $Author: BRobin 
 * $Date: 2015/12/08  
 */
/*============================================================================*/
/* DESCRIPTION :                                                              */
/** \file  Library_PrivateCanOpenComWrapper.c
  * \brief Fichier Source du Service PrivateCanOpenComWrapper
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
#include "Library_PrivateCanOpenComWrapper.h"
#include "Protocol_CanOpen.h"
#include "Library_PrivateCanOpenPCanWrapper.h"
#include "Library_PrivateCanOpenXtppSerialWrapper.h"
#include <stdio.h>
#include <string.h>

/* Constants and types  						      */
/*============================================================================*/ 
#define K_DEBUG
   
/* Variables 								      */
/*============================================================================*/ 
static Std_ReturnType gt_InitComWrapperCanOpen_Status = E_NOT_OK; /* Global Initialization variable */
static Std_ReturnType gt_CfgComWrapperCanOpen_Status = E_NOT_OK; /* Global Initialization variable */

/* CANopen wrapper used for the CAN communication */
static te_CanopenWrapperType ge_canopen_com_wrapper;


/* Private functions prototypes 					      */
/*============================================================================*/ 

/* Inline functions 							      */
/*============================================================================*/

/* Public functions 							      */
/*============================================================================*/

/**
 * \fn  Std_ReturnType ComWrapperCanOpen_Init(void)
 * \brief Communication drivers wrapper initialization
 *
 * \param  void
 *
 * \return Std_ReturnType :
 *         - E_OK if command is successful
 *         - E_NOT_OK if not
 */
Std_ReturnType ComWrapperCanOpen_Init(void)
{
    /*! Declaration */

    /*! Initialization */
	gt_InitComWrapperCanOpen_Status = XtppWrapperCanOpen_Init();
	gt_InitComWrapperCanOpen_Status |= PCanWrapperCanOpen_Init();

	ge_canopen_com_wrapper = E_CANOPENWRP_NR;

    /*! return code */
	return gt_InitComWrapperCanOpen_Status;
}

/**
 * \fn  Std_ReturnType ComWrapperCanOpen_Configure(const ts_CanopenCfg * const ps_canopen_cfg)
 * \brief PCan driver wrapper CANopen configuration
 *
 * \param <ts_CanopenCfg *ps_canopen_cfg> { CANopen library configuration }
 *
 * \return Std_ReturnType :
 *         - E_OK if command is successful
 *         - E_NOT_OK if not
 */
Std_ReturnType ComWrapperCanOpen_Configure(const ts_CanopenCfg * const ps_canopen_cfg)
{
    /*! Declaration */

    /*! Initialization */
    gt_CfgComWrapperCanOpen_Status = E_OK;

	if ((E_OK == gt_InitComWrapperCanOpen_Status) && (NULL_PTR != ps_canopen_cfg))
	{ 
		/* Configure the communication wrapper */
		switch(ps_canopen_cfg->e_canopen_wrapper_cfg)
		{
			case E_CANOPENWRP_SERIAL_XTPP:
				/* XTPP serial driver *//* Configure the XTPP serial driver */
				gt_CfgComWrapperCanOpen_Status |= XtppWrapperCanOpen_Configure(&ps_canopen_cfg->s_canopen_xtpp_serial_cfg);
				ge_canopen_com_wrapper = ps_canopen_cfg->e_canopen_wrapper_cfg;
				break;

			case E_CANOPENWRP_PCAN:
				/* PCAN driver */
				/* Configure the PCAN driver */
				gt_CfgComWrapperCanOpen_Status |= PCanWrapperCanOpen_Configure(ps_canopen_cfg->au8_canopen_pcan_ifname_cfg);
				ge_canopen_com_wrapper = ps_canopen_cfg->e_canopen_wrapper_cfg;
				break;

			default:
#ifdef K_DEBUG
				printf("ComWrapperCanOpen_Configure: invalid communication wrapper (%d)\n", ps_canopen_cfg->e_canopen_wrapper_cfg);
#endif
				break;
		}
	}
	else
    {
    	/* Module is not initialized or error on a function parameter, Halt */
    }

    /*! return code */
	return gt_CfgComWrapperCanOpen_Status;
}

/**
 * \fn  Std_ReturnType ComWrapperCanOpen_Receive(ts_CanopenCanFrame * const ps_can_frame, te_CanopenWrapperStatus * const pe_can_data_read_status)
 * \brief Receive data from a CAN network
 *
 * \param <ts_CanopenCanFrame *ps_can_frame> { the received CAN id and CAN data }
 * \param <te_CanopenWrapperStatus *pe_can_data_read_status>  { E_CANOPENWRP_DATA_OK if a frame was received, E_CANOPENWRP_NO_DATA if no frame was received,
 * 																E_CANOPENWRP_DRV_ERR if PCan driver error received }
 *
 * \return Std_ReturnType :
 *         - E_OK if command is successful
 *         - E_NOT_OK if not
 */
Std_ReturnType ComWrapperCanOpen_Receive(ts_CanopenCanFrame * const ps_can_frame, te_CanopenWrapperStatus * const pe_can_data_read_status)
{
    /*! Declaration */     
    Std_ReturnType t_Read_Status;

    /*! Initialization */
    t_Read_Status = E_NOT_OK;

    /*! Function Body */
    if ((E_OK == gt_CfgComWrapperCanOpen_Status) && (NULL_PTR != ps_can_frame) && (NULL_PTR != pe_can_data_read_status))
    {
    	switch(ge_canopen_com_wrapper)
    	{
    	case E_CANOPENWRP_SERIAL_XTPP:
    		/* XTPP serial driver */
    		t_Read_Status = XtppWrapperCanOpen_Read(ps_can_frame, pe_can_data_read_status);
    		break;

    	case E_CANOPENWRP_PCAN:
    		/* PCAN driver */
    		t_Read_Status = PCanWrapperCanOpen_Read(ps_can_frame, pe_can_data_read_status);
    		break;

    	default:
#ifdef K_DEBUG
    		printf("ComWrapperCanOpen_Receive: invalid communication wrapper type (%d)\n", ge_canopen_com_wrapper);
#endif
    		break;
    	}
    }
    else
    {
    	/* Module is not initialized or null pointer encountered, Halt */
    }

    /*! return code */
    return(t_Read_Status);
}


/**
 * \fn  Std_ReturnType ComWrapperCanOpen_Send(const ts_CanopenCanFrame * const ps_can_frame, te_CanopenWrapperStatus * const pe_can_data_write_status)
 * \brief Send data on a CAN network
 *
 * \param <te_CanopenWrapperType e_can_wrapper> { Communication driver wrapper to use }
 * \param <ts_CanopenCanFrame *ps_can_frame> { the CAN id and CAN data to send }
 * \param <te_CanopenWrapperStatus *pe_can_data_read_status>  { E_CANOPENWRP_DATA_OK if the frame was successfully sent,
 * 																E_CANOPENWRP_DRV_ERR if PCan driver error received }
 *
 * \return Std_ReturnType :
 *         - E_OK if command is successful
 *         - E_NOT_OK if not
 */
Std_ReturnType ComWrapperCanOpen_Send(const ts_CanopenCanFrame * const ps_can_frame, te_CanopenWrapperStatus * const pe_can_data_write_status)
{
    /*! Declaration */     
    Std_ReturnType t_Write_Status;

    /*! Initialization */
    t_Write_Status=E_NOT_OK;

    /*! Function Body */
    if ((E_OK == gt_CfgComWrapperCanOpen_Status) && (NULL_PTR != ps_can_frame) && (NULL_PTR != pe_can_data_write_status))
    {
    	switch(ge_canopen_com_wrapper)
    	{
    	case E_CANOPENWRP_SERIAL_XTPP:
    		/* XTPP serial driver */
    		t_Write_Status = XtppWrapperCanOpen_Write(ps_can_frame, pe_can_data_write_status);
    		break;

    	case E_CANOPENWRP_PCAN:
    		/* PCAN driver */
    		t_Write_Status = PCanWrapperCanOpen_Write(ps_can_frame, pe_can_data_write_status);
    		break;

    	default:
#ifdef K_DEBUG
    		printf("ComWrapperCanOpen_Receive: invalid communication wrapper type (%d)\n", ge_canopen_com_wrapper);
#endif
    		break;
    	}

//    	t_Write_Status = E_OK; // TODO : to be tested
    }
    else
    {
    	/* Module is not initialized or error on a function parameter, Halt */
    }

    /*! return code */
    return(t_Write_Status);
}


/**
 * \fn  Std_ReturnType ComWrapperCanOpen_Close(const ts_CanopenCfg * const ps_canopen_cfg)
 * \brief Terminate the communication
 *
 * \param <ts_CanopenCfg *ps_canopen_cfg> { CANopen library configuration }
 *
 * \return Std_ReturnType :
 *         - E_OK if command is successful
 *         - E_NOT_OK if not
 */
Std_ReturnType ComWrapperCanOpen_Close(const ts_CanopenCfg * const ps_canopen_cfg)
{
    /*! Declaration */
    Std_ReturnType t_Close_Status;

    /*! Initialization */
    t_Close_Status = E_OK;

	if ((E_OK == gt_CfgComWrapperCanOpen_Status) && (NULL_PTR != ps_canopen_cfg))
	{
		switch(ps_canopen_cfg->e_canopen_wrapper_cfg)
		{
			case E_CANOPENWRP_SERIAL_XTPP:
				/* Close the XTPP serial driver */
				t_Close_Status |= XtppWrapperCanOpen_Close();
				break;

			case E_CANOPENWRP_PCAN:
				/* Close the PCAN driver */
				t_Close_Status |= PCanWrapperCanOpen_Close();
				break;

			default:
				t_Close_Status = E_NOT_OK ;
#ifdef K_DEBUG
				printf("ComWrapperCanOpen_Close: invalid communication wrapper (%d)\n", ps_canopen_cfg->e_canopen_wrapper_cfg);
#endif
				break;
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

