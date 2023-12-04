/*============================================================================*/
/*                        	ROBOSOFT                                      */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*/
/*! 
 * $Source: TI_Driver_SerialDriver.c 
 * $Author: MDEGA
 * $Date: 2015/12/28
 */
/*============================================================================*/
/* DESCRIPTION :                                                              */
/** \file  TI_Driver_SerialDriver.c 
  * \brief Fichier Source file of Integration Test SerialDriver
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
#include "Driver_SerialDriver.h"

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
#define K_TI_SERIAL_SIZE_ARRAY 128

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
    serialdrv_cfg_type t_configuration ;
    uint16 u16_error_count ;
    uint8 au8_buffer[K_TI_SERIAL_SIZE_ARRAY] ;
    uint8 au8_buffer_read[K_TI_SERIAL_SIZE_ARRAY] ;
    uint8 au8_in_buffer[K_TI_SERIAL_SIZE_ARRAY] ;
    uint32 u32_buffer_size ;
    sint32 s32_read ;
    sint32 s32_written ;
    uint16 u16_i ;
    sint32 s32_port_desc ;
    sint32 s32_port_desc_another_process ;
    uint32 au32_baudrate[] = {1, 300, 1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200, 230400, 460800, 921600} ;
    uint8  au8_bitsize[] = {1, 7, 8} ;
    parity_mode_type at_parity_mode[] = { (parity_mode_type)5 , E_DRVSERIAL_NO_PARITY , E_DRVSERIAL_ODD_PARITY , E_DRVSERIAL_EVEN_PARITY } ;
    bool ab_stopbitena[] = { (bool)2 , K_TRUE , K_FALSE } ;

    /*! Initialization */ 
    u32_buffer_size = K_TI_SERIAL_SIZE_ARRAY ;
    s32_port_desc = K_DRVSERIAL_CLOSED ;
    u16_error_count = 0 ;

    /*! Function Body */
    printf( "/***************** TI_Driver_SerialDriver ******************** \n" ) ;
    printf( " * Hardware request :                                        * \n" ) ;
    printf( " *  - USB/Serial cable                                       * \n" ) ;
    printf( " *  - Strap between pinout 2 to 3                            * \n" ) ;
    printf( " * Software request :                                        * \n" ) ;
    printf( " *   - Serial device name : /dev/ttyUSB0                     * \n" ) ;
    printf( " *************************************************************/ \n\n" ) ;

    /***************************************************************************************/
    /* CASE 1 : Test call all functions if driver not initialized */
    u16_error_count++ ;
    MACRO_START_UT( u16_error_count , "Test call all functions if driver not initialized" )

    t_main_Status = DrvSerial_Open( &t_configuration , &s32_port_desc ) ;
    MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_NOT_OK )

	t_main_Status = DrvSerial_Close( &s32_port_desc ) ;
    MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_NOT_OK )

	t_main_Status = DrvSerial_Receive( s32_port_desc , au8_buffer, u32_buffer_size , &s32_read ) ;
    MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_NOT_OK )

	t_main_Status = DrvSerial_Send( s32_port_desc , au8_buffer, u32_buffer_size , &s32_written ) ;
    MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_NOT_OK )

	t_main_Status = DrvSerial_Flush( s32_port_desc ) ;
    MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_NOT_OK )


	/***************************************************************************************/
	/* CASE 2 : Test call all function if driver is initialized but not opened */
    u16_error_count++ ;
	MACRO_START_UT( u16_error_count , "Test call all function if driver is initialized but not opened" )

	t_main_Status = DrvSerial_Initialize() ;
    MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_OK )

	t_main_Status = DrvSerial_Close( &s32_port_desc ) ;
    MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_OK )

	t_main_Status = DrvSerial_Receive( s32_port_desc , au8_buffer, u32_buffer_size , &s32_read ) ;
    MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_NOT_OK )

	t_main_Status = DrvSerial_Send( s32_port_desc , au8_buffer, u32_buffer_size , &s32_written ) ;
    MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_NOT_OK )

	t_main_Status = DrvSerial_Flush( s32_port_desc ) ;
    MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_NOT_OK )


	/***************************************************************************************/
	/* CASE 3 : Test Open multiple call */
    u16_error_count++ ;
	MACRO_START_UT( u16_error_count , "Test Open multiple call" )

	strcpy( t_configuration.sz_serialport_name , "/dev/ttyUSB0" ) ;
	t_configuration.u32_baudrate = 9600 ;
	t_configuration.u8_bitsize = 8 ;
	t_configuration.t_parity_mode = E_DRVSERIAL_NO_PARITY ;
	t_configuration.b_stopbitena = K_TRUE ;

	t_main_Status = DrvSerial_Open( &t_configuration , &s32_port_desc ) ;
    MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_OK )

	t_main_Status = DrvSerial_Open( &t_configuration , &s32_port_desc ) ;
    MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_OK )

	t_main_Status = DrvSerial_Close(&s32_port_desc) ;
    MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_OK )



	/***************************************************************************************/
	/* CASE 4 : Test normal case */
    u16_error_count++ ;
	MACRO_START_UT( u16_error_count , "Test normal case" )

	t_main_Status = DrvSerial_Open( &t_configuration , &s32_port_desc ) ;
    MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_OK )

	t_main_Status = DrvSerial_Send( s32_port_desc , au8_buffer, 1 , &s32_written ) ;
    MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_OK )
	usleep(20*1000) ;

	t_main_Status = DrvSerial_Receive( s32_port_desc , au8_buffer, u32_buffer_size , &s32_read ) ;
    MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_OK )

	t_main_Status = DrvSerial_Flush( s32_port_desc ) ;
    MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_OK )

	t_main_Status = DrvSerial_Close( &s32_port_desc ) ;
    MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_OK )



	/***************************************************************************************/
	/* CASE 5 : Test normal case and Read/write datas */
    u16_error_count++ ;
	MACRO_START_UT( u16_error_count , "Test normal case and Read/write datas" )

	t_main_Status = DrvSerial_Open( &t_configuration , &s32_port_desc ) ;
    MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_OK )

	for( u16_i = 0 ; u16_i < 10 ; u16_i++ )
	{
		au8_buffer[u16_i] = u16_i & 0xFF ;
	}
	t_main_Status = DrvSerial_Send( s32_port_desc , au8_buffer, 10 , &s32_written ) ;
    MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_OK )
	MACRO_ASSERT_UT( u16_error_count , s32_written , 10 )

	/* Waiting transfer datas */
	usleep(20*1000) ;

	t_main_Status = DrvSerial_Receive( s32_port_desc , au8_buffer_read, u32_buffer_size , &s32_read ) ;
    MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_OK )
	MACRO_ASSERT_UT( u16_error_count , s32_read , s32_written )
	for( u16_i = 0 ; u16_i < s32_read ; u16_i++ )
	{
		MACRO_ASSERT_UT( u16_error_count , (au8_buffer[u16_i]&0xFF) , (au8_buffer_read[u16_i]&0xFF) )
	}

	t_main_Status = DrvSerial_Flush( s32_port_desc ) ;
    MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_OK )

	t_main_Status = DrvSerial_Close( &s32_port_desc ) ;
    MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_OK )



	/***************************************************************************************/
	/* CASE 6 : Test parameters limit */
    u16_error_count++ ;
	MACRO_START_UT( u16_error_count , "Test parameters limit" )

	t_main_Status = DrvSerial_Open( &t_configuration , NULL_PTR ) ;
    MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_NOT_OK )

	t_main_Status = DrvSerial_Close( NULL_PTR ) ;
    MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_NOT_OK )

	t_main_Status = DrvSerial_Open( &t_configuration , &s32_port_desc ) ;
    MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_OK )

	t_main_Status = DrvSerial_Send( s32_port_desc , NULL_PTR, 10 , &s32_written ) ;
    MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_NOT_OK )

	t_main_Status = DrvSerial_Send( s32_port_desc , au8_buffer, 10 , NULL_PTR ) ;
    MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_NOT_OK )

	t_main_Status = DrvSerial_Send( s32_port_desc , NULL_PTR, 10 , NULL_PTR ) ;
    MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_NOT_OK )

	t_main_Status = DrvSerial_Receive( s32_port_desc , NULL_PTR, u32_buffer_size , &s32_read ) ;
    MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_NOT_OK )

	t_main_Status = DrvSerial_Receive( s32_port_desc , au8_buffer_read, u32_buffer_size , NULL_PTR ) ;
    MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_NOT_OK )

	t_main_Status = DrvSerial_Receive( s32_port_desc , NULL_PTR, u32_buffer_size , NULL_PTR ) ;
    MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_NOT_OK )

	t_main_Status = DrvSerial_Close( &s32_port_desc ) ;
    MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_OK )


	/***************************************************************************************/
	/* CASE 7 : Lost communication */
    u16_error_count++ ;
	MACRO_START_UT( u16_error_count , "Test Lost communication" )


	t_main_Status = DrvSerial_Open( &t_configuration , &s32_port_desc ) ;
    MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_OK )

	printf( "Disconnect USB/serial cable and then press \"X+Enter\" \n" ) ;
    fflush( stdout ) ;
	fgets( (char*)au8_in_buffer , K_TI_SERIAL_SIZE_ARRAY , stdin ) ;

	t_main_Status = DrvSerial_Receive( s32_port_desc , au8_buffer_read, u32_buffer_size , &s32_read ) ;
    MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_NOT_OK )

	t_main_Status = DrvSerial_Send( s32_port_desc , au8_buffer, 10 , &s32_written ) ;
    MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_NOT_OK )

	t_main_Status = DrvSerial_Close( &s32_port_desc ) ;
    MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_OK )


    /***************************************************************************************/
	/* CASE 8 : non-existent serial port */
	u16_error_count++ ;
	MACRO_START_UT( u16_error_count , "Test non-existent serial port" )

	t_main_Status = DrvSerial_Open( &t_configuration , &s32_port_desc ) ;
    MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_NOT_OK )

    /***************************************************************************************/
	/* CASE 9 : Test all configurations */
	u16_error_count++ ;
	MACRO_START_UT( u16_error_count , "Test all configurations" )


	printf( "Connect USB/serial cable and then press \"X+Enter\" \n" ) ;
    fflush( stdout ) ;
	fgets( (char*)au8_in_buffer , K_TI_SERIAL_SIZE_ARRAY , stdin ) ;

    strcpy( t_configuration.sz_serialport_name , "/dev/ttyUSB0" ) ;
	t_configuration.u32_baudrate = 9600 ;
	t_configuration.u8_bitsize = 8 ;
	t_configuration.t_parity_mode = E_DRVSERIAL_NO_PARITY ;
	t_configuration.b_stopbitena = K_TRUE ;

	/* Test all baudrate */
	for( u16_i = 0 ; u16_i < (sizeof(au32_baudrate)/sizeof(uint32)) ; u16_i++ )
	{
		t_configuration.u32_baudrate = au32_baudrate[u16_i] ;
		t_main_Status = DrvSerial_Open( &t_configuration , &s32_port_desc ) ;
	    MACRO_ASSERT_UT( u16_error_count , t_main_Status , (0==u16_i)?E_NOT_OK:E_OK )
		t_main_Status = DrvSerial_Close( &s32_port_desc ) ;
	    MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_OK )
	}
	/* Test all byte size */
	for( u16_i = 0 ; u16_i < (sizeof(au8_bitsize)/sizeof(uint8)) ; u16_i++ )
	{
		t_configuration.u8_bitsize = au8_bitsize[u16_i] ;
		t_main_Status = DrvSerial_Open( &t_configuration , &s32_port_desc ) ;
		MACRO_ASSERT_UT( u16_error_count , t_main_Status , (0==u16_i)?E_NOT_OK:E_OK )
		t_main_Status = DrvSerial_Close( &s32_port_desc ) ;
		MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_OK )
	}
	/* Test all parity type */
	for( u16_i = 0 ; u16_i < (sizeof(at_parity_mode)/sizeof(parity_mode_type)) ; u16_i++ )
	{
		t_configuration.t_parity_mode = at_parity_mode[u16_i] ;
		t_main_Status = DrvSerial_Open( &t_configuration , &s32_port_desc ) ;
		MACRO_ASSERT_UT( u16_error_count , t_main_Status , (0==u16_i)?E_NOT_OK:E_OK )
		t_main_Status = DrvSerial_Close( &s32_port_desc ) ;
		MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_OK )
	}
	/* Test all stop bit */
	for( u16_i = 0 ; u16_i < (sizeof(ab_stopbitena)/sizeof(bool)) ; u16_i++ )
	{
		t_configuration.b_stopbitena = ab_stopbitena[u16_i] ;
		t_main_Status = DrvSerial_Open( &t_configuration , &s32_port_desc ) ;
		MACRO_ASSERT_UT( u16_error_count , t_main_Status , (0==u16_i)?E_NOT_OK:E_OK )
		t_main_Status = DrvSerial_Close( &s32_port_desc ) ;
		MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_OK )
	}


	/***************************************************************************************/
	/* CASE 10 : Test Open serial port already locked by another process */
	u16_error_count++ ;
	MACRO_START_UT( u16_error_count , "Test Open multiple call" )

	strcpy( t_configuration.sz_serialport_name , "/dev/ttyUSB0" ) ;
	t_configuration.u32_baudrate = 9600 ;
	t_configuration.u8_bitsize = 8 ;
	t_configuration.t_parity_mode = E_DRVSERIAL_NO_PARITY ;
	t_configuration.b_stopbitena = K_TRUE ;

	s32_port_desc_another_process = K_DRVSERIAL_CLOSED ;

	t_main_Status = DrvSerial_Open( &t_configuration , &s32_port_desc_another_process ) ;
	MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_OK )

	t_main_Status = DrvSerial_Open( &t_configuration , &s32_port_desc ) ;
	MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_NOT_OK )

	t_main_Status = DrvSerial_Close(&s32_port_desc) ;
	MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_OK )

	t_main_Status = DrvSerial_Close(&s32_port_desc_another_process) ;
	MACRO_ASSERT_UT( u16_error_count , t_main_Status , E_OK )



    /*! return code */ 
    return(t_main_Status);
} 
