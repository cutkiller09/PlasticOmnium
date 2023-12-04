/*============================================================================*/
/*                        	ROBOSOFT                                      */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*/
/*! 
 * $Source: Library_LibLidarLms151.c 
 * $Author: Slo
 * $Date: 2016/01/14
 */
/*============================================================================*/
/* DESCRIPTION :                                                              */
/** \file  Library_LibLidarLms151.c 
  * \brief Fichier Source du stub de LibLidarLms151
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
#include "Library_LibLidarLms151.h" 
#include "Driver_TcpSocketDriver.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

/* Constants and types  						      */
/*============================================================================*/

/* Variables 								      */
/*============================================================================*/ 

/* Private functions prototypes 					      */
/*============================================================================*/

/* inline functions prototypes                                                */
/*============================================================================*/

/* inline functions 							      */
/*============================================================================*/

/* Private functions 							      */
/*============================================================================*/

/* Public functions 							      */
/*============================================================================*/

/**
 * \fn  Std_ReturnType LibLidarLms151_Initialize(void) 
 * \brief this function initialize module
 *
 * \param  none
 *
* \return Std_ReturnType :
*         - E_OK if SrvConf_Get_Lms151TcpSocket, LibLidarLms151_Open and SrvConf_Get_Lms151OutputMode return functions are E_OK
*         - E_NOT_OK if not
 */
Std_ReturnType LibLidarLms151_Initialize(void) 
{
    /* return code */
    return(gs_LibLidarLms151_status);
}

/**
 * \fn  Std_ReturnType LibLidarLms151_Configure( const ts_LibLidarLms151_conf * const ps_LibLidarLms151_conf,
 *											 const ts_LibLidarLms151_dataconf * const  ps_LibLidarLms151_dataconf,
 *											 const ts_LibLidarLms151_socket * const  ps_LibLidarLms151_socket)
 * \brief this function Configure module
 *
 * \param const ts_LibLidarLms151_laserconf * const ps_LibLidarLms151_laserconf : laser configuration
 * 		  const ts_LibLidarLms151_dataconf * const  ps_LibLidarLms151_dataconf : data (returned by laser) configuration
 * 		  const ts_LibLidarLms151_socket * const  ps_LibLidarLms151_socket : socket laser configuration
 *
* \return Std_ReturnType :
*         - E_OK if module is initialized and ps_LibLidarLms151_laserconf, ps_LibLidarLms151_dataconf, ps_LibLidarLms151_socket are not NULL
*         - E_NOT_OK if not
 */
Std_ReturnType LibLidarLms151_Configure( const ts_LibLidarLms151_Configuration * const ps_LibLidarLms151_Configuration)
{
    /* Function Body */
    if( (NULL_PTR != ps_LibLidarLms151_Configuration) )
    {
    	/* NOP */
        ;
    }

    /* return code */
    return(gs_LibLidarLms151_status);
}

/**
 * \fn  Std_ReturnType LibLidarLms151_Manage(void)
 * \brief this function is the central point of the library, it manage the laser
 *
 * \param  none
 *
* \return Std_ReturnType :
*         - E_OK if good return function
*         - E_NOT_OK if not
 */
Std_ReturnType LibLidarLms151_Manage(void)
{
    /* return code */
    return(gs_LibLidarLms151_status);
}

/**
 * \fn  Std_ReturnType LibLidarLms151_Open(void)
 * \brief this function will open Tcp Socket needed for communication with Lms151
 *
 * \param  none
 *
* \return Std_ReturnType :
*         - E_NOT_OK if module is not initialized or Tcp socket return an error
*         - else E_OK
 */
Std_ReturnType LibLidarLms151_Open(void)
{
    /* return code */
    return(gs_LibLidarLms151_status);
}

/**
 * \fn  Std_ReturnType LibLidarLms151_Close(void) 
 * \brief this function will close Tcp Socket needed for communication with Lms151
 *
 * \param  none
 *
* \return Std_ReturnType :
*         - E_NOT_OK if module is not initialized or Tcp socket return an error
*         - else E_OK
 */
Std_ReturnType LibLidarLms151_Close(void) 
{
    /* return code */
    return(gs_LibLidarLms151_status);
}

/**
 * \fn  Std_ReturnType LibLidarLms151_Get_Timestamp(uint32 *pu32_Timestamp)
 * \brief this function will return Laser data Timestamp
 *
 * \param  uint32 *pu32_Timestamp : timestamp parameter
 *
* \return Std_ReturnType :
*         - E_NOT_OK if pu32_Timestamp is null or module is not initialized
*         - else E_OK
 */
Std_ReturnType LibLidarLms151_Get_Timestamp(uint32 *const pu32_Timestamp)
{
    /* Function Body */
    if( (NULL_PTR!=pu32_Timestamp) )
    {
    	*pu32_Timestamp=gu32_LibLidarLms151_Timestamp ;
    }
    /* return code */
    return(gs_LibLidarLms151_status);
}

/**
 * \fn  Std_ReturnType LibLidarLms151_Get_Scan(ts_LibLidarLms151_scan_data *ps_LibLidarLms151_scan_data)
 * \brief this function will return scan values
 *
 * \param  ts_LibLidarLms151_scan_data *ps_LibLidarLms151_scan_data : scan parameter
 *
* \return Std_ReturnType :
*         - E_NOT_OK if ps_LibLidarLms151_scan_data is null or module is not initialized
*         - else E_OK
 */
Std_ReturnType LibLidarLms151_Get_Scan(ts_LibLidarLms151_scan_data *const ps_LibLidarLms151_scan_data)
{
    /* Function Body */
    if((NULL_PTR!=ps_LibLidarLms151_scan_data))
    {
    	memcpy(ps_LibLidarLms151_scan_data,&gs_LibLidarLms151_scan_data,sizeof(ts_LibLidarLms151_scan_data));
    }

    /* return code */
    return(gs_LibLidarLms151_status);
}

/**
 * \fn  Std_ReturnType LibLidarLms151_Get_InternalState(te_InternalStateMachine *pe_State)
 * \brief this function will return internal state
 *
 * \param te_InternalStateMachine *pe_State : state parameter
 *
* \return Std_ReturnType :
*         - E_NOT_OK if pe_State is null or module is not initialized
*         - else E_OK
 */
Std_ReturnType LibLidarLms151_Get_InternalState(te_InternalStateMachine *const pe_State)
{
    /* Function Body */
    if((NULL_PTR!=pe_State))
    {
    	*pe_State=ge_LibLidarLms151_State;
    }

    /* return code */
    return(gs_LibLidarLms151_status);
}

/**
 * \fn  Std_ReturnType LibLidarLms151_Get_Status(te_LibLidarLms151_device_status *pe_LibLidarLms151_device_status)
 * \brief this function will return laser status
 *
 * \param  te_LibLidarLms151_device_status *pe_LibLidarLms151_device_status : device status parameter
 *
* \return Std_ReturnType :
*         - E_NOT_OK if pe_LibLidarLms151_device_status is null or module is not initialized
*         - else E_OK
 */
Std_ReturnType LibLidarLms151_Get_Status(te_LibLidarLms151_device_status *const pe_LibLidarLms151_device_status)
{
    /* Function Body */
    if((NULL_PTR!=pe_LibLidarLms151_device_status))
    {
    	*pe_LibLidarLms151_device_status=ge_LibLidarLms151_device_status;
    }

    /* return code */
    return(gs_LibLidarLms151_status);
}

/**
 * \fn  Std_ReturnType LibLidarLms151_Get_Mode(te_mode *pe_Mode)
 * \brief this function will return global mode
 *
 * \param  te_mode *pe_Mode : mode parameter
 *
* \return Std_ReturnType :
*         - E_NOT_OK if pe_Mode is null or module is not initialized
*         - else E_OK
 */
Std_ReturnType LibLidarLms151_Get_Mode(te_mode *const pe_Mode)
{
    /* Function Body */
    if((NULL_PTR!=pe_Mode))
    {
    	*pe_Mode=ge_LibLidarLms151_Mode;
    }
    /* return code */
    return(gs_LibLidarLms151_status);
}


/**
 * \fn  Std_ReturnType LibLidarLms151_Get_Error(te_LibLidarLms151_error * const pe_LibLidarLms151_error)
 * \brief this function will return module errors
 *
 * \param  te_LibLidarLms151_error * const pe_LibLidarLms151_error : error pointer parameter
 *
* \return Std_ReturnType :
*         - E_NOT_OK if pe_LibLidarLms151_error is null or module is not initialized
*         - else E_OK
 */
Std_ReturnType LibLidarLms151_Get_Error(te_LibLidarLms151_error * const pe_LibLidarLms151_error)
{
    /* Declaration */
    Std_ReturnType t_GetError_Status;

    /* Initialization */

    /* Function Body */
    if(NULL_PTR!=pe_LibLidarLms151_error)
    {
    	*pe_LibLidarLms151_error=ge_LibLidarLms151_error;
    	t_GetError_Status=E_OK;
    }
    else
    {
	/*! Module is not initialized, Halt */
    	t_GetError_Status=E_NOT_OK;
    }

    /* return code */
    return(t_GetError_Status);
}




