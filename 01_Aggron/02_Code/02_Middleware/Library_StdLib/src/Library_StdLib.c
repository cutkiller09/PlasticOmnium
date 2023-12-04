/*============================================================================*/
/*                        	ROBOSOFT                                      */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*/
/*! 
 * $Source: Library_StdLib.c 
 * $Author: SLoisel 
 * $Date: 2015/12/08  
 */
/*============================================================================*/
/* DESCRIPTION :                                                              */
/** \file  Library_StdLib.c 
  * \brief Fichier Source du Service StdLib
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
#include "time.h"

/* Constants and types  						      */
/*============================================================================*/ 
   
/* Variables 								      */
/*============================================================================*/

/* Private functions prototypes 					      */
/*============================================================================*/ 

/* Inline functions 							      */
/*============================================================================*/

/* Private functions 							      */
/*============================================================================*/

/* Public functions 							      */
/*============================================================================*/


/**
 * \fn  Std_ReturnType StdLib_GetTime(uint32 * const pu32_Time_sec , uint32 * const pu32_Time_ns )
 * \brief this function updates internal time registers
 *
 * \param  void
 *
* \return Std_ReturnType :
*         - E_OK if pu32_Time is not null and clock_gettime return code is not null
*         - E_NOT_OK if not
 */
Std_ReturnType StdLib_GetTime(uint32 * const pu32_Time_sec , uint32 * const pu32_Time_ns )
{
    /*! Declaration */
    Std_ReturnType t_GetTime_Status;
	struct timespec s_current_timespec ;
	sint32 s32_gettime_result ;

	/*! Initialization */
	t_GetTime_Status = E_OK ;
	s_current_timespec.tv_sec = 0 ;
	s_current_timespec.tv_nsec = 0 ;

	/*! Function Body */
	if( (NULL_PTR != pu32_Time_sec) && (NULL_PTR != pu32_Time_ns) )
	{
		/*! Read Current clock */
		s32_gettime_result = 0; // TODO : clock_gettime( CLOCK_REALTIME , &s_current_timespec ) ;
		if( 0 == s32_gettime_result )
		{
			/*! IF read successful */
			*pu32_Time_sec 	= *pu32_Time_sec + 2; // TODO : s_current_timespec.tv_sec ;
			*pu32_Time_ns 	= *pu32_Time_ns + 1; // TODO : s_current_timespec.tv_nsec ;
		}
		else
		{
			t_GetTime_Status = E_NOT_OK ;
		}
	}
	else
	{
		/* Module is not initialized, Halt */
		t_GetTime_Status = E_NOT_OK ;
	}

    /*! return code */
    return(t_GetTime_Status);
}
