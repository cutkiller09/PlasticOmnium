/*============================================================================*/
/*                        	ROBOSOFT                                      */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*/
/*! 
 * $Source: Service_Log.c 
 * $Author: Slo 
 * $Date: 2015/12/08  
 */
/*============================================================================*/
/* DESCRIPTION :                                                              */
/** \file  Service_Log.c 
  * \brief Fichier Source du Service Log
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
#include "Service_Log.h" 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Constants and types  						      */
/*============================================================================*/ 
#define K_LOG_1MS						"/var/www/html/Log1ms_Tangela"
#define K_LOG_10MS						"/var/www/html/Log10ms_Tangela"
#define K_LOG_1MS_FILE					"/var/www/html/Log1ms_Tangela.txt"
#define K_LOG_10MS_FILE					"/var/www/html/Log10ms_Tangela.txt"
#define K_SEPARATOR_MAX_LEN 1000
#define K_ENABLE_LOG


/* Variables 								      */
/*============================================================================*/ 
static Std_ReturnType gt_InitLog_Status=E_NOT_OK; /* Global Initialization variable */
static uint8 au8_separator[K_SEPARATOR_MAX_LEN];
static te_LogLevel ge_LogLevel=E_LOG_LEVEL_0;
static te_LogSelection ge_LogSelection;
static te_LogState ge_LogState;
static uint8 gau8_LogFile_1ms[100];
static uint8 gau8_LogFile_10ms[100];

/* Private functions prototypes 					      */
/*============================================================================*/ 

/* Inline functions 							      */
/*============================================================================*/

/* Private functions 							      */
/*============================================================================*/

/* Public functions 							      */
/*============================================================================*/


/**
 * \fn  Std_ReturnType SrvLog_Initialize(void) 
 * \brief this function <TBD>
 *
 * \param  <parameter-name> { parameter description }
 *
* \return Std_ReturnType :
*         - E_OK if good return function
*         - E_NOT_OK if not
 */
Std_ReturnType SrvLog_Initialize(te_LogSelection e_LogSelection)
{
    /*! Declaration */     
    Std_ReturnType t_Initialize_Status;
#ifdef K_ENABLE_LOG
    uint32 u32_size;
    FILE * fd ;
    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    /*! Initialization */
	u32_size=sprintf ((char *)gau8_LogFile_1ms, "%s_%02d_%02d_%02d_%02d:%02d.log",
			  K_LOG_1MS,
			  (timeinfo->tm_year+1900),
			  (timeinfo->tm_mon+1),
			  timeinfo->tm_mday,
			  timeinfo->tm_hour,
			  timeinfo->tm_min);

	u32_size=sprintf ((char *)gau8_LogFile_10ms, "%s_%02d_%02d_%02d_%02d:%02d.log",
			  K_LOG_10MS,
			  (timeinfo->tm_year+1900),
			  (timeinfo->tm_mon+1),
			  timeinfo->tm_mday,
			  timeinfo->tm_hour,
			  timeinfo->tm_min);

    /*! Function Body */
    ge_LogLevel=E_LOG_LEVEL_0;
    ge_LogSelection=e_LogSelection;
    switch(e_LogSelection)
    {
		case E_LOG_1MS :
			fd = fopen( (const char *) gau8_LogFile_1ms , "wb") ;
		    gt_InitLog_Status=E_OK;
		    if (NULL_PTR!=fd)
		    {
				fprintf( fd , "%s" , "/**********************************************************/\n" ) ;
				fprintf( fd , "%s" , "\t\t TANGELA RAISE EVENT LOG\n") ;
				fprintf( fd , "%s" , "/**********************************************************/\n" ) ;
				fclose( fd ) ;
		    }
			fd = fopen( (const char *) K_LOG_1MS_FILE , "wb") ;
		    gt_InitLog_Status=E_OK;
		    if (NULL_PTR!=fd)
		    {
				fprintf( fd , "%s" , "/**********************************************************/\n" ) ;
				fprintf( fd , "%s" , "\t\t TANGELA RAISE EVENT LOG\n") ;
				fprintf( fd , "%s" , "/**********************************************************/\n" ) ;
				fclose( fd ) ;
		    }
			break;
		case E_LOG_10MS :
			fd = fopen( (const char *) gau8_LogFile_10ms , "wb") ;
		    gt_InitLog_Status=E_OK;
		    if (NULL_PTR!=fd)
		    {
				fprintf( fd , "%s" , "/**********************************************************/\n" ) ;
				fprintf( fd , "%s" , "\t\t TANGELA RAISE EVENT LOG\n") ;
				fprintf( fd , "%s" , "/**********************************************************/\n" ) ;
				fclose( fd ) ;
		    }
			fd = fopen( (const char *) K_LOG_10MS_FILE , "wb") ;
		    gt_InitLog_Status=E_OK;
		    if (NULL_PTR!=fd)
		    {
				fprintf( fd , "%s" , "/**********************************************************/\n" ) ;
				fprintf( fd , "%s" , "\t\t TANGELA RAISE EVENT LOG\n") ;
				fprintf( fd , "%s" , "/**********************************************************/\n" ) ;
				fclose( fd ) ;
		    }
			break;
		default :
		    gt_InitLog_Status=E_NOT_OK;
			break;
    }

    system ("/var/www/html/Script_GestionLog.sh");

#else
    gt_InitLog_Status=E_OK;
#endif

    t_Initialize_Status=gt_InitLog_Status;

    /*! return code */
    return(t_Initialize_Status);
}


/**
 * \fn  Std_ReturnType SrvLog_Get_LogFile(uint8 const *pu8_LogFile_10ms, uint8 const *pu8_LogFile_1ms)
 * \brief this function will return 1ms and 10ms Log File
 *
 * \param  <uint8 const *pu8_LogFile_10ms> { Log 10ms Data Name }
 * 		   <uint8 const *pu8_LogFile_1ms> { Log 1ms Data Name }
 *
* \return Std_ReturnType :
*         - E_OK if good return function
*         - E_NOT_OK if not
 */
Std_ReturnType SrvLog_Get_LogFile(uint8 *pu8_LogFile_10ms, uint8 *pu8_LogFile_1ms)
{
    /*! Declaration */
    Std_ReturnType t_GetLog_Status;

    /*! Initialization */
	if	((NULL_PTR!=pu8_LogFile_1ms)&&
		(NULL_PTR!=pu8_LogFile_10ms))
	{
		t_GetLog_Status=E_OK;
		memcpy(pu8_LogFile_10ms,gau8_LogFile_10ms,sizeof(gau8_LogFile_10ms));
		memcpy(pu8_LogFile_1ms,gau8_LogFile_1ms,sizeof(gau8_LogFile_1ms));
	}
	else
	{
		t_GetLog_Status=E_NOT_OK;
	}

    /*! return code */
	return(t_GetLog_Status);
}


/**
 * \fn  Std_ReturnType SrvLog_Get(void)
 * \brief this function <TBD>
 *
 * \param  <parameter-name> { parameter description }
 *
* \return Std_ReturnType :
*         - E_OK if good return function
*         - E_NOT_OK if not
 */
void SrvLog_Set_Selection(const te_LogSelection e_LogSelection)
{
    /*! Declaration */

    /*! Initialization */
    ge_LogSelection=e_LogSelection;

    /*! return code */
}

/**
 * \fn  void SrvLog_Stop(void)
 * \brief this function will stop Log Service
 *
 * \param  none
 *
* \return none
 */
void SrvLog_Stop(void)
{
    /*! Declaration */

    /*! Initialization */
    ge_LogState=E_LOG_STOP;

    /*! return code */
}

/**
 * \fn  void SrvLog_Start(void)
 * \brief this function will start Log Service
 *
 * \param  none
 *
* \return none
 */
void SrvLog_Start(void)
{
    /*! Declaration */

    /*! Initialization */
    ge_LogState=E_LOG_START;

    /*! return code */
}

/**
 * \fn  void SrvLog_Set_Level(const te_LogLevel e_LogLevel)
 * \brief this function will set log level
 *
 * \param  none
 *
* \return none
 */
void SrvLog_Set_Level(const te_LogLevel e_LogLevel)
{
    /*! Declaration */

    /*! Initialization */

    /*! Function Body */
    if(E_NOT_OK!=gt_InitLog_Status)
    {
    	ge_LogLevel=e_LogLevel;
    }
    else
    {
    	/* Module is not initialized, Halt */
    	;
    }

    /*! return code */
}

/**
 * \fn  void SrvLog_Increment_Level(void)
 * \brief this function will increment Log Level
 *
 * \param  none
 *
* \return none
 */
void SrvLog_Increment_Level(void)
{
    /*! Declaration */

    /*! Initialization */

    /*! Function Body */
    if(E_NOT_OK!=gt_InitLog_Status)
    {
    	switch(ge_LogLevel)
    	{
			case E_LOG_LEVEL_0 :
						ge_LogLevel=E_LOG_LEVEL_1;
						break;
			case E_LOG_LEVEL_1 :
						ge_LogLevel=E_LOG_LEVEL_2;
						break;
			case E_LOG_LEVEL_2:
						ge_LogLevel=E_LOG_LEVEL_3;
						break;
			case E_LOG_LEVEL_3:
						ge_LogLevel=E_LOG_LEVEL_4;
						break;
			case E_LOG_LEVEL_4:
						ge_LogLevel=E_LOG_LEVEL_5;
						break;
			case E_LOG_LEVEL_5:
						ge_LogLevel=E_LOG_LEVEL_6;
						break;
			case E_LOG_LEVEL_6:
						ge_LogLevel=E_LOG_LEVEL_7;
						break;
			case E_LOG_LEVEL_7:
			default:
						break;
    	}
    }
    else
    {
    	/* Module is not initialized, Halt */
    	;
    }

    /*! return code */
}


/**
 * \fn  void SrvLog_Decrement_Level(void)
 * \brief this function will decrement log level
 *
 * \param  none
 *
* \return none
 */
void SrvLog_Decrement_Level(void)
{
    /*! Declaration */

    /*! Initialization */

    /*! Function Body */
    if(E_NOT_OK!=gt_InitLog_Status)
    {
    	switch(ge_LogLevel)
    	{
			case E_LOG_LEVEL_1 :
						ge_LogLevel=E_LOG_LEVEL_0;
						break;
			case E_LOG_LEVEL_2:
						ge_LogLevel=E_LOG_LEVEL_1;
						break;
			case E_LOG_LEVEL_3:
						ge_LogLevel=E_LOG_LEVEL_2;
						break;
			case E_LOG_LEVEL_4:
						ge_LogLevel=E_LOG_LEVEL_3;
						break;
			case E_LOG_LEVEL_5:
						ge_LogLevel=E_LOG_LEVEL_4;
						break;
			case E_LOG_LEVEL_6:
						ge_LogLevel=E_LOG_LEVEL_5;
						break;
			case E_LOG_LEVEL_7:
						ge_LogLevel=E_LOG_LEVEL_6;
						break;
			case E_LOG_LEVEL_0 :
			default:
						break;
    	}
    }
    else
    {
    	/* Module is not initialized, Halt */
    	;
    }

    /*! return code */
}

/**
 * \fn void SrvLog_RaiseEvent_State(const uint8 * const pu8_LogInformation, const Std_ReturnType t_EventStatus )
 * \brief this function will raise a Log event state
 *
 * \param  <uint8 * pu8_LogInformation> { Log Information }
 * 		   <Std_ReturnType t_EventStatus> { Event status }
 *
* \return Std_ReturnType :
*         - E_OK if good return function
*         - E_NOT_OK if not
 */
void SrvLog_RaiseEvent_State(const uint8 * const pu8_LogInformation, const Std_ReturnType t_EventStatus )
{
    /*! Declaration */
#ifdef K_ENABLE_LOG
    Std_ReturnType t_Open_Status;
    uint32 u32_Timestamp_s ;
    uint32 u32_Timestamp_ns ;
    FILE * fd=NULL_PTR ;
    FILE * fd2=NULL_PTR ;


    if((E_NOT_OK!=gt_InitLog_Status)&&
		(NULL_PTR!=pu8_LogInformation)&&
		(E_LOG_STOP!=ge_LogState))
    {
        /*! Initialization */
		u32_Timestamp_s =0;
		u32_Timestamp_ns = 0 ;
		switch(ge_LogSelection)
		{
			case E_LOG_1MS :
				fd = fopen( (const char *) gau8_LogFile_1ms , "ab") ;
				fd2 = fopen( (const char *) K_LOG_1MS_FILE , "ab") ;
				t_Open_Status=E_OK;
				break;
			case E_LOG_10MS :
				fd = fopen( (const char *) gau8_LogFile_10ms , "ab") ;
				fd2 = fopen( (const char *) K_LOG_10MS_FILE , "ab") ;
				t_Open_Status=E_OK;
				break;
			default:
				t_Open_Status=E_NOT_OK;
				break;
		}

    	switch (ge_LogLevel)
    	{
    		case E_LOG_LEVEL_0 :
    				strcpy( (char *)au8_separator, "" ) ;
					break ;
    		case E_LOG_LEVEL_1 :
					strcpy( (char *)au8_separator, "\t -->" ) ;
					break ;
    		case E_LOG_LEVEL_2 :
					strcpy( (char *)au8_separator, "\t\t -->" ) ;
					break ;
    		case E_LOG_LEVEL_3 :
					strcpy( (char *)au8_separator, "\t\t\t -->" ) ;
					break ;
    		case E_LOG_LEVEL_4 :
					strcpy( (char *)au8_separator, "\t\t\t\t -->" ) ;
 					break ;
    		case E_LOG_LEVEL_5 :
					strcpy( (char *)au8_separator, "\t\t\t\t\t -->" ) ;
					break ;
    		case E_LOG_LEVEL_6 :
					strcpy( (char *)au8_separator, "\t\t\t\t\t\t -->" ) ;
					break ;
    		case E_LOG_LEVEL_7 :
					strcpy( (char *)au8_separator, "\t\t\t\t\t\t\t -->" ) ;
					break ;
    		default :
    				break;
    	}
		StdLib_GetTime(&u32_Timestamp_s , &u32_Timestamp_ns);
	    if (NULL_PTR!=fd)
	    {
			if (E_NOT_OK!=t_EventStatus)
			{
				fprintf(fd , "Time : %u %06d us %s %s \t \t E_OK \n", u32_Timestamp_s , u32_Timestamp_ns/1000 , au8_separator, pu8_LogInformation ) ;
			}
			else
			{
				fprintf(fd , "Time : %u %06d us %s %s \t \t E_NOT_OK \n", u32_Timestamp_s , u32_Timestamp_ns/1000 , au8_separator, pu8_LogInformation ) ;
			}
	    }
	    if (NULL_PTR!=fd2)
	    {
			if (E_NOT_OK!=t_EventStatus)
			{
				fprintf(fd2 , "Time : %u %06d us %s %s \t \t E_OK \n", u32_Timestamp_s , u32_Timestamp_ns/1000 , au8_separator, pu8_LogInformation ) ;
			}
			else
			{
				fprintf(fd2 , "Time : %u %06d us %s %s \t \t E_NOT_OK \n", u32_Timestamp_s , u32_Timestamp_ns/1000 , au8_separator, pu8_LogInformation ) ;
			}
	    }

		if ((NULL_PTR!=fd)&&
			(E_NOT_OK!=t_Open_Status))
		{
			fclose( fd ) ;
		}
		if ((NULL_PTR!=fd2)&&
			(E_NOT_OK!=t_Open_Status))
		{
			fclose( fd2 ) ;
		}
    }
    else
    {
    	;
    }
#endif
}
