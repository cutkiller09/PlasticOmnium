/*============================================================================*/
/*                        	ROBOSOFT                                      */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*/
/*! 
 * $Source: Service_Lidar.h 
 * $Author: MDega 
 * $Date: 2015/12/08  
 */
/*============================================================================*/
/* DESCRIPTION :                                                              */
/** \file  Service_Lidar.h 
  * \brief Fichier Header du Service Lidar
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
#include "types.h"
#include "Library_StdLib.h"
#include "Library_LibLidarLms151.h"

/* Constants and types 							      */
/*============================================================================*/
#define K_SRVLIDAR_IMPACT_COUNT_MAX 1801

typedef enum
{
	E_STATE_SRVLIDAR_UNDEFINED = 0,
	E_STATE_SRVLIDAR_INITIALIZATION,
	E_STATE_SRVLIDAR_READY,
	E_STATE_SRVLIDAR_CLOSE
} te_SrvLidar_StateMachine;

typedef struct
{
	/* LMS151 Configuration */
	ts_LibLidarLms151_Configuration s_lms_Configuration;
} ts_SrvLidar_configuration ;


/* LIDAR data provided to the robotic application */
typedef struct
{
	uint32 u32_timestamp_sec;		/* Resolution: 1s (time from 1/1/1970 0:0:0 UTC) */
	uint32 u32_timestamp_ns;		/* Resolution: 1ns (number of nano-seconds into second */

	uint8	u8_sensor_status ;
	sint16	s16_angle_offset ;		/* Unit: tick */
	uint16	u16_angular_tick_per_revolution ;
	uint16	u16_impact_count ;
	float32 f32_distance_list[K_SRVLIDAR_IMPACT_COUNT_MAX] ;	/* Unit: m */
} ts_SrvLidar_lidarData ;

typedef struct
{
	/* S300 sensor data */
	ts_SrvLidar_lidarData s_S300 ;
	/* Hokuyo sensor data */
	ts_SrvLidar_lidarData s_ust1020lx ;
	/* LMS151 sensor data */
	ts_SrvLidar_lidarData s_lms151 ;
} ts_SrvLidar_data ;

/**
 * @enum te_SrvLidar_error
 * @brief define the Lidar error
 */
typedef enum
{
	E_SRVLIDAR_ERR_NO_ERROR=0,
	E_SRVLIDAR_ERR_NOT_INITIALIZED,
	E_SRVLIDAR_ERR_AT_LEAST_ONE_LIDAR_BADLY_INITIALIZED,
	E_SRVLIDAR_ERR_AT_LEAST_ONE_LIDAR_BADLY_OPENED,
	E_SRVLIDAR_ERR_S300_ERROR_MANAGE,
	E_SRVLIDAR_ERR_UST1020LX_ERROR_MANAGE,
	E_SRVLIDAR_ERR_LMS151_ERROR_MANAGE
} te_SrvLidar_error ;


/* Exported Variables 						              */
/*============================================================================*/

/* Exported functions prototypes 					      */
/*============================================================================*/    
 
Std_ReturnType SrvLidar_Initialize(void) ;
Std_ReturnType SrvLidar_Configure( const ts_SrvLidar_configuration * const ps_configuration ) ;
Std_ReturnType SrvLidar_Manage(void) ;
Std_ReturnType SrvLidar_Get_Data( ts_SrvLidar_data * const ps_data ) ;
Std_ReturnType SrvLidar_Get_State( te_SrvLidar_StateMachine * const pe_state );
Std_ReturnType SrvLidar_Close(void) ;
Std_ReturnType SrvLidar_Get_Error(te_SrvLidar_error *pe_SrvLidar_error );
