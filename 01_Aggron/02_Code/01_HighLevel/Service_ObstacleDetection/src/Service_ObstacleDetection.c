/*============================================================================*/
/*                        	ROBOSOFT                                      */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*/
/*! 
 * $Source: Service_ObstacleDetection.c 
 * $Author: SLoisel 
 * $Date: 2015/12/08  
 */
/*============================================================================*/
/* DESCRIPTION :                                                              */
/** \file  Service_ObstacleDetection.c 
  * \brief Fichier Source du Service ObstacleDetection
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
#include "Service_ObstacleDetection.h" 

/* Constants and types  						      */
/*============================================================================*/ 
   
/* Variables 								      */
/*============================================================================*/ 
static Std_ReturnType gt_InitObstacleDetection_Status; /* Global Initialization status */
static Std_ReturnType gt_ConfObstacleDetection_Status; /* Global Configuration status */
static ts_Detect_configuration gs_Detect_Conf;		   /* Global Configuration variable */


/* Private functions prototypes 					      */
/*============================================================================*/ 

/* Inline functions 							      */
/*============================================================================*/

/* Private functions 							      */
/*============================================================================*/

/* Public functions 							      */
/*============================================================================*/

/**
 * \fn  Std_ReturnType SrvDetect_Initialize(void) 
 * \brief this function <TBD>
 *
 * \param  <parameter-name> { parameter description }
 *
* \return Std_ReturnType :
*         - E_OK if good return function
*         - E_NOT_OK if not
 */
Std_ReturnType SrvDetect_Initialize(void) 
{
    /* Declaration */     
    Std_ReturnType t_Initialize_Status; 

    /* Initialization */

    /* Function Body */
    if(E_NOT_OK!=gt_InitObstacleDetection_Status)
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
 * \fn  Std_ReturnType SrvDetect_Configure(void)
 * \brief this function <TBD>
 *
 * \param  <parameter-name> { parameter description }
 *
* \return Std_ReturnType :
*         - E_OK if good return function
*         - E_NOT_OK if not
 */
Std_ReturnType SrvDetect_Configure(ts_Detect_configuration const * ps_Detect_Conf)
{
    /* Declaration */
    Std_ReturnType t_Configure_Status;

    /* Initialization */

    /* Function Body */
    if((E_NOT_OK!=gt_InitObstacleDetection_Status)&&
		(NULL_PTR!=ps_Detect_Conf))
    {
    	t_Configure_Status=E_OK;
    	memcpy(&gs_Detect_Conf,ps_Detect_Conf,sizeof(ts_Detect_configuration));
    }
    else
    {
	/*! Module is not initialized, Halt */
    	t_Configure_Status=E_NOT_OK;
    }

    gt_ConfObstacleDetection_Status=t_Configure_Status;

    /* return code */
    return(t_Configure_Status);
}

/**
 * \fn  Std_ReturnType SrvDetect_Manage(void) 
 * \brief this function <TBD>
 *
 * \param  <parameter-name> { parameter description }
 *
* \return Std_ReturnType :
*         - E_OK if good return function
*         - E_NOT_OK if not
 */
Std_ReturnType SrvDetect_Manage(void) 
{
    /* Declaration */     
    Std_ReturnType t_Manage_Status; 

    /* Initialization */

    /* Function Body */
    if((E_NOT_OK!=gt_InitObstacleDetection_Status)&&
		(E_NOT_OK!=gt_ConfObstacleDetection_Status))
    {
    	t_Manage_Status=E_OK; 
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
 * \fn  Std_ReturnType SrvDetect_Get_Error(void) 
 * \brief this function <TBD>
 *
 * \param  <parameter-name> { parameter description }
 *
* \return Std_ReturnType :
*         - E_OK if good return function
*         - E_NOT_OK if not
 */
Std_ReturnType SrvDetect_Get_Error(void) 
{
    /* Declaration */     
    Std_ReturnType t_Get_Error_Status; 

    /* Initialization */

    /* Function Body */
    if(E_NOT_OK!=gt_InitObstacleDetection_Status)
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

