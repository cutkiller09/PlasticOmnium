/*============================================================================*/
/*                        	ROBOSOFT                                      */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*/
/*!
 * $Source: Driver_PCANDriver.c
 * $Author: MDEGA
 * $Date: 2015/12/28
 */
/*============================================================================*/
/* DESCRIPTION :                                                              */
/** \file  Driver_PCANDriver.c
  * \brief Source file of PCANDriver Service
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

#define K_DEBUG 1

/* Includes 								      */
/*============================================================================*/
#include "types.h"
#include "Library_StdLib.h"
#include "../../Driver_PCanDriver/inc/Driver_PCanDriver.h"

#include <stdio.h>
#include <string.h>

#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>


/* Constants and types  						      */
/*============================================================================*/
/* particular protocols of the protocol family PF_CAN */
#define K_DRVPCAN_CAN_RAW		1 /* RAW sockets */
#define K_DRVPCAN_CAN_BCM		2 /* Broadcast Manager */
#define K_DRVPCAN_CAN_TP16		3 /* VAG Transport Protocol v1.6 */
#define K_DRVPCAN_CAN_TP20		4 /* VAG Transport Protocol v2.0 */
#define K_DRVPCAN_CAN_MCNET		5 /* Bosch MCNet */
#define K_DRVPCAN_CAN_ISOTP		6 /* ISO 15765-2 Transport Protocol */
#define K_DRVPCAN_CAN_NPROTO	7

/**
 * The sockaddr structure for CAN sockets
 */
typedef struct {
	/* address family number AF_CAN */
	uint16 	u16_can_family;
	/* CAN network interface index */
	sint32  s32_can_ifindex;
	/* protocol specific address information */
	union {
		/* transport protocol class address information (e.g. ISOTP) */
		struct { t_canid t_rx_id, t_tx_id; } s_tp;

		/* reserved for future CAN protocols address information */
	} u_can_addr;
} ts_PCanSockaddr;


/* Variables 								      */
/*============================================================================*/
static Std_ReturnType gt_InitPCANDriver_Status;


/* Private functions prototypes 					      */
/*============================================================================*/

/* Inline functions 							      */
/*============================================================================*/

/* Private functions 							      */
/*============================================================================*/

/**
 * \fn  Std_ReturnType DrvPCan_Initialize(void)
 * \brief this function initialize internal parameters
 *
 * \param  void
 *
* \return Std_ReturnType :
*         - E_OK if good return function
*         - E_NOT_OK if not
 */
Std_ReturnType DrvPCan_Initialize(void)
{
    /*! Declaration */
    Std_ReturnType t_Initialize_Status;

    /*! Initialization */
    gt_InitPCANDriver_Status = E_OK ;

    /*! Function Body */

    t_Initialize_Status = gt_InitPCANDriver_Status ;

    /*! return code */
    return(t_Initialize_Status);
}

/**
 * \fn  Std_ReturnType DrvPCan_Open(const char * const pc_can_ifname, t_CanDrvHandler * const pt_can_handler)
 * \brief Open a CAN channel
 *
 * \param  <uint8 *pc_can_ifname> { CAN interface name }
 * \param  <t_CanDrvHandler *pt_can_handler> { CAN port handler }
 *
 * \return Std_ReturnType :
 *         - E_OK if good return function
 *         - E_NOT_OK if not
 */
Std_ReturnType DrvPCan_Open(const uint8 * const pu8_can_ifname, t_CanDrvHandler * const pt_can_handler)
{
    /*! Declaration */
    Std_ReturnType t_Open_Status;
	/* Socket configuration structures for CAN interface */
    ts_PCanSockaddr s_can_addr;
	struct ifreq s_can_ifr;
	/* CAN interface status used by ioctl and bind */
	int status;

    /*! Initialization */
	t_Open_Status = E_NOT_OK ;

    /*! Function Body */
    if ((E_OK == gt_InitPCANDriver_Status) && (NULL_PTR != pt_can_handler) && (NULL_PTR != pu8_can_ifname))
    {
		/* Create a raw data socket for CAN network interface */
    	*pt_can_handler = socket(PF_CAN, SOCK_RAW, K_DRVPCAN_CAN_RAW);

		if (*pt_can_handler < 0) {
			perror("DrvPCan_Open: unable to create a CAN interface socket");
		} else {
			/* Configure the socket with the requested network interface name */
			s_can_addr.u16_can_family = AF_CAN;
			strcpy(s_can_ifr.ifr_name, (const char *)pu8_can_ifname);

			/* Get the index number of the CAN interface */
			s_can_ifr.ifr_ifindex = if_nametoindex(s_can_ifr.ifr_name);
			if (0 == s_can_ifr.ifr_ifindex)
			{
				perror("DrvPCan_Open: cannot get the index number of the CAN network interface");
			} else {
				/* Get the received index number */
				s_can_addr.s32_can_ifindex = s_can_ifr.ifr_ifindex;

				/* Bind the socket to the physical CAN interface */
				status = bind(*pt_can_handler, (struct sockaddr *)&s_can_addr, sizeof(ts_PCanSockaddr));
				if (status < 0)
				{
					perror("DrvPCan_Open: cannot bind the CAN socket to the CAN network interface");
				} else {
					t_Open_Status = E_OK;
				}
			}
		}

		if (E_NOT_OK == t_Open_Status)
		{
			/* Error occurred, close the CAN socket */
			DrvPCan_Close(pt_can_handler);
		}
    }
    else
    {
	/* Module is not initialized or null pointer encountered, Halt */
	;
    }

    /*! return code */
    return(t_Open_Status);
}

/**
 * \fn  Std_ReturnType DrvPcan_Close(t_CanDrvHandler * const pt_can_handler)
 * \brief Closes a CAN channel
 *
 * \param  <t_CanDrvHandler *pt_can_handler> { CAN port handler }
 *
 * \return Std_ReturnType :
 *         - E_OK if good return function
 *         - E_NOT_OK if not
 */
Std_ReturnType DrvPCan_Close(t_CanDrvHandler * const pt_can_handler)
{
    /*! Declaration */
    Std_ReturnType t_Close_Status;

    /*! Initialization */
    t_Close_Status = E_NOT_OK;

    /*! Function Body */
    if ((E_OK == gt_InitPCANDriver_Status) && (NULL_PTR != pt_can_handler) )
    {
    	t_Close_Status = E_OK ;
        if (*pt_can_handler >= 0)
        {
        	close( *pt_can_handler ) ;
        	*pt_can_handler = K_DRVPCAN_CLOSED_HANDLE;
        }
    }
    else
    {
	/* Module is not initialized or null pointer encountered, Halt */
	;
    }

    /*! return code */
    return(t_Close_Status);
}

/**
 * \fn  Std_ReturnType DrvPcan_Send(const t_CanDrvHandler t_can_handler, const ts_PCanFrame * const ps_can_frame)
 * \brief Sends a CAN frame
 *
 * \param  <t_CanDrvHandler t_can_handler> { CAN port handler }
 * \param  <ts_PCanFrame *ps_can_frame> { CAN frame to send }
 *
 * \return Std_ReturnType :
 *         - E_OK if good return function
 *         - E_NOT_OK if not
 */
Std_ReturnType DrvPCan_Send(const t_CanDrvHandler t_can_handler, const ts_PCanFrame * const ps_can_frame)
{
    /*! Declaration */
    Std_ReturnType t_Send_Status;
	/* Write status */
	int write_status;

    /*! Initialization */
    t_Send_Status = E_NOT_OK ;

    /*! Function Body */
    if ((E_OK == gt_InitPCANDriver_Status) && (NULL_PTR != ps_can_frame) && (K_DRVPCAN_CLOSED_HANDLE != t_can_handler))
    {
    	/* Send the frame */
		write_status = send(t_can_handler, ps_can_frame, K_DRVPCAN_CAN_MTU, 0);
		if (write_status >= 0)
		{
			/* Write successful */
			t_Send_Status = E_OK ;
		}
	}
	else
	{
	/* Module is not initialized, invalid CAN handler or null pointer encountered, Halt */
	;
	}

    /*! return code */
    return(t_Send_Status);
}

/**
 * \fn  Std_ReturnType DrvPcan_Receive(const t_CanDrvHandler t_can_handler, ts_PCanFrame * const ps_can_frame, bool * const pb_can_data_avail)
 * \brief Receives a CAN frame
 *
 * \param  <t_CanDrvHandler t_can_handler> { CAN port handler }
 * \param  <ts_PCanFrame *ps_can_frame> { Received CAN frame }
 * \param  <bool * const pb_can_data_avail> { K_FALSE: no frame received, K_TRUE: frame available }
 *
 * \return Std_ReturnType :
 *         - E_OK if good return function
 *         - E_NOT_OK if not
 */
Std_ReturnType DrvPCan_Receive(const t_CanDrvHandler t_can_handler, ts_PCanFrame * const ps_can_frame, bool * const pb_can_data_avail)
{
    /*! Declaration */
    Std_ReturnType t_Receive_Status;
	/* Read status */
	int read_status;

    /*! Initialization */
    t_Receive_Status = E_NOT_OK ;

    /*! Function Body */
   	if ((E_NOT_OK != gt_InitPCANDriver_Status) && (K_DRVPCAN_CLOSED_HANDLE != t_can_handler) && (NULL_PTR != ps_can_frame) && (NULL_PTR != pb_can_data_avail))
	{
   		/* Non-blocking receive */
   		read_status = recv(t_can_handler, ps_can_frame, K_DRVPCAN_CAN_MTU, MSG_DONTWAIT) ;
    	if (K_DRVPCAN_CAN_MTU == read_status)
    	{
    		/* Frame available */
    		*pb_can_data_avail = K_TRUE;
    		t_Receive_Status = E_OK ;
		} else if (read_status < 0) {
			/* No CAN frame available */
    		*pb_can_data_avail = K_FALSE;
			t_Receive_Status = E_OK ;
		} else {
			/* Read error: incomplete CAN frame */
		}
	}
	else
	{
	/* Module is not initialized, invalid CAN handler or null pointer encountered, Halt */
	;
	}

    /*! return code */
    return(t_Receive_Status);
}
