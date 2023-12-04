/*============================================================================*/
/*                        	ROBOSOFT                                      */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*/
/*! 
 * $Source: Service_Scheduler.c 
 * $Author: SLoisel 
 * $Date: 2015/12/08  
 */
/*============================================================================*/
/* DESCRIPTION :                                                              */
/** \file  Service_Scheduler.c 
  * \brief Fichier Source du Service Scheduler
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
#include "Service_Scheduler.h" 

/* Constants and types  						      */
/*============================================================================*/ 
   
/* Variables 								      */
/*============================================================================*/ 
static Std_ReturnType gt_InitScheduler_Status; /* Global Initialization variable */

/* Private functions prototypes 					      */
/*============================================================================*/ 

/* Inline functions 							      */
/*============================================================================*/

/* Private functions 							      */
/*============================================================================*/

/* Public functions 							      */
/*============================================================================*/

/**
 * \fn  Std_ReturnType SrvSched_Initialize(void) 
 * \brief this function <TBD>
 *
 * \param  <parameter-name> { parameter description }
 *
* \return Std_ReturnType :
*         - E_OK if good return function
*         - E_NOT_OK if not
 */
Std_ReturnType SrvSched_Initialize(void) 
{
    /* Declaration */     
    Std_ReturnType t_Initialize_Status; 

    /* Initialization */

    /* Function Body */
    gt_InitScheduler_Status=E_OK;
    SrvInit_Initialize();
    SrvAut_Pbit();
	srvConf_Manage();

    /* return code */
    return(t_Initialize_Status);
}

/**
 * \fn  Std_ReturnType SrvSched_Manage(void) 
 * \brief this function <TBD>
 *
 * \param  <parameter-name> { parameter description }
 *
* \return Std_ReturnType :
*         - E_OK if good return function
*         - E_NOT_OK if not
 */
Std_ReturnType SrvSched_Manage(void) 
{
    /* Declaration */     
    Std_ReturnType t_Manage_Status; 

    /* Initialization */

    /* Function Body */
    if(E_NOT_OK!=gt_InitScheduler_Status)
    {
    	t_Manage_Status=E_OK; 
    	SrvAut_Cbit();
    	SrvLidar_Manage();
    	SrvDetect_Manage();
    	SrvErr_Manage();
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
 * \fn  Std_ReturnType SrvSched_Get_Error(void) 
 * \brief this function <TBD>
 *
 * \param  <parameter-name> { parameter description }
 *
* \return Std_ReturnType :
*         - E_OK if good return function
*         - E_NOT_OK if not
 */
Std_ReturnType SrvSched_Get_Error(void) 
{
    /* Declaration */     
    Std_ReturnType t_Get_Error_Status; 

    /* Initialization */

    /* Function Body */
    if(E_NOT_OK!=gt_InitScheduler_Status)
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

