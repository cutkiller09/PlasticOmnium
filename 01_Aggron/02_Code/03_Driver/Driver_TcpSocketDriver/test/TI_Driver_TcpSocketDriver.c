/*============================================================================*/
/*                        	ROBOSOFT                                      */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*/
/*! 
 * $Source: TI_Driver_TcpSocketDriver.c 
 * $Author: MDega 
 * $Date: 2015/12/08  
 */
/*============================================================================*/
/* DESCRIPTION :                                                              */
/** \file  TI_Driver_TcpSocketDriver.c 
  * \brief Fichier Source file of Integration Test TcpSocketDriver
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
#include "Driver_TcpSocketDriver.h" 

#include <stdio.h>
#include <string.h>
#include <unistd.h>


#define MACRO_START_UT(u16_num_case, sz_title_case) printf( "\nCase %d : \\************ %s ************\\ \n" , u16_num_case , sz_title_case ) ; fflush( stdout ) ;
#define MACRO_ASSERT_UT(u16_num_case , u16_error_count , u16_num_error_expected ) \
		printf( "Case %d : tested_value = %d (expected : %d) : %s\n" , \
				u16_num_case , \
				u16_error_count , \
				u16_num_error_expected , \
				(u16_num_error_expected==u16_error_count)?"SUCCESS":"FAILED" ) ; fflush( stdout ) ;


/* Constants and types  						      */
/*============================================================================*/  
#define K_TI_TCP_SIZE_ARRAY 128
#define K_TI_IP_ADDRESS "192.168.1.2"
   
/* Variables 								      */
/*============================================================================*/ 

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
/*! Declaration */     
    Std_ReturnType t_main_Status; 
    DrvTcpsocket_cfg_type t_configuration ;
    E_DRVTCPSOCKET_ERROR t_error ;
    uint16 u16_error_count ;
    uint8 au8_buffer[K_TI_TCP_SIZE_ARRAY] ;
    uint32 u32_buffer_size ;
    sint32 s32_read ;
    sint32 s32_written ;
    uint32 u32_byteSize ;
    DrvTcpsocket_descriptor_type t_socket_desc ;

    /*! Initialization */ 
    u16_error_count = 0 ;
    u32_buffer_size = K_TI_TCP_SIZE_ARRAY ;
    t_socket_desc = E_DRVTCPSOCKET_DESCRIPTOR_CLOSED ;

    /*! Function Body */
    printf( "/**************** TI_Driver_TcpSocketDriver ****************** \n" ) ;
    printf( " * Hardware request :                                        * \n" ) ;
    printf( " *  - Server TCP                                             * \n" ) ;
    printf( " * Software request :                                        * \n" ) ;
    printf( " *   - Server address : %s                         * \n" , K_TI_IP_ADDRESS ) ;
    printf( " *   - Server port : 10940                                   * \n" ) ;
    printf( " *************************************************************/ \n\n" ) ;

    /***************************************************************************************/
    /* CASE 1 : Test call all functions if driver not initialized */
    u16_error_count++ ;
    MACRO_START_UT( u16_error_count , "Test call all functions if driver not initialized" )

    t_main_Status = DrvTcpSocket_Open( &t_configuration , &t_socket_desc , &t_error ) ;
	MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_NOT_OK )

	t_main_Status = DrvTcpSocket_Close( &t_socket_desc , &t_error ) ;
	MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_NOT_OK )

	t_main_Status = DrvTcpSocket_Receive( t_socket_desc , &t_configuration , au8_buffer, u32_buffer_size , &s32_read , &t_error ) ;
	MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_NOT_OK )

	t_main_Status = DrvTcpSocket_Send( t_socket_desc , au8_buffer, u32_buffer_size , &s32_written ) ;
	MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_NOT_OK )

	t_main_Status = DrvTcpSocket_Get_NumberOfBytes( t_socket_desc , &u32_byteSize ) ;
	MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_NOT_OK )


	/***************************************************************************************/
	/* CASE 2 : Test call all function if driver is initialized but not opened */
    u16_error_count++ ;
	MACRO_START_UT( u16_error_count , "Test call all function if driver is initialized but not opened" )

	t_main_Status = DrvTcpSocket_Initialize() ;
	MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_OK )

	t_main_Status = DrvTcpSocket_Close( &t_socket_desc , &t_error ) ;
	MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_OK )

	t_main_Status = DrvTcpSocket_Receive( t_socket_desc , &t_configuration , au8_buffer, u32_buffer_size , &s32_read , &t_error ) ;
	MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_NOT_OK )

	t_main_Status = DrvTcpSocket_Send( t_socket_desc , au8_buffer, u32_buffer_size , &s32_written ) ;
	MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_NOT_OK )

	t_main_Status = DrvTcpSocket_Get_NumberOfBytes( t_socket_desc , &u32_byteSize ) ;
	MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_NOT_OK )



	/***************************************************************************************/
	/* CASE 3 : Test Open multiple call : /!\ Need server TCP */
    u16_error_count++ ;
	MACRO_START_UT( u16_error_count , "Test Open multiple call : /!\\ Need server TCP" )

	strcpy( (char*)t_configuration.au8_serverAddress , K_TI_IP_ADDRESS ) ;
	t_configuration.u32_server_port = 10940 ;
	t_configuration.b_blocking_mode = K_FALSE ;
	t_configuration.u32_timeout_sec = 0 ;

	t_main_Status = DrvTcpSocket_Open( &t_configuration , &t_socket_desc , &t_error ) ;
	MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_OK )

	t_main_Status = DrvTcpSocket_Open( &t_configuration , &t_socket_desc , &t_error ) ;
	MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_OK )

	t_main_Status = DrvTcpSocket_Close( &t_socket_desc , &t_error ) ;
	MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_OK )


	printf( "Waiting reset server service \n" ) ;
	fflush( stdout ) ;
	usleep(2000*1000) ;

	/***************************************************************************************/
	/* CASE 4 : Test normal case (non-blocking mode) : /!\ Need server TCP */
    u16_error_count++ ;
	MACRO_START_UT( u16_error_count , "Test normal case (non-blocking mode) : /!\\ Need server TCP" )

	t_main_Status = DrvTcpSocket_Open( &t_configuration , &t_socket_desc , &t_error ) ;
	MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_OK )

	sprintf( (char*)au8_buffer , "VV" ) ;
	t_main_Status = DrvTcpSocket_Send( t_socket_desc , au8_buffer, strlen( (char*)au8_buffer) , &s32_written ) ;
	MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_OK )
	usleep(100*1000) ;

	t_main_Status = DrvTcpSocket_Get_NumberOfBytes( t_socket_desc , &u32_byteSize ) ;
	MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_OK )

	t_main_Status = DrvTcpSocket_Receive( t_socket_desc , &t_configuration , au8_buffer, u32_buffer_size , &s32_read , &t_error ) ;
	MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_OK )

	t_main_Status = DrvTcpSocket_Close( &t_socket_desc , &t_error ) ;
	MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_OK )

	printf( "Waiting reset server service \n" ) ;
	fflush( stdout ) ;
	usleep(2000*1000) ;


	/***************************************************************************************/
	/* CASE 5 : Test normal case (blocking mode) : /!\ Need server TCP */
	u16_error_count++ ;
	MACRO_START_UT( u16_error_count , "Test normal case (blocking mode) : /!\\ Need server TCP" )

	t_configuration.b_blocking_mode = K_TRUE ;
	t_configuration.u32_timeout_sec = 2 ;
	t_main_Status = DrvTcpSocket_Open( &t_configuration , &t_socket_desc , &t_error ) ;
	MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_OK )
	usleep(2000*1000) ;

	sprintf( (char*)au8_buffer , "VV%c%c" , 0x0A , 0x0A ) ;
	t_main_Status = DrvTcpSocket_Send( t_socket_desc , au8_buffer, strlen( (char*)au8_buffer)+2 , &s32_written ) ;
	MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_NOT_OK )
	usleep(1000*1000) ;

	t_main_Status = DrvTcpSocket_Get_NumberOfBytes( t_socket_desc , &u32_byteSize ) ;
	MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_NOT_OK )

	t_main_Status = DrvTcpSocket_Receive( t_socket_desc , &t_configuration , au8_buffer, u32_buffer_size , &s32_read , &t_error ) ;
	MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_OK )

	t_main_Status = DrvTcpSocket_Close( &t_socket_desc , &t_error ) ;
	MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_OK )

	printf( "Waiting reset server service \n" ) ;
	fflush( stdout ) ;
	usleep(2000*1000) ;


	/***************************************************************************************/
	/* CASE 6 : Test parameters limit : /!\ Need server TCP */
	u16_error_count++ ;
	MACRO_START_UT( u16_error_count , "Test parameters limit : /!\\ Need server TCP" )

	t_configuration.b_blocking_mode = K_FALSE ;
	t_configuration.u32_timeout_sec = 0 ;

	t_main_Status = DrvTcpSocket_Open( NULL_PTR , &t_socket_desc , &t_error ) ;
	MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_NOT_OK )

	t_main_Status = DrvTcpSocket_Open( &t_configuration , NULL_PTR , &t_error ) ;
	MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_NOT_OK )

	t_main_Status = DrvTcpSocket_Open( &t_configuration , &t_socket_desc , NULL_PTR ) ;
	MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_NOT_OK )

	t_main_Status = DrvTcpSocket_Open( NULL_PTR , NULL_PTR , NULL_PTR ) ;
	MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_NOT_OK )

	t_main_Status = DrvTcpSocket_Open( &t_configuration , &t_socket_desc , &t_error ) ;
	MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_OK )

	t_main_Status = DrvTcpSocket_Close( NULL_PTR , &t_error ) ;
	MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_NOT_OK )

	t_main_Status = DrvTcpSocket_Close( &t_socket_desc , NULL_PTR ) ;
	MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_NOT_OK )

	t_main_Status = DrvTcpSocket_Close( NULL_PTR , NULL_PTR ) ;
	MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_NOT_OK )

	t_main_Status = DrvTcpSocket_Send( t_socket_desc , NULL_PTR , u32_buffer_size , &s32_written ) ;
	MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_NOT_OK )

	t_main_Status = DrvTcpSocket_Send( t_socket_desc , au8_buffer, u32_buffer_size , NULL_PTR ) ;
	MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_NOT_OK )

	t_main_Status = DrvTcpSocket_Send( t_socket_desc , NULL_PTR, u32_buffer_size , NULL_PTR ) ;
	MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_NOT_OK )

	t_main_Status = DrvTcpSocket_Receive( t_socket_desc , NULL_PTR , au8_buffer, u32_buffer_size , &s32_read , &t_error ) ;
	MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_NOT_OK )

	t_main_Status = DrvTcpSocket_Receive( t_socket_desc , &t_configuration , NULL_PTR, u32_buffer_size , &s32_read , &t_error ) ;
	MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_NOT_OK )

	t_main_Status = DrvTcpSocket_Receive( t_socket_desc , &t_configuration , au8_buffer, u32_buffer_size , NULL_PTR , &t_error ) ;
	MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_NOT_OK )

	t_main_Status = DrvTcpSocket_Receive( t_socket_desc , &t_configuration , au8_buffer, u32_buffer_size , &s32_read , NULL_PTR ) ;
	MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_NOT_OK )

	t_main_Status = DrvTcpSocket_Receive( t_socket_desc , NULL_PTR , NULL_PTR, u32_buffer_size , NULL_PTR , NULL_PTR ) ;
	MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_NOT_OK )

	t_main_Status = DrvTcpSocket_Get_NumberOfBytes( t_socket_desc , NULL_PTR ) ;
	MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_NOT_OK )

	t_main_Status = DrvTcpSocket_Close( &t_socket_desc , &t_error ) ;
	MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_OK )

	t_socket_desc = 3 ;
	t_main_Status = DrvTcpSocket_Send( t_socket_desc , au8_buffer, u32_buffer_size , NULL_PTR ) ;
	MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_NOT_OK )

	t_main_Status = DrvTcpSocket_Close( &t_socket_desc , &t_error ) ;
	MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_NOT_OK )

	t_socket_desc = E_DRVTCPSOCKET_DESCRIPTOR_CLOSED ;

	printf( "Waiting reset server service \n" ) ;
	usleep(2000*1000) ;



	/***************************************************************************************/
	/* CASE 7 : Lost communication : /!\ Need server TCP */
	u16_error_count++ ;
	MACRO_START_UT( u16_error_count , "Test Lost communication : /!\\ Need server TCP " )

	t_main_Status = DrvTcpSocket_Open( &t_configuration , &t_socket_desc , &t_error ) ;
	MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_OK )

	printf( "Disconnect ethernet cable (or wifi connection) and then press \"X+Enter\" \n" ) ;
	fflush( stdout ) ;
	fgets( (char*)au8_buffer , K_TI_TCP_SIZE_ARRAY , stdin ) ;

	sprintf( (char*)au8_buffer , "VV\r" ) ;
	t_main_Status = DrvTcpSocket_Send( t_socket_desc , au8_buffer, strlen((char*)au8_buffer) , &s32_written ) ;
	MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_NOT_OK )

	/* Receive no detect if connection is lost */
	t_main_Status = DrvTcpSocket_Receive( t_socket_desc , &t_configuration , au8_buffer, u32_buffer_size , &s32_read , &t_error ) ;
	MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_OK )

	t_main_Status = DrvTcpSocket_Get_NumberOfBytes( t_socket_desc , &u32_byteSize ) ;
	MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_NOT_OK )

	t_main_Status = DrvTcpSocket_Close( &t_socket_desc , &t_error ) ;
	MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_OK )



	/***************************************************************************************/
	/* CASE 8 : No communication */
	u16_error_count++ ;
	MACRO_START_UT( u16_error_count , "Test No communication " )

	strcpy( (char*)t_configuration.au8_serverAddress , K_TI_IP_ADDRESS ) ;
	t_configuration.u32_server_port = 10940 ;
	t_configuration.b_blocking_mode = K_FALSE ;
	t_configuration.u32_timeout_sec = 0 ;

	t_main_Status = DrvTcpSocket_Open( &t_configuration , &t_socket_desc , &t_error ) ;
	MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_NOT_OK )

	strcpy( (char*)t_configuration.au8_serverAddress , K_TI_IP_ADDRESS ) ;
	t_configuration.u32_server_port = 10941 ;
	t_configuration.b_blocking_mode = K_FALSE ;
	t_configuration.u32_timeout_sec = 0 ;

	t_main_Status = DrvTcpSocket_Open( &t_configuration , &t_socket_desc , &t_error ) ;
	MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_NOT_OK )

	strcpy( (char*)t_configuration.au8_serverAddress , "toto" ) ;
	t_configuration.u32_server_port = 10940 ;
	t_configuration.b_blocking_mode = K_FALSE ;
	t_configuration.u32_timeout_sec = 0 ;

	t_main_Status = DrvTcpSocket_Open( &t_configuration , &t_socket_desc , &t_error ) ;
	MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_NOT_OK )

    /*! return code */ 
    return(t_main_Status);
} 
