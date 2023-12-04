/*============================================================================*/
/*                        	ROBOSOFT                                      */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*/
/*! 
 * $Source: Library_LibLidarLms151.h 
 * $Author: Slo
 * $Date: 2016/01/14
 */
/*============================================================================*/
/* DESCRIPTION :                                                              */
/** \file  Library_LibLidarLms151.h 
  * \brief Fichier Header du Service LibLidarLms151
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

/* Includes 								      */
/*============================================================================*/

/* Constants and types 							      */
/*============================================================================*/

/**
 * @enum  te_liblms151_errors
 * @brief define the errors for lms151
 */
typedef enum
{
	E_LIBLMS151_NO_ERROR = 0,  					/* no Error */
	E_LIBLMS151_FREQUENCY_ERROR = 1, 			/* Frequency Error */
	E_LIBLMS151_RESOLUTION_ERROR = 2,  			/* Resolution Error */
	E_LIBLMS151_RESOLUTION_AND_SCAN_ERROR = 3,  /* Resolution and scan Error */
	E_LIBLMS151_SCAN_AREA_ERROR = 4,  			/* Scan area Error */
	E_LIBLMS151_OTHER_ERROR =5    				/* other Errors */
}te_liblms151_errors;

///* Lidar LMS151 Configuration Values */
//#define K_LMS151_SERVER_ADDRESS 					(uint8 *)"192.168.001.030"
//#define K_LMS151_PORT_SERVER 						2111
//#define K_LMS151_BLOCKING_MODE_ACTIVATED 			K_FALSE
#define K_TCPSOCKET_SERVER_ADDRESS_MAX_LEN		    32
//
//static const sint32 gs32_angle_min=-420000;
//static const sint32 gs32_angle_max=2100000;
//static const uint32 gu32_angular_resolution_0_25_deg=2500;
//static const uint32 gu32_angular_resolution_0_5_deg=5000;
//static const uint32 gu32_frequency_25_Hz=2500;
//static const uint32 gu32_frequency_50_Hz=5000;

/**
 * @enum  ts_LibLidarLms151_socket
 * @brief define the socket lidar configuration for lms151
 */
typedef struct {
	uint8 au8_serverAddress[K_TCPSOCKET_SERVER_ADDRESS_MAX_LEN] ; 		/* u8_serverAddress format is "192.168.1.30" */
	uint32 u32_server_port ;		/* port server */
	bool b_blocking_mode ; 			/* socket blocking mode */
	uint32 u32_timeout_sec ; 		/* socket timeout */
} ts_LibLidarLms151_socket ;

/**
 * @enum  ts_LibLidarLms151_conf
 * @brief define the lidar configuration for lms151
 */
typedef struct {
	uint32 u32_scaning_frequency ; 		/* Scanning frequency, 25 Hz or 50Hz */
	uint32 u32_angle_resolution ;		/* angle resolution in 1/10000 degree */
	sint32 s32_start_angle ; 			/* start angle in 1/10000 degree */
	sint32 s32_stop_angle ; 			/* stop angle in 1/10000 degree */
} ts_LibLidarLms151_laserconf ;

typedef enum
{
	E_LMS151_8_BIT = 0,
	E_LMS151_16_BIT = 1
}te_LibLidarLms151_resolution;

typedef enum
{
	E_LMS151_NO_ENCODER = 0,
	E_LMS151_CHANNEL_1 = 0x100
}te_LibLidarLms151_encoder;

/**
 * @struct  ts_LibLidarLms151_dataconf
 * @brief define the lidar data configuration for lms151
 */
typedef struct {
	uint8 						 u8_data_channel_0  ; 		/* Output Channel */
	uint8 						 u8_data_channel_1  ; 		/* Output Channel */
	bool						 b_remission;				/* Remission data output */
	te_LibLidarLms151_resolution e_resolution;				/* Resolution in 8 bit or 16 bit */
	uint8 						 u8_unit;					/* Encoder Data */
	uint8 						 u8_encoder_0;				/* Encoder Data */
	uint8 						 u8_encoder_1;				/* Encoder Data */
	bool 						 b_position;				/* Position values */
	bool 						 b_device_name;				/* Device name */
	bool 						 b_comment;					/* Comment */
	bool 						 b_time;					/* Sends time information */
	uint8 						 au8_output_rate[3];			/* Sends output rate */
} ts_LibLidarLms151_dataconf ;

/**
 * @enum  ts_LibLidarLms151_Configuration
 * @brief define the Lidar lms151 library Configuration
 */
typedef struct
{
	ts_LibLidarLms151_socket s_LibLidarLms151_socket;
	ts_LibLidarLms151_laserconf s_LibLidarLms151_laserconf;
	ts_LibLidarLms151_dataconf s_LibLidarLms151_dataconf;
}ts_LibLidarLms151_Configuration;


/// @brief Describe LMS151 possible statuses
typedef enum {
	E_LIBLMS151_STATUS_UNDEFINED = 0,
	E_LIBLMS151_STATUS_INITIALIZATION = 1,
	E_LIBLMS151_STATUS_CONFIGURATION = 2,
	E_LIBLMS151_STATUS_IDLE = 3,
	E_LIBLMS151_STATUS_ROTATED = 4,
	E_LIBLMS151_STATUS_IN_PREPARATION = 5,
	E_LIBLMS151_STATUS_READY = 6,
	E_LIBLMS151_STATUS_READY_FOR_MEASUREMENT = 7
} te_LibLidarLms151_device_status;

typedef enum
{
	E_STATE_LMS151_INITIALIZATION = 0,
	E_STATE_LMS151_UNDEFINED,
	E_STATE_LMS151_CONFIGURATION,
	E_STATE_LMS151_READY,
	E_STATE_LMS151_SCAN
}te_InternalStateMachine;

typedef enum {
	E_MODE_UNDEFINED = 0,
	E_MODE_LOGIN,
	E_MODE_SET_FREQUENCY_AND_RESOLUTION,
	E_MODE_CONFIGURE_SCAN_DATA_CONTENT,
	E_MODE_CONFIGURE_SCAN_DATA_OUTPUT,
	E_MODE_STORE_PARAMETERS,
	E_MODE_SET_TIMESTAMP,
	E_MODE_LOGOUT,
	E_MODE_ASK_STATUS,
	E_MODE_SCAN_CONTINUOUS,
	E_MODE_ONE_SCAN,
	E_MODE_GET_DATA,
	E_MODE_START_MEAS,
}te_mode;

/// @brief Structure containing single scan message.
typedef struct
{
  /// @brief Number of samples in dist1.
  uint16 u16_dist_len1;
  /// @brief Radial distance for the first reflected pulse
  uint16 u16_dist1[1082];
  /// @brief Number of samples in dist2.
  uint16 u16_dist_len2;
  /// @brief Radial distance for the second reflected pulse
  uint16 u16_dist2[1082];
  /// @brief Number of samples in rssi1.
  uint16 u16_rssi_len1;
  /// @brief Remission values for the first reflected pulse
  uint16 u16_rssi1[1082];
  /// @brief Number of samples in rssi2.
  uint16 u16_rssi_len2;
  /// @brief Remission values for the second reflected pulse
  uint16 u16_rssi2[1082];
  /// @brief Timestamp of read data
  bool b_is_new_data;
}ts_LibLidarLms151_scan_data;

/**
 * @enum te_LibLidarLms151_error
 * @brief define the LMS151 error
 */
typedef enum
{
	E_LIBLMS151_ERR_NO_ERROR=0,
	E_LIBLMS151_ERR_NOT_INITIALIZED,
	E_LIBLMS151_ERR_NOT_CONFIGURED,
	E_LIBLMS151_ERR_SCAN_CONTINUOUS,
	E_LIBLMS151_ERR_LOGIN,
	E_LIBLMS151_ERR_SET_FREQUENCY_AND_RESOLUTION,
	E_LIBLMS151_ERR_CONFIGURE_SCAN_DATA_CONTENT,
	E_LIBLMS151_ERR_CONFIGURE_SCAN_DATA_OUTPUT,
	E_LIBLMS151_ERR_STORE_PARAMETERS,
	E_LIBLMS151_ERR_SET_TIMESTAMP,
	E_LIBLMS151_ERR_LOGOUT,
	E_LIBLMS151_ERR_ASK_STATUS,
	E_LIBLMS151_ERR_ONE_SCAN,
	E_LIBLMS151_ERR_GET_DATA,
	E_LIBLMS151_ERR_START_MEAS
} te_LibLidarLms151_error ;

/* Exported Variables 							      */
/*============================================================================*/

/* Exported functions prototypes 					      */
/*============================================================================*/    
 
Std_ReturnType LibLidarLms151_Initialize(void);
Std_ReturnType LibLidarLms151_Configure(const ts_LibLidarLms151_Configuration * const ps_LibLidarLms151_Configuration);
Std_ReturnType LibLidarLms151_Open(void);
Std_ReturnType LibLidarLms151_Close(void);
Std_ReturnType LibLidarLms151_Manage(void);
Std_ReturnType LibLidarLms151_Get_Timestamp(uint32 *const pu32_Timestamp);
Std_ReturnType LibLidarLms151_Get_Scan(ts_LibLidarLms151_scan_data *const ps_LibLidarLms151_scan_data);
Std_ReturnType LibLidarLms151_Get_Status(te_LibLidarLms151_device_status *const pe_LibLidarLms151_device_status);
Std_ReturnType LibLidarLms151_Get_InternalState(te_InternalStateMachine *const pe_State);
Std_ReturnType LibLidarLms151_Get_Mode(te_mode *const pe_Mode);
Std_ReturnType LibLidarLms151_Get_Error(te_LibLidarLms151_error * const pe_LibLidarLms151_error);
