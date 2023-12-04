/*============================================================================*/
/*                        	ROBOSOFT                                      */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*/
/*! 
 * $Source: Service_ObstacleDetection.h 
 * $Author: SLoisel 
 * $Date: 2015/12/08  
 */
/*============================================================================*/
/* DESCRIPTION :                                                              */
/** \file  Service_ObstacleDetection.h 
  * \brief Fichier Header du Service ObstacleDetection
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

/* Constants and types 							      */
/*============================================================================*/  
/**
 * @enum  ts_Detect_configuration
 * @brief define the Obstacle Detection Configuration
 */
typedef struct
{
	uint32 u32_ObjectSize;			/*! Object size detected */
	uint32 u32_ObjectDuration;		/*! Object duration detected */
}ts_Detect_configuration;

/* Exported Variables 						              */
/*============================================================================*/

/* Exported functions prototypes 					      */
/*============================================================================*/    
 
Std_ReturnType SrvDetect_Initialize(void) ;
Std_ReturnType SrvDetect_Configure(void) ;
Std_ReturnType SrvDetect_Manage(void) ;
Std_ReturnType SrvDetect_Get_Error(void) ;
