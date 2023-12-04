/*============================================================================*/
/*                              ROBOSOFT                                      */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*/
/*! 
 * $Source: Driver_TcpSocketDriver.c 
 * $Author: MDega 
 * $Date: 2015/12/30
 */
/*============================================================================*/
/* DESCRIPTION :                                                              */
/** \file  Driver_TcpSocketDriver.c 
  * \brief Source file of TcpSocketDriver Service
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

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <netdb.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <termios.h>

/* Constants and types  						      */
/*============================================================================*/ 
   
/* Variables 								      */
/*============================================================================*/ 
static Std_ReturnType gt_InitTcpSocketDriver_Status = E_NOT_OK ;

/* Private functions prototypes 					      */
/*============================================================================*/ 

/* Inline functions 							      */
/*============================================================================*/

/* Private functions 							      */
/*============================================================================*/

/**
 * \fn  Std_ReturnType DrvTcpSocket_Initialize(void) 
 * \brief this function initializes internal variables
 *
 * \param  void
 *
* \return Std_ReturnType :
*         - E_OK if good return function
*         - E_NOT_OK if not
 */
Std_ReturnType DrvTcpSocket_Initialize(void) 
{
    /*! Declaration */     
    Std_ReturnType t_Initialize_Status; 

    /*! Initialization */
    t_Initialize_Status=E_OK; 
    gt_InitTcpSocketDriver_Status = t_Initialize_Status ;

    /*! return code */
    return(t_Initialize_Status);
}

/**
 * \fn  Std_ReturnType DrvTcpSocket_Open( const DrvTcpsocket_cfg_type * const pt_configuration , DrvTcpsocket_descriptor_type * const pt_tcpsocket_desc ,
									E_DRVTCPSOCKET_ERROR * const pt_error )
 * \brief this function opens tcp socket link
 *
 * \param  t_configuration [in] : Socket configuration (server address, server port, blocking mode, timeout)
 * \param  ps32_tcpsocket_desc [out] : link descriptor
 * \param  pt_error [out] : Error type
 *
* \return Std_ReturnType :
*         - E_OK if good return function
*         - E_NOT_OK if not
 */
Std_ReturnType DrvTcpSocket_Open( const DrvTcpsocket_cfg_type * const pt_configuration , DrvTcpsocket_descriptor_type * const pt_tcpsocket_desc ,
									E_DRVTCPSOCKET_ERROR * const pt_error )
{
    /*! Declaration */
    Std_ReturnType t_Open_Status;
    struct sockaddr_in t_SockAddrIn;
    struct hostent * t_serverHostName ;
    sint32 s32_flags ;
    sint32 s32_error ;

    /*! Initialization */
	t_Open_Status = E_NOT_OK ;

    /*! Function Body */
    if( (E_NOT_OK != gt_InitTcpSocketDriver_Status) && (NULL_PTR != pt_tcpsocket_desc) && (NULL_PTR != pt_configuration) && (NULL_PTR != pt_error) )
    {
		t_Open_Status = E_OK ;

		if( (E_DRVTCPSOCKET_DESCRIPTOR_CLOSED == *pt_tcpsocket_desc) )
		{
			 *pt_error = E_DRVTCPSOCKET_ERROR_NOERROR ;

			/* Set TCP  Datagram */
			*pt_tcpsocket_desc = socket(AF_INET, SOCK_STREAM, 0);

			/* Check Socket Init Error */
			if( -1 == *pt_tcpsocket_desc )
			{
				*pt_error =  E_DRVTCPSOCKET_ERROR_CONNECT_INVALID_SOCKET_ERROR ;
			}

			/* If O.S Unix Set Zero to  0x00 to sockaddr_in gt_SockAddrIn */
			if( E_DRVTCPSOCKET_ERROR_NOERROR == *pt_error )
			{
				memset( &t_SockAddrIn , 0 , sizeof(t_SockAddrIn) );
			}

			/* -> Get Host By Name */
			if( E_DRVTCPSOCKET_ERROR_NOERROR == *pt_error )
			{
				t_serverHostName = gethostbyname( (char*) pt_configuration->au8_serverAddress ) ;
				/* Check if Address is Known */
				if( t_serverHostName == NULL )
				{
					*pt_error = E_DRVTCPSOCKET_ERROR_CONNECT_INVALID_SERVER_ADDRESS_ERROR ;
				}
			}

			/* Initialize Socket Configuration */
			if( E_DRVTCPSOCKET_ERROR_NOERROR == *pt_error )
			{
				t_SockAddrIn.sin_family = AF_INET;
				t_SockAddrIn.sin_port = htons( pt_configuration->u32_server_port ) ;
				memcpy( &t_SockAddrIn.sin_addr , t_serverHostName->h_addr , t_serverHostName->h_length ) ;
			}

			/* Connect Socket */
			if( E_DRVTCPSOCKET_ERROR_NOERROR == *pt_error )
			{
				s32_error = connect( *pt_tcpsocket_desc , (struct sockaddr *)&t_SockAddrIn, sizeof(t_SockAddrIn) ) ;
				if( -1 == s32_error )
				{
					*pt_error = E_DRVTCPSOCKET_ERROR_CONNECT_FUNCTION_CONNECT_ERROR ;
				}
			}
			/* Set Flag Blocking / Non Blocking */
			if( E_DRVTCPSOCKET_ERROR_NOERROR == *pt_error )
			{
				if( K_TRUE == pt_configuration->b_blocking_mode )
				{
					s32_flags = fcntl( *pt_tcpsocket_desc , F_GETFL ) ;
					if( -1 != s32_flags)
					{
						fcntl( *pt_tcpsocket_desc , F_SETFL , s32_flags & ~O_NONBLOCK ) ;
					}
					else
					{
						*pt_error = E_DRVTCPSOCKET_ERROR_CONNECT_FUNCTION_FCNTL_ERROR ;
					}
				}
				else
				{
					s32_flags = fcntl( *pt_tcpsocket_desc , F_GETFL ) ;
					if( -1 != s32_flags)
					{
						fcntl( *pt_tcpsocket_desc , F_SETFL , s32_flags | O_NONBLOCK ) ;
					}
					else
					{
						*pt_error = E_DRVTCPSOCKET_ERROR_CONNECT_FUNCTION_FCNTL_ERROR ;
					}
				}
			}

			/* Set Internal Flag */
			if( E_DRVTCPSOCKET_ERROR_NOERROR != *pt_error )
			{
				t_Open_Status = E_NOT_OK ;
				if( E_DRVTCPSOCKET_DESCRIPTOR_CLOSED != *pt_tcpsocket_desc )
				{
					close( *pt_tcpsocket_desc ) ;
				}
			}
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
 * \fn  Std_ReturnType DrvTcpSocket_Close( DrvTcpsocket_descriptor_type * const pt_tcpsocket_desc , E_DRVTCPSOCKET_ERROR * const pt_error )
 * \brief this function closes link
 *
 * \param  pt_tcpsocket_desc  [out] : link descriptor
 * \param  pt_error  [out] : Error type
 *
* \return Std_ReturnType :
*         - E_OK if good return function
*         - E_NOT_OK if not
 */
Std_ReturnType DrvTcpSocket_Close( DrvTcpsocket_descriptor_type * const pt_tcpsocket_desc , E_DRVTCPSOCKET_ERROR * const pt_error )
{
    /*! Declaration */
    Std_ReturnType t_Status;
    sint32 s32_error,s32_status ;

    /*! Initialization */
    t_Status=E_NOT_OK;

    /*! Function Body */
    if( (E_NOT_OK != gt_InitTcpSocketDriver_Status) && (NULL_PTR != pt_tcpsocket_desc) && (NULL_PTR != pt_error) )
    {
    	t_Status = E_OK ;
        if( E_DRVTCPSOCKET_DESCRIPTOR_CLOSED != *pt_tcpsocket_desc )
        {
        	*pt_error = E_DRVTCPSOCKET_ERROR_NOERROR ;

        	s32_error = ioctl( *pt_tcpsocket_desc , TIOCMGET, &s32_status) ;
			if( 0 == s32_error )
			{
				t_Status = E_OK ;
				tcflush( *pt_tcpsocket_desc , TCIOFLUSH ) ;

				usleep(200000); /* Wait for flush to be activated */

				s32_error = close( *pt_tcpsocket_desc ) ;
				if( -1 == s32_error )
				{
					t_Status = E_NOT_OK ;
					*pt_error = E_DRVTCPSOCKET_ERROR_DISCONNECT_FUNCTION_CLOSE_ERROR ;
				}
				if( E_DRVTCPSOCKET_ERROR_NOERROR == *pt_error )
				{
					*pt_tcpsocket_desc = E_DRVTCPSOCKET_DESCRIPTOR_CLOSED ;
				}
			}
			else
			{
				*pt_error = E_DRVTCPSOCKET_ERROR_DISCONNECT_FUNCTION_CLOSE_ERROR ;
			}
        }
        else
		{
			*pt_error = E_DRVTCPSOCKET_ERROR_DISCONNECT_SOCKET_NOT_OOPEN_ERROR ;
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



/**
* \fn Std_ReturnType DrvTcpSocket_Send( const DrvTcpsocket_descriptor_type t_tcpsocket_desc , uint8 * const au8_buffer,
const uint32 u32_buffer_size , sint32 * const ps32_written )
* \brief this function sends datas on ethernet link
*
* \param t_tcpsocket_desc [in] : ethernet descriptor
* \param au8_buffer [in] : bytes to send
* \param u32_buffer_size [in] : Size of bytes to send
* \param ps32_written [out] : Number of datas written (< 0 : error )
*
* \return Std_ReturnType :
* - E_OK if good return function
* - E_NOT_OK if not
*/
Std_ReturnType DrvTcpSocket_Send( const DrvTcpsocket_descriptor_type t_tcpsocket_desc ,
								  const uint8 * const au8_buffer,
							      const uint32 u32_buffer_size , sint32 *
								  const ps32_written )
{
    /*! Declaration */     
    Std_ReturnType t_Send_Status;
    fd_set rfds;
    fd_set wfds;
    FD_ZERO(&rfds);
    FD_ZERO(&wfds);
    sint32 s32_error ;

    /*! Initialization */
    t_Send_Status = E_NOT_OK ;

    /*! Function Body */
    if( NULL_PTR != ps32_written )
    {
		*ps32_written = -1 ;
		if( (E_NOT_OK != gt_InitTcpSocketDriver_Status) && (E_DRVTCPSOCKET_DESCRIPTOR_CLOSED != t_tcpsocket_desc) && (NULL_PTR != au8_buffer) )
		{
			FD_SET(t_tcpsocket_desc, &rfds);
			FD_SET(t_tcpsocket_desc, &wfds);

			s32_error = select(t_tcpsocket_desc + 1, &rfds, &wfds, NULL, NULL) ;
		    if( 0 > s32_error )
		    {
		        perror("select");
		    }
		    else
		    {
		    	/*! Check if file descriptor ready to be written */
		        if(FD_ISSET(t_tcpsocket_desc, &wfds))
		        {
		        	*ps32_written = send( t_tcpsocket_desc , au8_buffer , u32_buffer_size , 0 ) ;

					if( (0 == errno) )
					{
						/* SUCCESS */
						t_Send_Status = E_OK ;
					}
					else
					{
#ifdef K_DEBUG
						printf("errno : %i \n",errno);
#endif
						;
					}
		        }
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
 * \fn  Std_ReturnType DrvTcpSocket_Receive( const DrvTcpsocket_descriptor_type t_tcpsocket_desc , const DrvTcpsocket_cfg_type * const pt_configuration ,
		uint8 * const au8_buffer, const uint32 u32_buffer_size , sint32 * const ps32_read, E_DRVTCPSOCKET_ERROR * const pt_error )
 * \brief this function reads datas from ethernet link
 *
 * \param  t_tcpsocket_desc [in] : ethernet descriptor
 * \param  au8_buffer [in] : bytes to send
 * \param  u32_buffer_size [in] : Size of bytes to send
 * \param  ps32_written [out] : Number of datas written (< 0 : error )
 *
* \return Std_ReturnType :
*         - E_OK if good return function
*         - E_NOT_OK if not
 */
Std_ReturnType DrvTcpSocket_Receive( const DrvTcpsocket_descriptor_type t_tcpsocket_desc , const DrvTcpsocket_cfg_type * const pt_configuration ,
		uint8 * const au8_buffer, const uint32 u32_buffer_size , sint32 * const ps32_read, E_DRVTCPSOCKET_ERROR * const pt_error )
{
    /*! Declaration */     
    Std_ReturnType t_Receive_Status; 
    bool b_read ;
	fd_set readfds ;
	fd_set masterfds ;
	struct timeval t_timeout ;
    sint32 s32_error ;

    /*! Initialization */
    t_Receive_Status = E_NOT_OK ;
	FD_ZERO(&masterfds);
	b_read = K_FALSE ;

    /*! Function Body */
    if( (NULL_PTR != ps32_read) && (NULL_PTR != pt_error) )
    {
    	*ps32_read = 0 ;
    	*pt_error = E_DRVTCPSOCKET_ERROR_NOERROR ;

		if( (E_NOT_OK != gt_InitTcpSocketDriver_Status) && (NULL_PTR != au8_buffer) && (NULL_PTR != pt_configuration) )
		{
			if( (E_DRVTCPSOCKET_DESCRIPTOR_CLOSED != t_tcpsocket_desc) )
			{
				if( (K_TRUE == pt_configuration->b_blocking_mode) && (0 < pt_configuration->u32_timeout_sec ) )
				{
					/* Prepare time-out in blocking mode */
					FD_SET( t_tcpsocket_desc , &masterfds ) ;
					memcpy( &readfds , &masterfds , sizeof(fd_set) ) ;

					/* Configure time-out */
					t_timeout.tv_sec = pt_configuration->u32_timeout_sec ;
					t_timeout.tv_usec = 0 ;

					s32_error = select( t_tcpsocket_desc+1 , &readfds , NULL , NULL , &t_timeout ) ;
					if( 0 > s32_error )
					{
						*pt_error = E_DRVTCPSOCKET_ERROR_CONNECT_CONF_TIMEOUT_ERROR ;
					}
					else
					{
						/* Wait datas for time-out period */
						if( 1 == FD_ISSET( t_tcpsocket_desc , &readfds) )
						{
							// If OK, we can reading data
							b_read = K_TRUE ;
						}
						else
						{
							*pt_error = E_DRVTCPSOCKET_ERROR_READ_FUNCTION_RECV_NO_DATA_BEFORE_TIMEOUT ;
						}
					}
				}
				else
				{
					b_read = K_TRUE ;
				}

				if( K_TRUE == b_read )
				{
					t_Receive_Status = E_OK ;
					*ps32_read = recv( t_tcpsocket_desc , au8_buffer , u32_buffer_size , 0 ) ;
					if( 0 > *ps32_read )
					{
						*ps32_read = 0 ;
					}
				}
			}
    	    else
    	    {
    	        *pt_error = E_DRVTCPSOCKET_ERROR_READ_SOCKET_NOT_OPEN_ERROR ;
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
 * \fn  Std_ReturnType DrvTcpSocket_Get_NumberOfBytes( const DrvTcpsocket_descriptor_type t_tcpsocket_desc , uint32 * const pu32_byteSize )
 * \brief this function gets the number of available data
 *
 * \param  t_tcpsocket_desc [in] : Socket descriptor
 * \param  pu32_byteSize [out] : Positive value = Number of Bytes availables , if an error occurs negative value among
 *
* \return Std_ReturnType :
*         - E_OK if good return function
*         - E_NOT_OK if not
 */
Std_ReturnType DrvTcpSocket_Get_NumberOfBytes( const DrvTcpsocket_descriptor_type t_tcpsocket_desc , uint32 * const pu32_byteSize )
{
    /*! Declaration */
    Std_ReturnType t_Status;

    /*! Initialization */
    t_Status = E_NOT_OK ;

    /*! Function Body */
    if( NULL_PTR != pu32_byteSize )
    {
		*pu32_byteSize = 0 ;

		if( (E_NOT_OK != gt_InitTcpSocketDriver_Status) && (E_DRVTCPSOCKET_DESCRIPTOR_CLOSED != t_tcpsocket_desc) )
		{
			ioctl( t_tcpsocket_desc , FIONREAD , pu32_byteSize ) ;
			if( 0 == errno )
			{
				/* SUCCESS */
				t_Status = E_OK ;
			}
			else
			{
				*pu32_byteSize = 0 ;
			}
		}
		else
		{
			/* Module is not initialized, Halt */
		    ;
		}
    }

    /*! return code */
    return(t_Status);
}
