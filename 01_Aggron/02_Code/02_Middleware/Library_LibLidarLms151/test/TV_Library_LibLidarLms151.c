/*============================================================================*/
/*                        	ROBOSOFT                                      */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*/
/*! 
 * $Source: TI_Library_LibLidarLms151.c 
 * $Author: MDega 
 * $Date: 2015/12/08  
 */
/*============================================================================*/
/* DESCRIPTION :                                                              */
/** \file  TI_Library_LibLidarLms151.c 
  * \brief Fichier Source file of Integration Test LibLidarLms151
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* Constants and types  						      */
/*============================================================================*/  
#define K_1_MS K_100_MS //1000
#define K_10_MS 10000
#define K_100_MS 100000
#define K_1_S 1000000
#define K_10_S 10000000

#define K_LMS151_SERVER_ADDRESS 					(uint8 *)"192.168.2.2"
#define K_LMS151_PORT_SERVER 						2111
#define K_LMS151_BLOCKING_MODE_ACTIVATED 			K_FALSE
   
/* Variables 								      */
/*============================================================================*/ 
static bool gb_FirstCall_ModeConfigureScanDataContent;
static bool gb_FirstCall_ModeConfigureScanDataOutput;
static bool gb_FirstCall_ModeLogin;
static bool gb_FirstCall_ModeLogout;
static bool gb_FirstCall_ModeScanContinuous;
static bool gb_FirstCall_ModeGetData;
static bool gb_FirstCall_ModeAskStatus;
static bool gb_FirstCall_ModeSetFrequencyAndResolution;
static bool gb_FirstCall_ModeSetTimestamp;
static bool gb_FirstCall_ModeStoreParameters;
static bool gb_FirstCall_ModeUndefined;
static uint32 gau32_Timestamp[201800];

static const sint32 gs32_angle_min=-420000;
static const sint32 gs32_angle_max=2100000;
static const uint32 gu32_angular_resolution_0_25_deg=2500;
static const uint32 gu32_angular_resolution_0_5_deg=5000;
static const uint32 gu32_frequency_25_Hz=2500;
static const uint32 gu32_frequency_50_Hz=5000;

#define K_FILE "/home/agv/AGV/trace.txt"

/* Private functions prototypes 					      */
/*============================================================================*/ 

/* Inline functions 							      */
/*============================================================================*/

/* Private functions 							      */
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
    /* Declaration */     
    Std_ReturnType t_main_Status; 
    uint16 u16_i;
    uint32 u32_Timestamp;
    ts_LibLidarLms151_scan_data s_LibLidarLms151_scan_data;
    te_mode e_Mode;
    ts_LibLidarLms151_Configuration s_Configuration;

	uint32 u32_idx;
	uint16 u16_scan_data_size;
	float32 f32_angle;
	float32 f32_angle_step;
	FILE * fd ;
	u32_idx = 0 ;

    /* Initialization */ 
    t_main_Status = E_OK;
	gb_FirstCall_ModeConfigureScanDataContent = K_FALSE;
	gb_FirstCall_ModeConfigureScanDataOutput = K_FALSE;
	gb_FirstCall_ModeLogin= K_FALSE;
	gb_FirstCall_ModeLogout= K_FALSE;
	gb_FirstCall_ModeScanContinuous= K_FALSE;
	gb_FirstCall_ModeGetData= K_FALSE;
	gb_FirstCall_ModeAskStatus= K_FALSE;
	gb_FirstCall_ModeSetFrequencyAndResolution= K_FALSE;
	gb_FirstCall_ModeSetTimestamp= K_FALSE;
	gb_FirstCall_ModeStoreParameters= K_FALSE;
	gb_FirstCall_ModeUndefined= K_FALSE;

   system ("rm " K_FILE);

	//system ("rm ~/AGV/trace.txt");


#define K_LMS151_ALL_SCANS "+1"

	s_Configuration.s_LibLidarLms151_dataconf.b_comment=K_FALSE;
	s_Configuration.s_LibLidarLms151_dataconf.b_device_name=K_TRUE;
	s_Configuration.s_LibLidarLms151_dataconf.b_position=K_TRUE;
	s_Configuration.s_LibLidarLms151_dataconf.b_remission=K_TRUE;
	s_Configuration.s_LibLidarLms151_dataconf.b_time=K_TRUE;
	s_Configuration.s_LibLidarLms151_dataconf.e_resolution=E_LMS151_16_BIT;
	strcpy((char *)s_Configuration.s_LibLidarLms151_dataconf.au8_output_rate,(const char *)K_LMS151_ALL_SCANS);
//	s_Configuration.s_LibLidarLms151_dataconf.u16_output_rate="+1"; // +1 all Scans
	s_Configuration.s_LibLidarLms151_dataconf.u8_data_channel_0=3; // Output channel 1+2: 03 00
	s_Configuration.s_LibLidarLms151_dataconf.u8_data_channel_1=0;
	s_Configuration.s_LibLidarLms151_dataconf.u8_encoder_0=0;
	s_Configuration.s_LibLidarLms151_dataconf.u8_encoder_1=0;
	s_Configuration.s_LibLidarLms151_dataconf.u8_unit=0;
	s_Configuration.s_LibLidarLms151_laserconf.s32_start_angle=gs32_angle_min;
	s_Configuration.s_LibLidarLms151_laserconf.s32_stop_angle=gs32_angle_max;
	s_Configuration.s_LibLidarLms151_laserconf.u32_angle_resolution=gu32_angular_resolution_0_25_deg;
	s_Configuration.s_LibLidarLms151_laserconf.u32_scaning_frequency=gu32_frequency_25_Hz;
	s_Configuration.s_LibLidarLms151_socket.b_blocking_mode=K_LMS151_BLOCKING_MODE_ACTIVATED;
	s_Configuration.s_LibLidarLms151_socket.u32_server_port=(uint32)K_LMS151_PORT_SERVER;
	s_Configuration.s_LibLidarLms151_socket.u32_timeout_sec=100;
	strcpy((char *)s_Configuration.s_LibLidarLms151_socket.au8_serverAddress,(const char *)K_LMS151_SERVER_ADDRESS);

	/*! Configure Module */
	t_main_Status=DrvTcpSocket_Initialize() ;
	t_main_Status|=LibLidarLms151_Initialize();
	t_main_Status|=LibLidarLms151_Configure(&s_Configuration);
	t_main_Status|=LibLidarLms151_Open();

    /* Function Body */

    while ((u32_idx<10)&&
    		(E_NOT_OK!=t_main_Status))
    {
    	t_main_Status|=LibLidarLms151_Manage();
    	usleep(K_1_MS);

    	t_main_Status|=LibLidarLms151_Get_Mode(&e_Mode);

    	switch(e_Mode)
    	{
		case E_MODE_CONFIGURE_SCAN_DATA_CONTENT:
				if (K_FALSE == gb_FirstCall_ModeConfigureScanDataContent)
				{
					gb_FirstCall_ModeConfigureScanDataContent=K_TRUE;
					printf("E_MODE_CONFIGURE_SCAN_DATA_CONTENT \n");
				}
				else
				{
					;
				}
				break;
		case E_MODE_CONFIGURE_SCAN_DATA_OUTPUT :
				if (K_FALSE == gb_FirstCall_ModeConfigureScanDataOutput)
				{
					gb_FirstCall_ModeConfigureScanDataOutput=K_TRUE;
					printf("E_MODE_CONFIGURE_SCAN_DATA_OUTPUT \n");
				}
				else
				{
					;
				}
				break;
		case E_MODE_LOGIN :
				if (K_FALSE == gb_FirstCall_ModeLogin)
				{
					gb_FirstCall_ModeLogin=K_TRUE;
					printf("E_MODE_LOGIN \n");
				}
				else
				{
					;
				}
				break;
		case E_MODE_LOGOUT :
				if (K_FALSE == gb_FirstCall_ModeLogout)
				{
					gb_FirstCall_ModeLogout=K_TRUE;
					printf("E_MODE_LOGOUT \n");
				}
				else
				{
					;
				}
				break;
		case E_MODE_SCAN_CONTINUOUS :
				if (K_FALSE == gb_FirstCall_ModeScanContinuous)
				{
					gb_FirstCall_ModeScanContinuous=K_TRUE;
					printf("E_MODE_SCAN_CONTINUOUS \n");
				}
				else
				{
					;
				}
				break;
		case E_MODE_GET_DATA :
				if (K_FALSE == gb_FirstCall_ModeGetData)
				{
					gb_FirstCall_ModeGetData=K_TRUE;
					printf("E_MODE_GET_DATA \n");
				}
				else
				{
					;
				}
				LibLidarLms151_Get_Scan(&s_LibLidarLms151_scan_data);

				u16_scan_data_size = s_LibLidarLms151_scan_data.u16_dist_len1;
				f32_angle = 270-45 ;
				f32_angle_step = 270.0 / u16_scan_data_size ;

				if( 0 < u16_scan_data_size )
				{
					fd = fopen( K_FILE , "wb") ;

					if( fd  != NULL_PTR )
					{
						u32_idx++ ;
						for( u16_i = 0 ; u16_i < u16_scan_data_size ; u16_i++ )
						{
							fprintf( fd , "%d %.05f %d\n" , (int)u32_idx , f32_angle , (int) s_LibLidarLms151_scan_data.u16_dist1[u16_i] ) ;
							f32_angle = f32_angle - f32_angle_step ;
						}
						fprintf( fd , "\n" ) ;
						fclose( fd ) ;
					}
					printf("%i Trace added in trace.txt\n",u32_idx);
					LibLidarLms151_Get_Timestamp(&u32_Timestamp);
					gau32_Timestamp[u32_idx]=u32_Timestamp;
					printf("Timestamp %d \n",u32_Timestamp);
				}
				break;
		case E_MODE_ASK_STATUS :
				if (K_FALSE == gb_FirstCall_ModeAskStatus)
				{
					gb_FirstCall_ModeAskStatus=K_TRUE;
					printf("E_MODE_ASK_STATUS \n");
				}
				else
				{
					;
				}
				break;
		case E_MODE_SET_FREQUENCY_AND_RESOLUTION :
				if (K_FALSE == gb_FirstCall_ModeSetFrequencyAndResolution)
				{
					gb_FirstCall_ModeSetFrequencyAndResolution=K_TRUE;
					printf("E_MODE_SET_FREQUENCY_AND_RESOLUTION \n");
				}
				else
				{
					;
				}
				break;
		case E_MODE_SET_TIMESTAMP :
				if (K_FALSE == gb_FirstCall_ModeSetTimestamp)
				{
					gb_FirstCall_ModeSetTimestamp=K_TRUE;
					printf("E_MODE_SET_FREQUENCY_AND_RESOLUTION \n");
				}
				else
				{
					;
				}
				break;
		case E_MODE_STORE_PARAMETERS :
				if (K_FALSE == gb_FirstCall_ModeStoreParameters)
				{
					gb_FirstCall_ModeStoreParameters=K_TRUE;
					printf("E_MODE_STORE_PARAMETERS \n");
				}
				else
				{
					;
				}
				break;
		case E_MODE_UNDEFINED :
				if (K_FALSE == gb_FirstCall_ModeUndefined)
				{
					gb_FirstCall_ModeUndefined=K_TRUE;
					printf("E_MODE_UNDEFINED \n");
				}
				else
				{
					;
				}
				break;
		default :
				break;
    	}
    }

    /* return code */ 
    return(t_main_Status);
} 
