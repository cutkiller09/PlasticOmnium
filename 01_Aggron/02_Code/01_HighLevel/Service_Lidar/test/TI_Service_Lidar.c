/*============================================================================*/
/*                        	ROBOSOFT                                      */
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
/** \file  TI_Service_Lidar.c 
  * \brief Fichier Source file of Integration Test Lidar
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


#define MACRO_START_UT(u16_num_case, sz_title_case) printf( "\nCase %d : \\************ %s ************\\ \n" , u16_num_case , sz_title_case ) ;
#define MACRO_ASSERT_UT(u16_num_case , u16_error_count , u16_num_error_expected ) \
		printf( "Case %d : tested_value = %d (expected : %d) : %s\n" , \
				u16_num_case , \
				u16_error_count , \
				u16_num_error_expected , \
				(u16_num_error_expected==u16_error_count)?"SUCCESS":"FAILED" ) ;

/* Includes 								      */
/*============================================================================*/ 
#include "types.h"
#include "Library_StdLib.h"
#include "Service_Lidar.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/* Constants and types  						      */
/*============================================================================*/  
#define K_LMS151_SERVER_ADDRESS 					(uint8 *)"192.168.1.30"
#define K_LMS151_PORT_SERVER 						2111
#define K_LMS151_BLOCKING_MODE_ACTIVATED 			K_FALSE
#define K_LMS151_ALL_SCANS "+1"

/* Variables 								      */
/*============================================================================*/ 
static const sint32 gs32_angle_min=-450000;
static const sint32 gs32_angle_max=2250000;
static const uint32 gu32_angular_resolution_0_25_deg=2500;
static const uint32 gu32_angular_resolution_0_5_deg=5000;
static const uint32 gu32_frequency_25_Hz=2500;
static const uint32 gu32_frequency_50_Hz=5000;

/* Private functions prototypes 					      */
/*============================================================================*/ 

/* Inline functions 							      */
/*============================================================================*/

/* Private functions 							      */
/*============================================================================*/

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
    te_SrvLidar_StateMachine e_stateMachine ;
    ts_SrvLidar_data s_data ;
    uint16 u16_i ;
    uint16 u16_j ;
    te_SrvLidar_error e_error ;

    /*! Initialization */

	strcpy( s_configuration.s_s300_configuration.s_serial_configuration.sz_serialport_name , "/dev/ttyUSB0" ) ;
	s_configuration.s_s300_configuration.s_serial_configuration.u32_baudrate = 115200 ;
	s_configuration.s_s300_configuration.s_serial_configuration.u8_bitsize = 8 ;
	s_configuration.s_s300_configuration.s_serial_configuration.t_parity_mode = E_DRVSERIAL_NO_PARITY ;
	s_configuration.s_s300_configuration.s_serial_configuration.b_stopbitena = K_TRUE ;
	s_configuration.s_s300_configuration.e_orientation = E_LIBLIDARS300_ORIENTATION_NORMAL ;

	strcpy( (char*)s_configuration.s_ust_configuration.s_socket_configuration.au8_serverAddress , "192.168.1.10" ) ;
	s_configuration.s_ust_configuration.s_socket_configuration.u32_server_port = 10940 ;
	s_configuration.s_ust_configuration.s_socket_configuration.b_blocking_mode = K_FALSE ;
	s_configuration.s_ust_configuration.s_socket_configuration.u32_timeout_sec = 0 ;
	s_configuration.s_ust_configuration.s_settings.b_asynch_mode 	= K_TRUE ;
	s_configuration.s_ust_configuration.s_settings.u16_start_step 	= 0 ;
	s_configuration.s_ust_configuration.s_settings.u16_stop_step 	= 1080 ;
	s_configuration.s_ust_configuration.s_settings.u8_cluster_count	= 1 ;
	s_configuration.s_ust_configuration.s_settings.u8_scan_interval	= 0 ;
	s_configuration.s_ust_configuration.s_settings.u8_numb_of_scan 	= 0 ;
	s_configuration.s_ust_configuration.e_orientation = E_LIBLIDARUST1020LX_ORIENTATION_NORMAL ;


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

	u16_i = 0 ;
	gs_LibLidarLms151_status = E_OK ;
	gs_LibLidarS300_status = E_OK ;
	gs_LibLidarUst1020lx_status = E_OK ;

    /*! Function Body */

	/***************************************************************************************/
	/* CASE 1 : Test call all functions if service not initialized */
	MACRO_START_UT( u16_i , "Test call all functions if service not initialized" )

	t_main_Status = SrvLidar_Get_Error( &e_error );
	MACRO_ASSERT_UT( u16_i , t_main_Status , E_OK )
	MACRO_ASSERT_UT( u16_i , e_error , E_SRVLIDAR_ERR_NOT_INITIALIZED )

	t_main_Status = SrvLidar_Configure( &s_configuration ) ;
	MACRO_ASSERT_UT( u16_i , t_main_Status , E_NOT_OK )
	t_main_Status = SrvLidar_Get_State( &e_stateMachine ) ;
	MACRO_ASSERT_UT( u16_i , t_main_Status , E_OK )
	MACRO_ASSERT_UT( u16_i , e_stateMachine , E_STATE_SRVLIDAR_UNDEFINED )
	t_main_Status = SrvLidar_Manage() ;
	MACRO_ASSERT_UT( u16_i , t_main_Status , E_NOT_OK )
	t_main_Status = SrvLidar_Get_Data( &s_data ) ;
	MACRO_ASSERT_UT( u16_i , t_main_Status , E_NOT_OK )
	t_main_Status = SrvLidar_Close() ;
	MACRO_ASSERT_UT( u16_i , t_main_Status , E_NOT_OK )

	/***************************************************************************************/
	/* CASE 2 : Test initialize failed */
	u16_i++ ;
	MACRO_START_UT( u16_i , "Test initialize failed" )
	gs_LibLidarUst1020lx_status = E_NOT_OK ;
	ge_LibLidarUst1020lx_error = E_LIBLIDARUST1020LX_ERR_NOT_INITIALIZED ;
	ge_LibLidarS300_error = E_LIBLIDARS300_NO_INIT_ERROR ;
	ge_LibLidarLms151_error = E_LIBLMS151_ERR_NOT_INITIALIZED ;

	t_main_Status = SrvLidar_Initialize() ;
	MACRO_ASSERT_UT( u16_i, t_main_Status , E_NOT_OK )
	t_main_Status = SrvLidar_Configure( &s_configuration ) ;
	MACRO_ASSERT_UT( u16_i , t_main_Status , E_NOT_OK )
	t_main_Status = SrvLidar_Get_State( &e_stateMachine ) ;
	MACRO_ASSERT_UT( u16_i , t_main_Status , E_OK )
	MACRO_ASSERT_UT( u16_i , e_stateMachine , E_STATE_SRVLIDAR_UNDEFINED )
	t_main_Status = SrvLidar_Manage() ;
	MACRO_ASSERT_UT( u16_i , t_main_Status , E_NOT_OK )
	t_main_Status = SrvLidar_Get_Data( &s_data ) ;
	MACRO_ASSERT_UT( u16_i , t_main_Status , E_NOT_OK )
	t_main_Status = SrvLidar_Close() ;
	MACRO_ASSERT_UT( u16_i , t_main_Status , E_NOT_OK )
	gs_LibLidarUst1020lx_status = E_OK ;

	/***************************************************************************************/
	/* CASE 2 : Test call all function if driver is initialized but not configured */
	u16_i++ ;
	MACRO_START_UT( u16_i , "Test call all function if driver is initialized but not configured" )

	t_main_Status = SrvLidar_Initialize() ;
	MACRO_ASSERT_UT( u16_i, t_main_Status , E_OK )
	t_main_Status = SrvLidar_Get_State( &e_stateMachine ) ;
	MACRO_ASSERT_UT( u16_i , t_main_Status , E_OK )
	MACRO_ASSERT_UT( u16_i , e_stateMachine , E_STATE_SRVLIDAR_UNDEFINED )
	t_main_Status = SrvLidar_Manage() ;
	MACRO_ASSERT_UT( u16_i , t_main_Status , E_NOT_OK )
	t_main_Status = SrvLidar_Get_Data( &s_data ) ;
	MACRO_ASSERT_UT( u16_i , t_main_Status , E_NOT_OK )
	t_main_Status = SrvLidar_Close() ;
	MACRO_ASSERT_UT( u16_i , t_main_Status , E_NOT_OK )

	/***************************************************************************************/
	/* CASE 3 : Test NULL_PTR */
	u16_i++ ;
	MACRO_START_UT( u16_i , "Test NULL_PTR" )

	t_main_Status = SrvLidar_Configure( NULL_PTR ) ;
	MACRO_ASSERT_UT( u16_i , t_main_Status , E_NOT_OK )
	t_main_Status = SrvLidar_Get_State( NULL_PTR ) ;
	MACRO_ASSERT_UT( u16_i , t_main_Status , E_NOT_OK )
	t_main_Status = SrvLidar_Get_Data( NULL_PTR ) ;
	MACRO_ASSERT_UT( u16_i , t_main_Status , E_NOT_OK )
	t_main_Status = SrvLidar_Get_Error( NULL_PTR );
	MACRO_ASSERT_UT( u16_i , t_main_Status , E_NOT_OK )

	/***************************************************************************************/
	/* CASE 4 : Configuration failed */
	u16_i++ ;
	MACRO_START_UT( u16_i , "Configuration failed" )

	gs_LibLidarUst1020lx_status = E_NOT_OK ;
	ge_LibLidarUst1020lx_error = E_LIBLIDARUST1020LX_ERR_NOT_OPENED ;
	ge_LibLidarS300_error = E_LIBLIDARS300_OPEN_FAILED_ERROR ;
	ge_LibLidarLms151_error = E_LIBLMS151_ERR_CONFIGURE_SCAN_DATA_CONTENT ;

	t_main_Status = SrvLidar_Get_State( &e_stateMachine ) ;
	MACRO_ASSERT_UT( u16_i , t_main_Status , E_OK )
	MACRO_ASSERT_UT( u16_i , e_stateMachine , E_STATE_SRVLIDAR_UNDEFINED )

	t_main_Status = SrvLidar_Configure( &s_configuration ) ;
	MACRO_ASSERT_UT( u16_i , t_main_Status , E_NOT_OK )

	t_main_Status = SrvLidar_Get_State( &e_stateMachine ) ;
	MACRO_ASSERT_UT( u16_i , t_main_Status , E_OK )
	MACRO_ASSERT_UT( u16_i , e_stateMachine , E_STATE_SRVLIDAR_UNDEFINED )

	t_main_Status = SrvLidar_Manage() ;
	MACRO_ASSERT_UT( u16_i , t_main_Status , E_NOT_OK )

	gs_LibLidarUst1020lx_status = E_OK ;

	/***************************************************************************************/
	/* CASE 5 : Test normal case */
	u16_i++ ;
	MACRO_START_UT( u16_i , "Test normal case" )

	t_main_Status = SrvLidar_Get_State( &e_stateMachine ) ;
	MACRO_ASSERT_UT( u16_i , t_main_Status , E_OK )
	MACRO_ASSERT_UT( u16_i , e_stateMachine , E_STATE_SRVLIDAR_UNDEFINED )

	t_main_Status = SrvLidar_Configure( &s_configuration ) ;
	MACRO_ASSERT_UT( u16_i , t_main_Status , E_OK )

	t_main_Status = SrvLidar_Get_State( &e_stateMachine ) ;
	MACRO_ASSERT_UT( u16_i , t_main_Status , E_OK )
	MACRO_ASSERT_UT( u16_i , e_stateMachine , E_STATE_SRVLIDAR_READY )

	/***************************************************************************************/
	/* CASE 5 : Test normal case : no datas */
	u16_i++ ;
	MACRO_START_UT( u16_i , "Test normal case : no datas" )

	gs_LibLidarS300_data.b_is_new_data = K_FALSE ;
	gs_LibLidarUst1020lx_data.b_is_new_data = K_FALSE ;
	gs_LibLidarLms151_scan_data.b_is_new_data = K_FALSE ;
	t_main_Status = SrvLidar_Manage() ;
	MACRO_ASSERT_UT( u16_i , t_main_Status , E_OK )

	t_main_Status = SrvLidar_Get_Data( &s_data ) ;
	MACRO_ASSERT_UT( u16_i , t_main_Status , E_OK )
	MACRO_ASSERT_UT( u16_i , s_data.s_S300.u32_timestamp_sec , 0 )
	MACRO_ASSERT_UT( u16_i , s_data.s_S300.u32_timestamp_ns , 0 )
	MACRO_ASSERT_UT( u16_i , s_data.s_lms151.u32_timestamp_sec , 0 )
	MACRO_ASSERT_UT( u16_i , s_data.s_lms151.u32_timestamp_ns , 0 )
	MACRO_ASSERT_UT( u16_i , s_data.s_ust1020lx.u32_timestamp_sec , 0 )
	MACRO_ASSERT_UT( u16_i , s_data.s_ust1020lx.u32_timestamp_ns , 0 )

	t_main_Status = SrvLidar_Get_State( &e_stateMachine ) ;
	MACRO_ASSERT_UT( u16_i , t_main_Status , E_OK )
	MACRO_ASSERT_UT( u16_i , e_stateMachine , E_STATE_SRVLIDAR_READY )

	/***************************************************************************************/
	/* CASE 5 : Test normal case : new datas */
	u16_i++ ;
	MACRO_START_UT( u16_i , "Test normal case : new datas" )

	gs_LibLidarS300_data.b_is_new_data = K_TRUE ;
	gs_LibLidarS300_data.u16_scan_data_size = 541 ;
	for( u16_j = 0 ; u16_j < gs_LibLidarS300_data.u16_scan_data_size ; u16_j++ )
	{
		gs_LibLidarS300_data.au16_scan_data[u16_j] = 200 ;
	}
	gs_LibLidarUst1020lx_data.b_is_new_data = K_TRUE ;
	gs_LibLidarUst1020lx_data.u16_scan_data_size = 1081 ;
	for( u16_j = 0 ; u16_j < gs_LibLidarUst1020lx_data.u16_scan_data_size ; u16_j++ )
	{
		gs_LibLidarUst1020lx_data.au32_scan_data[u16_j] = 300 ;
	}
	gs_LibLidarLms151_scan_data.b_is_new_data = K_TRUE ;
	gs_LibLidarLms151_scan_data.u16_dist_len1 = 1081 ;
	for( u16_j = 0 ; u16_j < gs_LibLidarLms151_scan_data.u16_dist_len1 ; u16_j++ )
	{
		gs_LibLidarLms151_scan_data.u16_dist1[u16_j] = 400 ;
	}
	t_main_Status = SrvLidar_Manage() ;
	MACRO_ASSERT_UT( u16_i , t_main_Status , E_OK )

	t_main_Status = SrvLidar_Get_Data( &s_data ) ;
	MACRO_ASSERT_UT( u16_i , t_main_Status , E_OK )
	MACRO_ASSERT_UT( u16_i , (s_data.s_S300.u32_timestamp_sec > 0) , K_TRUE )
	MACRO_ASSERT_UT( u16_i , (s_data.s_S300.u32_timestamp_ns > 0) , K_TRUE )
	MACRO_ASSERT_UT( u16_i , (s_data.s_S300.s16_angle_offset) , -270 )
	MACRO_ASSERT_UT( u16_i , (s_data.s_S300.u16_angular_tick_per_revolution) , 720 )
	MACRO_ASSERT_UT( u16_i , (s_data.s_S300.u16_impact_count) , 541 )
	MACRO_ASSERT_UT( u16_i , ((1.99<s_data.s_S300.f32_distance_list[0])&&(2.01>s_data.s_S300.f32_distance_list[0])) , K_TRUE )
	MACRO_ASSERT_UT( u16_i , (s_data.s_lms151.u32_timestamp_sec > 0) , K_TRUE )
	MACRO_ASSERT_UT( u16_i , (s_data.s_lms151.u32_timestamp_ns > 0) , K_TRUE )
	MACRO_ASSERT_UT( u16_i , (s_data.s_lms151.s16_angle_offset) , -540 )
	MACRO_ASSERT_UT( u16_i , (s_data.s_lms151.u16_angular_tick_per_revolution) , 1440 )
	MACRO_ASSERT_UT( u16_i , (s_data.s_lms151.u16_impact_count) , 1081 )
	MACRO_ASSERT_UT( u16_i , ((0.399<s_data.s_lms151.f32_distance_list[0])&&(0.401>s_data.s_lms151.f32_distance_list[0])) , K_TRUE )
	MACRO_ASSERT_UT( u16_i , (s_data.s_ust1020lx.u32_timestamp_sec > 0) , K_TRUE )
	MACRO_ASSERT_UT( u16_i , (s_data.s_ust1020lx.u32_timestamp_ns > 0) , K_TRUE )
	MACRO_ASSERT_UT( u16_i , (s_data.s_ust1020lx.s16_angle_offset) , -540 )
	MACRO_ASSERT_UT( u16_i , (s_data.s_ust1020lx.u16_angular_tick_per_revolution) , 1440 )
	MACRO_ASSERT_UT( u16_i , (s_data.s_ust1020lx.u16_impact_count) , 1081 )
	MACRO_ASSERT_UT( u16_i , ((0.299<s_data.s_ust1020lx.f32_distance_list[0])&&(0.301>s_data.s_ust1020lx.f32_distance_list[0])) , K_TRUE )

	t_main_Status = SrvLidar_Get_State( &e_stateMachine ) ;
	MACRO_ASSERT_UT( u16_i , t_main_Status , E_OK )
	MACRO_ASSERT_UT( u16_i , e_stateMachine , E_STATE_SRVLIDAR_READY )

	/***************************************************************************************/
	/* CASE 5 : Test manage failed */
	u16_i++ ;
	MACRO_START_UT( u16_i , "Test manage failed" )

	t_main_Status = SrvLidar_Get_State( &e_stateMachine ) ;
	MACRO_ASSERT_UT( u16_i , t_main_Status , E_OK )
	MACRO_ASSERT_UT( u16_i , e_stateMachine , E_STATE_SRVLIDAR_READY )

	gs_LibLidarUst1020lx_status = E_NOT_OK ;
	ge_LibLidarUst1020lx_error = E_LIBLIDARUST1020LX_ERR_COM_LOST ;
	ge_LibLidarS300_error = E_LIBLIDARS300_COM_LOST_ERROR ;
	ge_LibLidarLms151_error = E_LIBLMS151_ERR_SCAN_CONTINUOUS ;

	gs_LibLidarS300_data.b_is_new_data = K_FALSE ;
	gs_LibLidarUst1020lx_data.b_is_new_data = K_FALSE ;
	gs_LibLidarLms151_scan_data.b_is_new_data = K_FALSE ;
	t_main_Status = SrvLidar_Manage() ;
	MACRO_ASSERT_UT( u16_i , t_main_Status , E_NOT_OK )
	t_main_Status = SrvLidar_Get_State( &e_stateMachine ) ;
	MACRO_ASSERT_UT( u16_i , t_main_Status , E_OK )
	MACRO_ASSERT_UT( u16_i , e_stateMachine , E_STATE_SRVLIDAR_UNDEFINED )

	gs_LibLidarUst1020lx_status = E_OK ;


	/***************************************************************************************/
	/* CASE 5 : Test re-init */
	u16_i++ ;
	MACRO_START_UT( u16_i , "Test re-init" )
	t_main_Status = SrvLidar_Close() ;
	MACRO_ASSERT_UT( u16_i , t_main_Status , E_OK )
	t_main_Status = SrvLidar_Initialize() ;
	MACRO_ASSERT_UT( u16_i , t_main_Status , E_OK )
	t_main_Status = SrvLidar_Configure( &s_configuration ) ;
	MACRO_ASSERT_UT( u16_i , t_main_Status , E_OK )

	gs_LibLidarS300_data.b_is_new_data = K_FALSE ;
	gs_LibLidarUst1020lx_data.b_is_new_data = K_FALSE ;
	gs_LibLidarLms151_scan_data.b_is_new_data = K_FALSE ;
	t_main_Status = SrvLidar_Manage() ;
	MACRO_ASSERT_UT( u16_i , t_main_Status , E_OK )



    /*! return code */ 
    return(t_main_Status);
} 
