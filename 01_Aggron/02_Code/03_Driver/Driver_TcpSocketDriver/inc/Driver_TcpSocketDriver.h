/*============================================================================*/
/*                              ROBOSOFT                                      */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*/
/*! 
 * $Source: Driver_TcpSocketDriver.h 
 * $Author: MDega 
 * $Date: 2015/12/30
 */
/*============================================================================*/
/* DESCRIPTION :                                                              */
/** \file  Driver_TcpSocketDriver.h 
  * \brief Fichier Header du Service TcpSocketDriver
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
/* Maximum length of kernel server address */
#define DRVTCPSOCKET_SERVER_ADDRESS_MAX_LEN		32

typedef enum
{
	E_DRVTCPSOCKET_ERROR_NOERROR 									= 0 ,
	E_DRVTCPSOCKET_ERROR_CONNECT_INVALID_SOCKET_ERROR 				= 1 ,
	E_DRVTCPSOCKET_ERROR_CONNECT_INVALID_SERVER_ADDRESS_ERROR 		= 2 ,
	E_DRVTCPSOCKET_ERROR_CONNECT_FUNCTION_CONNECT_ERROR 			= 3 ,
	E_DRVTCPSOCKET_ERROR_DISCONNECT_SOCKET_NOT_OOPEN_ERROR 			= 4,
	E_DRVTCPSOCKET_ERROR_DISCONNECT_FUNCTION_CLOSE_ERROR 			= 5,
	E_DRVTCPSOCKET_ERROR_WRITE_FUNCTION_SEND_ERROR 					= 6,
	E_DRVTCPSOCKET_ERROR_WRITE_SOCKET_NOT_OPEN_ERROR 				= 7,
	E_DRVTCPSOCKET_ERROR_READ_SOCKET_NOT_OPEN_ERROR 				= 8,
	E_DRVTCPSOCKET_ERROR_READ_FUNCTION_RECV_ERROR 					= 9,
	E_DRVTCPSOCKET_ERROR_READ_FUNCTION_RECV_NO_DATA_BEFORE_TIMEOUT 	= 0,
	E_DRVTCPSOCKET_ERROR_CONNECT_CONF_TIMEOUT_ERROR 				= 11,
	E_DRVTCPSOCKET_ERROR_CONNECT_FUNCTION_FCNTL_ERROR				= 12
} E_DRVTCPSOCKET_ERROR ;


typedef enum {
	E_DRVTCPSOCKET_DESCRIPTOR_CLOSED = -1
} E_DRVTCPSOCKET_DESCRIPTOR ;

/* Configuration structure of a tcp link */
typedef struct {
	/* Server address */
	uint8 au8_serverAddress[DRVTCPSOCKET_SERVER_ADDRESS_MAX_LEN];
	/* Server port */
	uint32 u32_server_port ;
	/* Mode blocking or no blocking */
	bool b_blocking_mode ;
	/* Timeout duration (it's used only if blocking mode*/
	uint32 u32_timeout_sec ;
} DrvTcpsocket_cfg_type ;

typedef sint32 DrvTcpsocket_descriptor_type ;

/* Exported Variables 							      */
/*============================================================================*/

/* Exported functions prototypes 					      */
/*============================================================================*/    
 
Std_ReturnType DrvTcpSocket_Initialize(void);
Std_ReturnType DrvTcpSocket_Open( const DrvTcpsocket_cfg_type * const pt_configuration , DrvTcpsocket_descriptor_type * const pt_tcpsocket_desc , E_DRVTCPSOCKET_ERROR * const pt_error ) ;
Std_ReturnType DrvTcpSocket_Close( DrvTcpsocket_descriptor_type * const pt_tcpsocket_desc , E_DRVTCPSOCKET_ERROR * const pt_error ) ;
Std_ReturnType DrvTcpSocket_Send( const DrvTcpsocket_descriptor_type t_tcpsocket_desc , const uint8 * const au8_buffer,	const uint32 u32_buffer_size , sint32 * const ps32_written );
Std_ReturnType DrvTcpSocket_Receive( const DrvTcpsocket_descriptor_type t_tcpsocket_desc , const DrvTcpsocket_cfg_type * const pt_configuration ,
						uint8 * const au8_buffer, const uint32 u32_buffer_size , sint32 * const ps32_read, E_DRVTCPSOCKET_ERROR * const pt_error ) ;

Std_ReturnType DrvTcpSocket_Get_NumberOfBytes( const DrvTcpsocket_descriptor_type t_tcpsocket_desc , uint32 * const pu32_byteSize ) ;
