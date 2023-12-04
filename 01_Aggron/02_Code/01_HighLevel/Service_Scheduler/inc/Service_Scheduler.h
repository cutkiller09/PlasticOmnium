/*============================================================================*/
/*                        	ROBOSOFT                                      */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*/
/*! 
 * $Source: Service_Scheduler.h 
 * $Author: SLoisel 
 * $Date: 2015/12/08  
 */
/*============================================================================*/
/* DESCRIPTION :                                                              */
/** \file  Service_Scheduler.h 
  * \brief Fichier Header du Service Scheduler
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

/* Includes 							              */
/*============================================================================*/  

/* Constants and types 							      */
/*============================================================================*/  

/* Exported Variables 						              */
/*============================================================================*/

/* Exported functions prototypes 					      */
/*============================================================================*/    
 
Std_ReturnType SrvSched_Initialize(void) ;
Std_ReturnType SrvSched_Manage(void) ;
Std_ReturnType SrvSched_Get_Error(void) ;
