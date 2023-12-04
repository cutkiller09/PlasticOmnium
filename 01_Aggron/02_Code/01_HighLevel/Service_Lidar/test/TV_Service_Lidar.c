/*============================================================================*/
/*                              ROBOSOFT                                      */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*/
/*! 
 * $Source: TI_Service_Lidar.c 
 * $Author: MDega 
 * $Date: 2015/12/08  
 */
/*============================================================================*/
/* DESCRIPTION :                                                              */
/** \file  TV_Service_Lidar.c
  * \brief Fichier Source file of validation Test Lidar
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

/* Includes                                                                   */
/*============================================================================*/ 
#include "types.h"
#include "Library_StdLib.h"
#include "Service_Lidar.h"
#include "Driver_TcpSocketDriver.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/* Constants and types                                                        */
/*============================================================================*/  
#define K_LMS151_SERVER_ADDRESS 					(uint8 *)"192.168.0.40"
#define K_LMS151_PORT_SERVER 						2111
#define K_LMS151_BLOCKING_MODE_ACTIVATED 			K_FALSE
#define K_LMS151_ALL_SCANS 							"+1"

/* Variables                                                                  */
/*============================================================================*/ 
static const sint32 gs32_angle_min=-450000;
static const sint32 gs32_angle_max=2250000;
static const uint32 gu32_angular_resolution_0_25_deg=2500;
static const uint32 gu32_angular_resolution_0_5_deg=5000;
static const uint32 gu32_frequency_25_Hz=2500;
static const uint32 gu32_frequency_50_Hz=5000;

/* Private functions prototypes                                               */
/*============================================================================*/ 
static void makeTraceFile( char* filename , const ts_SrvLidar_lidarData * const ps_data ) ;

/* Inline functions                                                           */
/*============================================================================*/

/* Private functions                                                          */
/*============================================================================*/
void makeTraceFile( char* filename , const ts_SrvLidar_lidarData * const ps_data )
{
	FILE* fd = NULL_PTR ;
	uint32 u32_idx ;
	uint16 u16_j ;
	float32 f32_angle ;
	float32 f32_angle_step ;

	if( (NULL_PTR != filename) && (NULL_PTR != ps_data) )
	{
		u32_idx = 0 ;
		f32_angle = 270-45 ;
		f32_angle_step = 1.0 / (ps_data->u16_angular_tick_per_revolution/360.0) ;

		if( 0 < ps_data->u16_impact_count )
		{
			fd = fopen( filename , "wb") ;

			if( fd  != NULL_PTR )
			{
				u32_idx++ ;
				for( u16_j = 0 ; u16_j < ps_data->u16_impact_count ; u16_j++ )
				{
					fprintf( fd , "%d %.05f %.03f \n" , u32_idx , f32_angle , ps_data->f32_distance_list[u16_j] ) ;
					f32_angle = f32_angle - f32_angle_step ;
				}
				fprintf( fd , "\n" ) ;
			}
			if( NULL_PTR != fd )
			{
				fclose( fd ) ;
			}
		}
	}
}

/* Public functions 							      */
/*============================================================================*/
/**
 * \fn  Std_ReturnType main(void)  
 * \brief this function is the entry point of the integration test
 *
 * \param  
 *
* \return Std_ReturnType :
*         - E_OK if good return function
*         - E_NOT_OK if not
 */
int main(void)
{
    /*! Declaration */     
    Std_ReturnType t_main_Status; 
    ts_SrvLidar_configuration s_configuration ;
    ts_SrvLidar_data s_data ;
    uint16 u16_i ;
    uint32 u32_last_time_lms_s ;
	uint32 u32_last_time_lms_ns ;

    /*! Initialization */ 
    t_main_Status = E_OK;
    u32_last_time_lms_s = 0 ;
	u32_last_time_lms_ns = 0 ;

	s_configuration.s_lms_Configuration.s_LibLidarLms151_dataconf.b_comment=K_FALSE;
	s_configuration.s_lms_Configuration.s_LibLidarLms151_dataconf.b_device_name=K_TRUE;
	s_configuration.s_lms_Configuration.s_LibLidarLms151_dataconf.b_position=K_TRUE;
	s_configuration.s_lms_Configuration.s_LibLidarLms151_dataconf.b_remission=K_TRUE;
	s_configuration.s_lms_Configuration.s_LibLidarLms151_dataconf.b_time=K_TRUE;
	s_configuration.s_lms_Configuration.s_LibLidarLms151_dataconf.e_resolution=E_LMS151_16_BIT;
	strcpy((char *)s_configuration.s_lms_Configuration.s_LibLidarLms151_dataconf.au8_output_rate,(const char *)K_LMS151_ALL_SCANS);
	s_configuration.s_lms_Configuration.s_LibLidarLms151_dataconf.u8_data_channel_0=3; // Output channel 1+2: 03 00
	s_configuration.s_lms_Configuration.s_LibLidarLms151_dataconf.u8_data_channel_1=0;
	s_configuration.s_lms_Configuration.s_LibLidarLms151_dataconf.u8_encoder_0=0;
	s_configuration.s_lms_Configuration.s_LibLidarLms151_dataconf.u8_encoder_1=0;
	s_configuration.s_lms_Configuration.s_LibLidarLms151_dataconf.u8_unit=0;
	s_configuration.s_lms_Configuration.s_LibLidarLms151_laserconf.s32_start_angle=gs32_angle_min;
	s_configuration.s_lms_Configuration.s_LibLidarLms151_laserconf.s32_stop_angle=gs32_angle_max;
	s_configuration.s_lms_Configuration.s_LibLidarLms151_laserconf.u32_angle_resolution=gu32_angular_resolution_0_25_deg;
	s_configuration.s_lms_Configuration.s_LibLidarLms151_laserconf.u32_scaning_frequency=gu32_frequency_25_Hz;
	s_configuration.s_lms_Configuration.s_LibLidarLms151_socket.b_blocking_mode=K_LMS151_BLOCKING_MODE_ACTIVATED;
	s_configuration.s_lms_Configuration.s_LibLidarLms151_socket.u32_server_port=(uint32)K_LMS151_PORT_SERVER;
	s_configuration.s_lms_Configuration.s_LibLidarLms151_socket.u32_timeout_sec=100;
	strcpy((char *)s_configuration.s_lms_Configuration.s_LibLidarLms151_socket.au8_serverAddress,(const char *)K_LMS151_SERVER_ADDRESS);

    /*! Function Body */
    printf( "/******************** TV_Service_Lidar *********************** \n" ) ;
    printf( " * Hardware request :                                        * \n" ) ;
    printf( " *  - USB/Serial cable                                       * \n" ) ;
    printf( " *  - Lidar S300                                             * \n" ) ;
    printf( " *  - Lidar UST10-LX                                         * \n" ) ;
    printf( " *  - Lidar LMS151                                           * \n" ) ;
    printf( " * Software request :                                        * \n" ) ;
    printf( " *   - LMS151 conf : Address %s , port %d         * \n" , K_LMS151_SERVER_ADDRESS , K_LMS151_PORT_SERVER ) ;
    printf( " *************************************************************/ \n\n" ) ;
    DrvTcpSocket_Initialize() ;

	t_main_Status |= SrvLidar_Initialize() ;
	t_main_Status |= SrvLidar_Configure( &s_configuration ) ;

	for( u16_i = 0 ; (u16_i < 7000) && (E_NOT_OK != t_main_Status) ; u16_i++ )
	{
		usleep( 10*1000 ) ;

		t_main_Status |= SrvLidar_Manage() ;
		t_main_Status |= SrvLidar_Get_Data( &s_data ) ;


		/* LMS151 */
		if( (s_data.s_lms151.u32_timestamp_sec != u32_last_time_lms_s) || (s_data.s_lms151.u32_timestamp_ns != u32_last_time_lms_ns) )
		{
			printf( "LMS151 data  : %u.%09u %d %u \n" , s_data.s_lms151.u32_timestamp_sec , s_data.s_lms151.u32_timestamp_ns ,
								s_data.s_lms151.s16_angle_offset ,
								s_data.s_lms151.u16_impact_count ) ;
			makeTraceFile( "./_LMS151_scandata.log" , &(s_data.s_lms151) ) ;
		}

		u32_last_time_lms_s		= s_data.s_lms151.u32_timestamp_sec ;
		u32_last_time_lms_ns	= s_data.s_lms151.u32_timestamp_ns ;

	}


	printf( "Exit... %d \n" , t_main_Status ) ;

    /*! return code */ 
    return(t_main_Status);
} 
