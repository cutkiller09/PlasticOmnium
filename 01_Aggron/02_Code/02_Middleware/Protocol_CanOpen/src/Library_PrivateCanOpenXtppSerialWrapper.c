/*============================================================================*/
/*                        	ROBOSOFT                                      */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*/
/*! 
 * $Source: Library_XtppWrapperCanOpen.c 
 * $Author: BRobin 
 * $Date: 2015/12/08  
 */
/*============================================================================*/
/* DESCRIPTION :                                                              */
/** \file  Library_XtppWrapperCanOpen.c 
  * \brief Fichier Source du Service XtppWrapperCanOpen
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
#include "Library_PrivateCanOpenXtppSerialWrapper.h"
#include <stdio.h>
#include <string.h>

/* Constants and types  						      */
/*============================================================================*/ 
#define K_DEBUG

/* XtrapulsPac format of CAN-on-serial frame */
/* 2 bytes for header and footer | 2 bytes for CAN identifier | 8 bytes CAN data */
#define K_XTPPCANOPEN_FRAME_LEN (K_CANOPEN_DATA_FRAME_LEN + 2 + 2)

/* Variables 								      */
/*============================================================================*/ 
static Std_ReturnType gt_InitXtppWrapperCanOpen_Status = E_NOT_OK; /* Global Initialization variable */
static Std_ReturnType gt_CfgXtppWrapperCanOpen_Status = E_NOT_OK; /* Global Initialization variable */

/* Serial port used to communicate with XtrapulsPac motors */
static sint32 gs32_serial_port_desc;

/* Current states of frames reception on the serial line from a CANopen slave node */
static uint8 gau8_xtpp_serialdata_frame[K_XTPPCANOPEN_FRAME_LEN];
static uint8 gu8_xtpp_serialdata_rcv_len;

/* Private functions prototypes 					      */
/*============================================================================*/ 

/* Check the integrity of a CAN-on-serial line CANopen frame received in the XtrapulsPac format */
static bool XtppWrapperCanOpen_Check_Frame(uint8 au8_frame[K_XTPPCANOPEN_FRAME_LEN]);


/* Inline functions 							      */
/*============================================================================*/

/* Public functions 							      */
/*============================================================================*/

/**
 * \fn  Std_ReturnType XtppWrapperCanOpen_Init(void)
 * \brief Library initialization
 *
 * \param  void
 *
 * \return Std_ReturnType :
 *         - E_OK if command is successful
 *         - E_NOT_OK if not
 */
Std_ReturnType XtppWrapperCanOpen_Init(void)
{
    /*! Declaration */

    /*! Initialization */
	gu8_xtpp_serialdata_rcv_len = 0;
	gs32_serial_port_desc = K_DRVSERIAL_CLOSED;

	gt_InitXtppWrapperCanOpen_Status = E_OK;

    /*! return code */
	return gt_InitXtppWrapperCanOpen_Status;
}


/**
 * \fn  Std_ReturnType XtppWrapperCanOpen_Configure(const serialdrv_cfg_type * const ps_canopen_xtpp_serial_cfg)
 * \brief CANopen serial port wrapper configuration
 *
 * \param <serialdrv_cfg_type *ps_canopen_xtpp_serial_cfg> { XTPP serial driver configuration }
 *
 * \return Std_ReturnType :
 *         - E_OK if command is successful
 *         - E_NOT_OK if not
 */
Std_ReturnType XtppWrapperCanOpen_Configure(const serialdrv_cfg_type * const ps_canopen_xtpp_serial_cfg)
{
    /*! Declaration */

    /*! Initialization */
    gt_CfgXtppWrapperCanOpen_Status = E_NOT_OK;

	if ((E_OK == gt_InitXtppWrapperCanOpen_Status) && (NULL_PTR != ps_canopen_xtpp_serial_cfg))
	{
		gt_CfgXtppWrapperCanOpen_Status = DrvSerial_Open(ps_canopen_xtpp_serial_cfg, &gs32_serial_port_desc);
	}
	else
    {
    	/* Module is not initialized or error on a function parameter, Halt */
    }

    /*! return code */
	return gt_CfgXtppWrapperCanOpen_Status;
}


/**
 * \fn  Std_ReturnType XtppWrapperCanOpen_Read(canopen_canif_frame_type pt_buffer, xtppwrpcanopen_readwrite_status_type *pt_data_read_status)
 * \brief Receive data from a CAN network with CAN-to-serial line simulation
 *
 * \param <ts_CanopenCanFrame *ps_can_frame> { the received CAN id and CAN data }
 * \param <te_CanopenWrapperStatus *pe_can_data_read_status>  { E_CANOPENWRP_DATA_OK if a frame was received, E_CANOPENWRP_NO_DATA if no frame was received,
 * 																E_CANOPENWRP_DRV_ERR if serial driver error received }
 *
 * \return Std_ReturnType :
 *         - E_OK if command is successful
 *         - E_NOT_OK if not
 */
Std_ReturnType XtppWrapperCanOpen_Read(ts_CanopenCanFrame * const ps_can_frame, te_CanopenWrapperStatus * const pe_can_data_read_status)
{
    /*! Declaration */     
    Std_ReturnType t_Read_Status;
    Std_ReturnType t_Serial_Status;
	/* Status of the serial port read operation */
    sint32 s32_serial_data_read_status;
	/* Frame reception state */
	uint8 u8_frame_current_len;
	uint8 u8_frame_remaining_len;

    /*! Initialization */
    t_Read_Status = E_NOT_OK;
	u8_frame_current_len = gu8_xtpp_serialdata_rcv_len;
	u8_frame_remaining_len = K_XTPPCANOPEN_FRAME_LEN - u8_frame_current_len;

    /*! Function Body */
    if ((E_OK == gt_CfgXtppWrapperCanOpen_Status) && (NULL_PTR != ps_can_frame) && (NULL_PTR != pe_can_data_read_status))
    {
    	/* Read some data on the configured serial port and append them at the end of the previously received data */
    	t_Serial_Status = DrvSerial_Receive(
    			gs32_serial_port_desc,
    			&gau8_xtpp_serialdata_frame[u8_frame_current_len],
    			u8_frame_remaining_len,
				&s32_serial_data_read_status);

    	if (E_OK == t_Serial_Status)
    	{
    		if (s32_serial_data_read_status >= 0)
    		{
				/* No error during read */
				u8_frame_current_len += s32_serial_data_read_status;

				if (K_XTPPCANOPEN_FRAME_LEN == u8_frame_current_len)
				{
					/* Whole frame received from the master */
					/* Check the frame integrity (XtrapulsPac format) */
					if (K_FALSE != XtppWrapperCanOpen_Check_Frame(gau8_xtpp_serialdata_frame))
					{
						/* Frame is ok, copy it into the caller CAN frame (without header and footer) */
						ps_can_frame->e_can_frame_type = E_CANOPEN_NORMAL_FRAME;
						ps_can_frame->u32_can_id = (gau8_xtpp_serialdata_frame[1] << 8) | gau8_xtpp_serialdata_frame[2];
						ps_can_frame->u8_can_dlc = K_CANOPEN_DATA_FRAME_LEN;
						memcpy(ps_can_frame->au8_can_data, &gau8_xtpp_serialdata_frame[3], K_CANOPEN_DATA_FRAME_LEN);

						*pe_can_data_read_status = E_CANOPENWRP_DATA_OK;
					} else {
						/* Integrity is not correct, consider that no frame has been received */
						*pe_can_data_read_status = E_CANOPENWRP_NO_DATA;
					}

					/* Reset the driver state */
					gu8_xtpp_serialdata_rcv_len = 0;
				} else {
					/* No full frame received yet */
					/* Update the current frame length */
					gu8_xtpp_serialdata_rcv_len = u8_frame_current_len;

					*pe_can_data_read_status = E_CANOPENWRP_NO_DATA;
				}
			} else {
				/* Error during serial port read */
#ifdef K_DEBUG
				printf("XtppWrapperCanOpen_Read: error during serial port read\n");
#endif
				*pe_can_data_read_status = E_CANOPENWRP_DRV_ERR;
			}
    	} else {
			/* Error during serial port read */
#ifdef K_DEBUG
			printf("XtppWrapperCanOpen_Read: error during serial port check\n");
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
 * \fn  Std_ReturnType XtppWrapperCanOpen_Write(void) 
 * \brief Send data to a CAN network with CAN-to-serial line simulation
 *
 * \param <ts_CanopenCanFrame *ps_can_frame> { the CAN id and CAN data to send }
 * \param <te_CanopenWrapperStatus *pe_can_data_read_status>  { E_CANOPENWRP_DATA_OK if the frame was successfully sent,
 * 																E_CANOPENWRP_DRV_ERR if serial driver error received }
 *
 * \return Std_ReturnType :
 *         - E_OK if command is successful
 *         - E_NOT_OK if not
 */
Std_ReturnType XtppWrapperCanOpen_Write(const ts_CanopenCanFrame * const ps_can_frame, te_CanopenWrapperStatus * const pe_can_data_read_status)
{
    /*! Declaration */     
    Std_ReturnType t_Write_Status;
    Std_ReturnType t_Serial_Status;
	/* Status of the write port operation */
    sint32 s32_serial_data_write_status;
	/* Frame encapsulation */
	uint8 au8_frame[K_XTPPCANOPEN_FRAME_LEN];
	uint8 u8_crc = 0;
	uint8 u8_idx;

    /*! Initialization */
    t_Write_Status = E_NOT_OK;

    /*! Function Body */
    if ((E_OK == gt_CfgXtppWrapperCanOpen_Status) && (NULL_PTR != ps_can_frame) && (NULL_PTR != pe_can_data_read_status))
    {
    	if ((E_CANOPEN_NORMAL_FRAME == ps_can_frame->e_can_frame_type) && (K_CANOPEN_DATA_FRAME_LEN == ps_can_frame->u8_can_dlc))
    	{
			/* Frame encapsulation: XtrapulsPac format */

			/* Content should be: | 'M' | 8000h ORed CAN id (2 bytes) | CANopen data (8 bytes) | 1-byte CRC |
			 * CRC = XORed 10 bytes of CAN id + CANopen data
			 */
			au8_frame[0] = 'M';
			au8_frame[1] = 0x80 | (ps_can_frame->u32_can_id >> 8);
			au8_frame[2] = ps_can_frame->u32_can_id & 0xFF;
			memcpy(&au8_frame[3], ps_can_frame->au8_can_data, K_CANOPEN_DATA_FRAME_LEN);
			/* CRC computation */
			for (u8_idx = 1; u8_idx < (K_XTPPCANOPEN_FRAME_LEN - 1); u8_idx++)
			{
				u8_crc ^= au8_frame[u8_idx];
			}
			au8_frame[K_XTPPCANOPEN_FRAME_LEN - 1] = u8_crc;

			/* Write the data on the serial port */
			t_Serial_Status = DrvSerial_Send(gs32_serial_port_desc, au8_frame, K_XTPPCANOPEN_FRAME_LEN, &s32_serial_data_write_status);

			if ((E_OK != t_Serial_Status) || (K_XTPPCANOPEN_FRAME_LEN != s32_serial_data_write_status))
			{
				/* Problem during the write or not all the data has been written */
#ifdef K_DEBUG
				printf("XtppWrapperCanOpen_Write: error during serial port write\n");
#endif

				*pe_can_data_read_status = E_CANOPENWRP_DRV_ERR;
			} else {
				/* Write complete */
				*pe_can_data_read_status = E_CANOPENWRP_DATA_OK;
			}

			t_Write_Status = E_OK;
    	} else {
#ifdef K_DEBUG
    		printf("XtppWrapperCanOpen_Write: invalid CAN frame type (%d) or invalid CAN frame length (%d)\n", ps_can_frame->e_can_frame_type, ps_can_frame->u8_can_dlc);
#endif
    	}
    }
    else
    {
    	/* Module is not initialized or error on a function parameter, Halt */
    }

    /*! return code */
    return(t_Write_Status);
}

/**
 * \fn  Std_ReturnType XtppWrapperCanOpen_Close(void)
 * \brief Terminate the CAN-to-serial line simulation
 *
 * \param void
 *
 * \return Std_ReturnType :
 *         - E_OK if command is successful
 *         - E_NOT_OK if not
 */
Std_ReturnType XtppWrapperCanOpen_Close(void)
{
    /*! Declaration */
    Std_ReturnType t_Close_Status;

    /*! Initialization */
    t_Close_Status = E_OK;

	if (E_NOT_OK != gt_CfgXtppWrapperCanOpen_Status)
	{
		if (K_DRVSERIAL_CLOSED != gs32_serial_port_desc)
		{
			t_Close_Status |= DrvSerial_Close( &gs32_serial_port_desc );
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

/* Check the integrity of a CAN-to-serial line CANopen frame received in the XtrapulsPac format
 * Content should be: | 'S' | 8000h ORed CAN id (2 bytes) | CANopen data (8 bytes) | 1-byte CRC |
 * CRC = XORed 10 bytes of CAN id + CANopen data
 *
 * Inputs:
 *     - frame_au8: frame to be checked
 *
 * Return code:
 *     - FALSE: received frame integrity error
 *     - TRUE: received frame integrity correct
 *
 */
static bool XtppWrapperCanOpen_Check_Frame(uint8 au8_frame[K_XTPPCANOPEN_FRAME_LEN])
{
	bool b_integrity_status = K_FALSE;
	uint8 u8_crc = 0;
	uint8 u8_idx;

	if (au8_frame[0] == 'S')
	{
		/* Frame header correct, check the CRC */
		for (u8_idx = 1; u8_idx < (K_XTPPCANOPEN_FRAME_LEN - 1); u8_idx++)
		{
			u8_crc ^= au8_frame[u8_idx];
		}
		if (u8_crc == au8_frame[K_XTPPCANOPEN_FRAME_LEN - 1])
		{
			/* Integrity OK */
			/* Remove the ORed 8000h on CAN id */
			au8_frame[1] &= 0x7F;

			/* SDO server answer 11-bit CAN id should be 101 1xxx xxxx: however XtrapulsPac returns SDO client request 110 0xxx xxxx */
			/* Update this in the response frame */
			if ((au8_frame[1] == 0x06) && ((au8_frame[2] & 0x80) == 0x00))
			{
				/* Replace the CAN id with the server response one */
				au8_frame[1] = 0x05;
				au8_frame[2] |= 0x80;
			}

			b_integrity_status = K_TRUE;
		} else {
#ifdef K_DEBUG
			printf("XtppWrapperCanOpen_Check_Frame: warning: invalid CRC received, frame rejected\n");
#endif
		}
	} else {
#ifdef K_DEBUG
		printf("XtppWrapperCanOpen_Check_Frame: warning: invalid header received, frame rejected\n");
#endif
	}

	return b_integrity_status;
}
