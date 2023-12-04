/*============================================================================*/
/*                              ROBOSOFT                                      */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*/
/*! 
 * $Source: Driver_SerialDriver.h 
 * $Author: MDEGA
 * $Date: 2015/12/28
 */
/*============================================================================*/
/* DESCRIPTION :                                                              */
/** \file  Driver_SerialDriver.h 
  * \brief Fichier Header du Service SerialDriver
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
/* Maximum length of kernel serial port filename */
#define DRVSERIAL_SERIAL_PORT_NAME_MAX_LEN		32

#define K_DRVSERIAL_CLOSED -1

/* Parity mode options */
typedef enum {
	E_DRVSERIAL_NO_PARITY = 0,
	E_DRVSERIAL_ODD_PARITY = 1,
	E_DRVSERIAL_EVEN_PARITY = 2
} parity_mode_type ;


/* Configuration structure of a serial port */
typedef struct {
	/* Serial port filename */
	char	sz_serialport_name[DRVSERIAL_SERIAL_PORT_NAME_MAX_LEN];
	/* Serial port baudrate: 300, 1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200, 230400, 460800, 921600 */
	uint32	u32_baudrate ;
	/* Size of a byte (7 or 8) */
	uint8   u8_bitsize;
	/* Parity mode: none, odd or even */
	parity_mode_type t_parity_mode ;
	/* Stop bit presence */
	bool b_stopbitena ;
} serialdrv_cfg_type;


/* Exported Variables 							      */
/*============================================================================*/

/* Exported functions prototypes 					      */
/*============================================================================*/    
 
Std_ReturnType DrvSerial_Initialize(void);
Std_ReturnType DrvSerial_Open( const serialdrv_cfg_type * const t_configuration , sint32 * const ps32_port_desc ) ;
Std_ReturnType DrvSerial_Close( sint32 * const ps32_port_desc ) ;
Std_ReturnType DrvSerial_Receive( const sint32 s32_port_desc , uint8 * const au8_buffer, const uint32 u32_buffer_size , sint32 * const ps32_read ) ;
Std_ReturnType DrvSerial_Send( const sint32 s32_port_desc , const uint8 * const au8_buffer, const uint32 u32_buffer_size , sint32 * const ps32_written ) ;
Std_ReturnType DrvSerial_Flush( const sint32 s32_port_desc ) ;
