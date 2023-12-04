/*============================================================================*/
/*                        	ROBOSOFT                                      */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*/
/*! 
 * $Source: Service_Configuration.c 
 * $Author: SLoisel 
 * $Date: 2015/12/08  
 */
/*============================================================================*/
/* DESCRIPTION :                                                              */
/** \file  Service_Configuration.c 
  * \brief Fichier Source du Service Configuration
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
#include "Service_Configuration.h" 

/* Constants and types  						      */
/*============================================================================*/ 
   
/* Variables 								      */
/*============================================================================*/ 
static Std_ReturnType gt_InitConfiguration_Status; /* Global Initialization variable */
static ts_Global_Configuration gs_Global_Configuration;

/* Private functions prototypes 					      */
/*============================================================================*/ 

/* Inline functions 							      */
/*============================================================================*/

/* Private functions 							      */
/*============================================================================*/

/* Public functions 							      */
/*============================================================================*/

/**
 * \fn  Std_ReturnType srvConf_Initialize(void) 
 * \brief this function <TBD>
 *
 * \param  <parameter-name> { parameter description }
 *
* \return Std_ReturnType :
*         - E_OK if good return function
*         - E_NOT_OK if not
 */
Std_ReturnType srvConf_Initialize(void) 
{
    /* Declaration */     
    Std_ReturnType t_Initialize_Status; 

    /* Initialization */

    /* Function Body */
    if(E_NOT_OK!=gt_InitConfiguration_Status)
    {
    	t_Initialize_Status=E_OK; 
    }
    else
    {
	/*! Module is not initialized, Halt */
    	t_Initialize_Status=E_NOT_OK; 
    }

    /* return code */
    return(t_Initialize_Status);
}

/**
 * \fn  Std_ReturnType srvConf_Set(void) 
 * \brief this function <TBD>
 *
 * \param  <parameter-name> { parameter description }
 *
* \return Std_ReturnType :
*         - E_OK if good return function
*         - E_NOT_OK if not
 */
Std_ReturnType srvConf_Set(ts_Global_Configuration const * ps_Global_Configuration)
{
    /* Declaration */     
    Std_ReturnType t_Set_Status; 

    /* Initialization */

    /* Function Body */
    if ((E_NOT_OK!=gt_InitConfiguration_Status)&&
		(NULL_PTR!=ps_Global_Configuration))
    {
    	t_Set_Status=E_OK; 
    	memcpy(&gs_Global_Configuration,ps_Global_Configuration,sizeof(ts_Global_Configuration));
    }
    else
    {
	/*! Module is not initialized, Halt */
    	t_Set_Status=E_NOT_OK; 
    }

    /* return code */
    return(t_Set_Status);
}

/**
 * \fn  Std_ReturnType srvConf_Manage(void) 
 * \brief this function <TBD>
 *
 * \param  <parameter-name> { parameter description }
 *
* \return Std_ReturnType :
*         - E_OK if good return function
*         - E_NOT_OK if not
 */
Std_ReturnType srvConf_Manage(void) 
{
    /* Declaration */     
    Std_ReturnType t_Manage_Status; 

    /* Initialization */

    /* Function Body */
    if(E_NOT_OK!=gt_InitConfiguration_Status)
    {
    	t_Manage_Status=E_OK; 
    	SrvLidar_Configure(gs_Global_Configuration.s_Lidar_Conf);
    	SrvDetect_Configure(gs_Global_Configuration.s_Detect_Conf);
    }
    else
    {
	/*! Module is not initialized, Halt */
    	t_Manage_Status=E_NOT_OK; 
    }

    /* return code */
    return(t_Manage_Status);
}

/**
 * \fn  Std_ReturnType srvConf_Get_Error(void) 
 * \brief this function <TBD>
 *
 * \param  <parameter-name> { parameter description }
 *
* \return Std_ReturnType :
*         - E_OK if good return function
*         - E_NOT_OK if not
 */
Std_ReturnType srvConf_Get_Error(void) 
{
    /* Declaration */     
    Std_ReturnType t_Get_Error_Status; 

    /* Initialization */

    /* Function Body */
    if(E_NOT_OK!=gt_InitConfiguration_Status)
    {
    	t_Get_Error_Status=E_OK; 
    }
    else
    {
	/*! Module is not initialized, Halt */
    	t_Get_Error_Status=E_NOT_OK; 
    }

    /* return code */
    return(t_Get_Error_Status);
}

