/*============================================================================*/
/*                              ROBOSOFT                                      */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*/
/*! 
 * $Source: Driver_SerialDriver.c 
 * $Author: MDEGA
 * $Date: 2015/12/28
 */
/*============================================================================*/
/* DESCRIPTION :                                                              */
/** \file  Driver_SerialDriver.c 
  * \brief Source file of SerialDriver Service
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

#define K_DEBUG 1

/* Includes 								      */
/*============================================================================*/ 
#include "types.h"
#include "Library_StdLib.h"
#include "Driver_SerialDriver.h" 

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>


/* Constants and types  						      */
/*============================================================================*/ 
static Std_ReturnType gt_InitSerialDriver_Status = E_NOT_OK ;
   
/* Variables 								      */
/*============================================================================*/ 


/* Private functions prototypes 					      */
/*============================================================================*/ 

/* Inline functions 							      */
/*============================================================================*/

/* Private functions 							      */
/*============================================================================*/

/**
 * \fn  Std_ReturnType DrvSerial_Initialize(void) 
 * \brief this function initialize internal parametres
 *
 * \param  void
 *
* \return Std_ReturnType :
*         - E_OK if good return function
*         - E_NOT_OK if not
 */
Std_ReturnType DrvSerial_Initialize(void) 
{
    /*! Declaration */     
    Std_ReturnType t_Initialize_Status; 

    /*! Initialization */
    gt_InitSerialDriver_Status = E_OK ;

    /*! Function Body */

    t_Initialize_Status = gt_InitSerialDriver_Status ;

    /*! return code */
    return(t_Initialize_Status);
}

/**
 * \fn  Std_ReturnType DrvSerial_Open( serialdrv_cfg_type t_configuration , sint32 *ps32_port_desc )
 * \brief this function opens serial link
 *
 * \param  t_configuration [in] : Serial configuration (port name, baudrate, size byte, parity, stop bit)
 * \param  ps32_port_desc [out] : Serial port description
 *
* \return Std_ReturnType :
*         - E_OK if good return function
*         - E_NOT_OK if not
 */
Std_ReturnType DrvSerial_Open( const serialdrv_cfg_type * const t_configuration , sint32 *ps32_port_desc )
{
    /*! Declaration */
    Std_ReturnType t_Open_Status;
	/* Baudrate configuration */
	tcflag_t bdrate_t;
	/* Bit size configuration */
	tcflag_t bsize_t;
	/* Parity check and generation configuration */
	tcflag_t parcfg_iflag_t;
	tcflag_t parcfg_cflag_t;
	/* Stop bit configuration */
	tcflag_t stopbit_t;

	/* Structure holding the serial port configuration */
	struct termios serialport_cfg_t;
	/* Serial port status used by ioctl */
	sint32 s32_status;
	/* Error indicator */
	sint32 s32_error;

    /*! Initialization */
	t_Open_Status = E_NOT_OK ;
	parcfg_cflag_t = 0;
	parcfg_iflag_t = IGNPAR;

    /*! Function Body */
    if( (E_NOT_OK != gt_InitSerialDriver_Status) && (NULL_PTR != ps32_port_desc) && (NULL_PTR != t_configuration) )
    {
    	if( (K_DRVSERIAL_CLOSED == *ps32_port_desc) )
    	{
			/* Open the serial port:
			 * O_RDWR: read/write
			 * O_NOCTTY: the tty terminal device should not become the process' controlling terminal
			 * O_NDELAY: non-blocking mode
			 */
			*ps32_port_desc = open( t_configuration->sz_serialport_name, O_RDWR | O_NOCTTY | O_NDELAY);

			if( K_DRVSERIAL_CLOSED == *ps32_port_desc)
			{
				perror("DrvSerial_Open: unable to open serial port :" );
				perror(t_configuration->sz_serialport_name);
			} else {
				/* Lock the serial port access to other processes:
				 * LOCK_EX: exclusive lock
				 * LOCK_NB: non-blocking lock request to avoid interblocking in case of incompatible lock */
				s32_error = flock( *ps32_port_desc , LOCK_EX | LOCK_NB);
				if (s32_error != 0)
				{
					perror("DrvSerial_Open: the requested serial port is exclusively locked by another process.");
				} else {
			    	t_Open_Status = E_OK ;
				}
			}

			if (E_OK == t_Open_Status)
			{
				/* No error: configure the serial port */

				/* Check the requested baudrate */
				switch(t_configuration->u32_baudrate)
				{
					case 300:
						bdrate_t = B300;
						break;

					case 1200:
						bdrate_t = B1200;
						break;

					case 2400:
						bdrate_t = B2400;
						break;

					case 4800:
						bdrate_t = B4800;
						break;

					case 9600:
						bdrate_t = B9600;
						break;

					case 19200:
						bdrate_t = B19200;
						break;

					case 38400:
						bdrate_t = B38400;
						break;

					case 57600:
						bdrate_t = B57600;
						break;

					case 115200:
						bdrate_t = B115200;
						break;

					case 230400:
						bdrate_t = B230400;
						break;

					case 460800:
						bdrate_t = B460800;
						break;

					case 921600:
						bdrate_t = B921600;
						break;

					default:
						printf("DrvSerial_Open: invalid baudrate value: %d\n", (int)t_configuration->u32_baudrate);
				    	t_Open_Status = E_NOT_OK ;
				    	bdrate_t = B9600 ;
						break;
				}

				/* Check the requested bit size */
				switch(t_configuration->u8_bitsize)
				{
					case 7:
						bsize_t = CS7;
						break;

					case 8:
						bsize_t = CS8;
						break;

					default:
						printf("DrvSerial_Open: invalid bit size value: %d\n", t_configuration->u8_bitsize);
				    	t_Open_Status = E_NOT_OK ;
				    	bsize_t = CS8 ;
						break;
				}

				/* Check the requested parity mode */
				switch(t_configuration->t_parity_mode)
				{
					case E_DRVSERIAL_NO_PARITY:
						parcfg_cflag_t = 0;
						parcfg_iflag_t = IGNPAR;
						break;

					case E_DRVSERIAL_EVEN_PARITY:
						parcfg_cflag_t = PARENB;
						parcfg_iflag_t = INPCK;
						break;

					case E_DRVSERIAL_ODD_PARITY:
						parcfg_cflag_t = (PARENB | PARODD);
						parcfg_iflag_t = INPCK;
						break;

					default:
						printf("DrvSerial_Open: invalid parity mode: %d\n", t_configuration->t_parity_mode );
				    	t_Open_Status = E_NOT_OK ;
					  break;
				}

				/* Check the requested stop bit mode */
				if( t_configuration->b_stopbitena == K_FALSE )
				{
					stopbit_t = 0;
				} else {
					stopbit_t = CSTOPB;
				}
			}

			if ( E_OK == t_Open_Status )
			{
				/* Configure the serial port */
				/* Clear the configuration structure */
				memset(&serialport_cfg_t, 0, sizeof(serialport_cfg_t));

				/* Update the configuration structure with requested modes, plus:
				 * - CLOCAL: ignore modem control lines
				 * - CREAD: enable the receiver
				 * - VMIN, VTIME: no serial port blocking on data read
				 */
				serialport_cfg_t.c_iflag = parcfg_iflag_t;
				serialport_cfg_t.c_oflag = 0;
				serialport_cfg_t.c_cflag = bsize_t | parcfg_cflag_t | stopbit_t | CLOCAL | CREAD;
				serialport_cfg_t.c_lflag = 0;
				serialport_cfg_t.c_cc[VMIN] = 0;
				serialport_cfg_t.c_cc[VTIME] = 0;

				/* Configure the baudrate for serial input and output */
				cfsetispeed(&serialport_cfg_t, bdrate_t);
				cfsetospeed(&serialport_cfg_t, bdrate_t);

				/* Write the new serial port configuration and take it immediately into account */
				s32_error = tcsetattr( *ps32_port_desc , TCSANOW, &serialport_cfg_t);
				if (s32_error == -1)
				{
					perror("DrvSerial_Open: unable to adjust serial port settings");
			    	t_Open_Status = E_NOT_OK ;
				} else {
					/* Activate Data Terminal Ready and Request To Send RS232 signals */

					/* Get current serial port status */
					s32_error = ioctl( *ps32_port_desc , TIOCMGET, &s32_status);
					if (s32_error == -1)
					{
						perror("DrvSerial_Open: unable to get current port status");
				    	t_Open_Status = E_NOT_OK ;
					} else {
						/* Turn on DTR and RTS */
						s32_status |= TIOCM_DTR | TIOCM_RTS;

						/* Set the serial port new status */
						s32_error = ioctl( *ps32_port_desc , TIOCMSET, &s32_status);
						if (s32_error == -1)
						{
							perror("serialdrv_init: unable to set new port status");
					    	t_Open_Status = E_NOT_OK ;
						}
					}
				}
			}

			if ( E_NOT_OK == t_Open_Status )
			{
				/* Error occurred, close the serial port descriptor */
				DrvSerial_Close( ps32_port_desc ) ;
			}
    	}
    	else
    	{
    		t_Open_Status = E_OK ;
    	}
    }
    else
    {
		/* Module is not initialized, Halt */
    	;
    }

    /*! return code */
    return(t_Open_Status);
}

/**
 * \fn  Std_ReturnType DrvSerial_Close(void)
 * \brief this function closes serial link
 *
 * \param  ps32_port_desc [in/out] : Serial port description
 *
* \return Std_ReturnType :
*         - E_OK if good return function
*         - E_NOT_OK if not
 */
Std_ReturnType DrvSerial_Close( sint32 * const ps32_port_desc )
{
    /*! Declaration */
    Std_ReturnType t_Send_Status;

    /*! Initialization */
    t_Send_Status=E_OK;

    /*! Function Body */
    if( (E_NOT_OK != gt_InitSerialDriver_Status) && (NULL_PTR != ps32_port_desc) )
    {
    	if( K_DRVSERIAL_CLOSED != *ps32_port_desc )
        {
        	close( *ps32_port_desc ) ;
        	*ps32_port_desc = K_DRVSERIAL_CLOSED ;
        }
    }
    else
    {
		/* Module is not initialized, Halt */
        t_Send_Status=E_NOT_OK;
    }

    /*! return code */
    return(t_Send_Status);
}

/**
 * \fn  Std_ReturnType DrvSerial_Send( sint32 s32_port_desc , uint8 *au8_buffer, uint32 u32_buffer_size , sint32 *ps32_written )
 * \brief this function sends datas on serial link
 *
 * \param  s32_port_desc [in] : Serial port description
 * \param  au8_buffer [in] : bytes to send
 * \param  u32_buffer_size [in] : Size of bytes to send
 * \param  ps32_written [out] : Number of datas written (< 0 : error )
 *
* \return Std_ReturnType :
*         - E_OK if good return function
*         - E_NOT_OK if not
 */
Std_ReturnType DrvSerial_Send( const sint32 s32_port_desc , const uint8 * const au8_buffer, const uint32 u32_buffer_size , sint32 * const ps32_written )
{
    /*! Declaration */     
    Std_ReturnType t_Send_Status;

    /*! Initialization */
    t_Send_Status = E_NOT_OK ;

    /*! Function Body */
    if( NULL_PTR != ps32_written )
    {
		*ps32_written = -1 ;
		if( (E_NOT_OK != gt_InitSerialDriver_Status) && (K_DRVSERIAL_CLOSED != s32_port_desc) && (NULL_PTR != au8_buffer) )
		{

				*ps32_written = write( s32_port_desc , au8_buffer, u32_buffer_size ) ;
				if( -1 < *ps32_written )
				{
					t_Send_Status = E_OK ;
				}
		}
		else
		{
			/* Module is not initialized, Halt */
		    ;
		}
    }

    /*! return code */
    return(t_Send_Status);
}

/**
 * \fn  Std_ReturnType DrvSerial_Receive( sint32 s32_port_desc , uint8 *au8_buffer, uint32 u32_buffer_size , sint32 *ps32_read )
 * \brief this function receives datas from serial link
 *
 * \param  s32_port_desc [in] : Serial port description
 * \param  au8_buffer [out] : bytes reading
 * \param  u32_buffer_size [in] : Number of bytes to read
 * \param  ps32_read [out] : Number of datas reading (< 0 : error )
 *
* \return Std_ReturnType :
*         - E_OK if good return function
*         - E_NOT_OK if not
 */
Std_ReturnType DrvSerial_Receive( const sint32 s32_port_desc , uint8 * const au8_buffer, const uint32 u32_buffer_size , sint32 * const ps32_read )
{
    /*! Declaration */     
    Std_ReturnType t_Receive_Status;
    sint32 s32_error ;
    sint32 s32_status ;

    /*! Initialization */
    t_Receive_Status = E_NOT_OK ;

    /*! Function Body */
    if( NULL_PTR != ps32_read )
    {
    	*ps32_read = -1 ;
    	if( (E_NOT_OK != gt_InitSerialDriver_Status) && (K_DRVSERIAL_CLOSED != s32_port_desc) && (NULL_PTR != au8_buffer) )
		{
    		s32_error = ioctl( s32_port_desc , TIOCMGET, &s32_status) ;
    		if( 0 == s32_error )
    		{
				t_Receive_Status = E_OK ;
				*ps32_read = read( s32_port_desc , au8_buffer , u32_buffer_size ) ;
    		}
		}
		else
		{
			/* Module is not initialized, Halt */
		    ;
		}
    }

    /*! return code */
    return(t_Receive_Status);
}

/**
 * \fn  Std_ReturnType DrvSerial_Flush( sint32 s32_port_desc )
 * \brief this function flush buffer
 *
 * \param  s32_port_desc [in] : Serial port description
 *
* \return Std_ReturnType :
*         - E_OK if good return function
*         - E_NOT_OK if not
 */
Std_ReturnType DrvSerial_Flush( const sint32 s32_port_desc )
{
    /*! Declaration */
    Std_ReturnType t_Status;
    sint32 s32_error ;
    sint32 s32_status ;

    /*! Initialization */
    t_Status = E_NOT_OK ;

    /*! Function Body */
	if( (E_NOT_OK != gt_InitSerialDriver_Status) && (K_DRVSERIAL_CLOSED != s32_port_desc) )
	{
		s32_error = ioctl( s32_port_desc , TIOCMGET, &s32_status) ;
		if( 0 == s32_error )
		{
			t_Status = E_OK ;
			tcflush( s32_port_desc , TCIOFLUSH ) ;
		}
	}
	else
	{
		/* Module is not initialized, Halt */
	    ;
	}

    /*! return code */
    return(t_Status);
}
