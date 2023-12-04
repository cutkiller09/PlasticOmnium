/*============================================================================*/
/*                        	ROBOSOFT                                      */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*/
/*! 
 * $Source: Service_ErrorReport.h 
 * $Author: SLoisel 
 * $Date: 2015/12/08  
 */
/*============================================================================*/
/* DESCRIPTION :                                                              */
/** \file  Service_ErrorReport.h 
  * \brief Fichier Header du Service ErrorReport
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
#include "Service_BatteryManager.h"
#include "Service_Command.h"
#include "Service_Configuration.h"
#include "Service_InertiaMobileUnit.h"
#include "Service_Log.h"
#include "Service_Motor.h"
#include "Service_Scheduler.h"
#include "Service_Signalisation.h"
#include "Service_Calibration.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Constants and types 							      */
/*============================================================================*/
/**
 * @struct ts_SrvSignalisation_configuration
 * @brief define the general Signalisation Configuration
 */
typedef struct
{
	te_SrvSignalisation_error e_SrvSignalisation_error;
	te_SrvMotor_error e_SrvMotor_error;
	te_SrvSched_error e_SrvSched_error;
	te_SrvLidar_error e_SrvLidar_error;
	te_SrvImu_error   e_SrvImu_error;
	te_SrvCom_error   e_SrvCom_error;
	te_SrvBat_error   e_SrvBat_error;
	te_SrvConf_error  e_SrvConf_error;
	te_SrvCal_error   e_SrvCal_error;
}ts_SrvErrorReport_Data;


/* Exported Variables 						              */
/*============================================================================*/

/* Exported functions prototypes 					      */
/*============================================================================*/    
 
Std_ReturnType SrvErr_Initialize(void) ;
Std_ReturnType SrvErr_Get(ts_SrvErrorReport_Data *ps_SrvErrorReport_Data) ;
Std_ReturnType SrvErr_Manage(void);
