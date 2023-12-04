/*============================================================================*/
/*                        	ROBOSOFT                                      */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*/
/*! 
 * $Source: Service_ErrorReport.c 
 * $Author: SLoisel 
 * $Date: 2015/12/08  
 */
/*============================================================================*/
/* DESCRIPTION :                                                              */
/** \file  Service_ErrorReport.c 
  * \brief Fichier Source du Service ErrorReport
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
#include "Service_ErrorReport.h"
#include <string.h>

/* Constants and types  						      */
/*============================================================================*/ 
#define K_LOG_ERROR  "/var/www/html/Log_Error"
   
/* Variables 								      */
/*============================================================================*/ 
static Std_ReturnType gt_InitErrorReport_Status; /* Global Initialization variable */
static ts_SrvErrorReport_Data gs_SrvErrorReport_Data;
static uint8 gau8_Log_ErrorFile[100];
   
static bool gb_SrvConf_NewError; 
static bool gb_SrvLidar_NewError; 
static bool gb_SrvSched_NewError; 

/* Private functions prototypes 					      */
/*============================================================================*/ 
static void DataLog_XmlErrorFile(void);

/* Inline functions 							      */
/*============================================================================*/

/* Private functions 							      */
/*============================================================================*/

/* Public functions 							      */
/*============================================================================*/

/**
 * \fn  Std_ReturnType SrvReport_Initialize(void) 
 * \brief this function <TBD>
 *
 * \param  <parameter-name> { parameter description }
 *
* \return Std_ReturnType :
*         - E_OK if good return function
*         - E_NOT_OK if not
 */
Std_ReturnType SrvErr_Initialize(void) 
{
    /*! Declaration */     
    Std_ReturnType t_Initialize_Status;
    uint32 u32_size;
    time_t rawtime;
    struct tm * timeinfo;

    /*! Initialization */
    gb_SrvBat_NewError=K_FALSE;
    gb_SrvCal_NewError=K_FALSE;
	gb_SrvCom_NewError=K_FALSE;
	gb_SrvConf_NewError=K_FALSE;
	gb_SrvImu_NewError=K_FALSE;
	gb_SrvLidar_NewError=K_FALSE;
	gb_SrvMotor_NewError=K_FALSE;
	gb_SrvSched_NewError=K_FALSE;
	gb_SrvSig_NewError=K_FALSE;

    /*! Function Body */
    gt_InitErrorReport_Status=E_OK;
    t_Initialize_Status=gt_InitErrorReport_Status;
    memset(&gs_SrvErrorReport_Data,0,sizeof(ts_SrvErrorReport_Data));

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    /*! Initialization */
	u32_size=sprintf ((char *)gau8_Log_ErrorFile, "%s_%d_%d_%d_%d:%d.log",
			   K_LOG_ERROR,
			  (timeinfo->tm_year+1900),
			  (timeinfo->tm_mon+1),
			  timeinfo->tm_mday,
			  timeinfo->tm_hour,
			  timeinfo->tm_min);

    /*! return code */
    return(t_Initialize_Status);
}

/**
 * \fn  Std_ReturnType SrvReport_Get(void) 
 * \brief this function <TBD>
 *
 * \param  <parameter-name> { parameter description }
 *
* \return Std_ReturnType :
*         - E_OK if good return function
*         - E_NOT_OK if not
 */
Std_ReturnType SrvErr_Manage(void)
{
    /*! Declaration */
    Std_ReturnType t_Manage_Status;

    /*! Initialization */
    t_Manage_Status=E_OK;

    /*! Function Body */
    if(E_NOT_OK!=gt_InitErrorReport_Status)
    { 

    	/*! Get Scheduler Error */
    	t_Manage_Status|=SrvSched_Get_Error(&gs_SrvErrorReport_Data.e_SrvSched_error );
		SrvLog_RaiseEvent_State((uint8 *)"SrvSched_Get_Error",t_Manage_Status);

    	/*! Get Lidar Error */
    	t_Manage_Status|=SrvLidar_Get_Error(&gs_SrvErrorReport_Data.e_SrvLidar_error );
		SrvLog_RaiseEvent_State((uint8 *)"SrvLidar_Get_Error",t_Manage_Status); 

    	/*! Get Command Error */
    	t_Manage_Status|=SrvCom_Get_Error(&gs_SrvErrorReport_Data.e_SrvCom_error );
		SrvLog_RaiseEvent_State((uint8 *)"SrvCom_Get_Error",t_Manage_Status); 

    	/*! Get Configuration Error */
    	t_Manage_Status|=SrvConf_Get_Error(&gs_SrvErrorReport_Data.e_SrvConf_error );
		SrvLog_RaiseEvent_State((uint8 *)"SrvConf_Get_Error",t_Manage_Status);  
    }
    else
    {
	/* Module is not initialized, Halt */
	;
    }

    /*! return code */
    return(t_Manage_Status);
}
 

/**
 * \fn  Std_ReturnType SrvReport_Get(void)
 * \brief this function <TBD>
 *
 * \param  <parameter-name> { parameter description }
 *
* \return Std_ReturnType :
*         - E_OK if good return function
*         - E_NOT_OK if not
 */
Std_ReturnType SrvErr_Get(ts_SrvErrorReport_Data *ps_SrvErrorReport_Data)
{
    /*! Declaration */     
    Std_ReturnType t_Get_Status; 

    /*! Initialization */
    t_Get_Status=E_NOT_OK; 

    /*! Function Body */
    if((E_NOT_OK!=gt_InitErrorReport_Status)&&
    		(NULL_PTR!=ps_SrvErrorReport_Data))
    {
        memcpy(ps_SrvErrorReport_Data,&gs_SrvErrorReport_Data,sizeof(ts_SrvErrorReport_Data));
        t_Get_Status=E_OK;
    }
    else
    {
	/* Module is not initialized, Halt */
	;
    }

    /*! return code */
    return(t_Get_Status);
}
