/*============================================================================*/
/*                        	ROBOSOFT                                      */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*/
/*! 
 * $Source: Service_Configuration.h 
 * $Author: SLoisel 
 * $Date: 2015/12/08  
 */
/*============================================================================*/
/* DESCRIPTION :                                                              */
/** \file  Service_Configuration.h 
  * \brief Fichier Header du Service Configuration
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

#pragma once

/* Includes 							              */
/*============================================================================*/  
#include "Service_ObstacleDetection.h"
#include "Service_Lidar.h"

/* Constants and types 							      */
/*============================================================================*/  

/**
 * @enum  ts_Global_Configuration
 * @brief define the Global Configuration
 */
typedef struct
{
	ts_SrvLidar_configuration 			s_Lidar_Conf;
	ts_Detect_configuration 			s_Detect_Conf;
}ts_Global_Configuration;

/**
 * @enum te_SrvConf_error
 * @brief define the Configuration error
 */
typedef enum
{
	E_SRVCONF_ERR_NO_ERROR=0,
	E_SRVCONF_ERR_NOT_INITIALIZED,
	E_SRVCONF_ERR_NOT_CONFIGURED
} te_SrvConf_error ;

/* Exported Variables 						              */
/*============================================================================*/

/* Exported functions prototypes 					      */
/*============================================================================*/    
 
Std_ReturnType srvConf_Initialize(void) ;
Std_ReturnType srvConf_Set(void) ;
Std_ReturnType srvConf_Manage(void) ;
Std_ReturnType srvConf_Get_Error(void) ;
