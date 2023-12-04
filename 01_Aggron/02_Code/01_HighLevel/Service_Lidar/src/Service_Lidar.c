/*============================================================================*/
/*                        	ROBOSOFT                                      */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*/
/*! 
 * $Source: Service_Lidar.c 
 * $Author: MDega 
 * $Date: 2015/12/08  
 */
/*============================================================================*/
/* DESCRIPTION :                                                              */
/** \file  Service_Lidar.c 
  * \brief Fichier Source du Service Lidar
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
#include "Service_Lidar.h"
#include "Service_Log.h"

#include <string.h>
#include <stdlib.h>
#include <time.h>

/* Constants and types  						      */
/*============================================================================*/ 
//#define K_SRVLIDAR_DISABLE_LMS151 1

#define K_SRVLIDAR_SECONDS_MODULO 4294
#define K_SRVLIDAR_REVOLUTION_DEGRE 360
#define K_SRVLIDAR_S300_ANGLE_OFFSET -135
#define K_SRVLIDAR_S300_RANGE 270
#define K_SRVLIDAR_UST1020LX_ANGLE_OFFSET -135
#define K_SRVLIDAR_UST1020LX_RANGE 270
#define K_SRVLIDAR_LMS151_ANGLE_OFFSET -135
#define K_SRVLIDAR_LMS151_RANGE 270

/* Variables 								      */
/*============================================================================*/ 
static Std_ReturnType gt_InitLidar_Status = E_NOT_OK ; /* Global Initialization variable */
static ts_SrvLidar_configuration gs_configuration ;
static bool gb_is_configure = K_FALSE ;
static te_SrvLidar_StateMachine ge_stateMachine = E_STATE_SRVLIDAR_UNDEFINED ;
static ts_SrvLidar_data gs_outputData ;
static te_SrvLidar_error ge_SrvLidar_error=E_SRVLIDAR_ERR_NOT_INITIALIZED;

/* Private functions prototypes 					      */
/*============================================================================*/ 
static uint8 srvLidar_Calculate_angularTickPerRevolution( uint16 u16_impactCount , uint16 u16_range_degre ,
												uint16 * const pu16_angularTickPerRevolution ) ;

/* Inline functions 							      */
/*============================================================================*/

/* Private functions 							      */
/*============================================================================*/
/**
 * \fn  void srvLidar_Calculate_angularTickPerRevolution( uint16 u16_impactCount , uint16 u16_range ,
												uint16 * const pu16_angularTickPerRevolution )
 * \brief this function calculates angular Tick Per Revolution from impact count and sensor range
 *
 * \param  u16_impactCount : Number of impact
 * \param  u16_range_degre : Range of sensor in degre
 * \param  pu16_angularTickPerRevolution [out] : Angular tick per revolution
 *
* \return Std_ReturnType :
*         - E_OK if good return function
*         - E_NOT_OK if not
 */
static uint8 srvLidar_Calculate_angularTickPerRevolution( uint16 u16_impactCount , uint16 u16_range_degre ,
												uint16 * const pu16_angularTickPerRevolution )
{
	uint16 u16_impact_number_per_degre ;

	if( NULL_PTR != pu16_angularTickPerRevolution )
	{
		u16_impact_number_per_degre = (u16_impactCount - 1) / u16_range_degre ;
		*pu16_angularTickPerRevolution = K_SRVLIDAR_REVOLUTION_DEGRE * u16_impact_number_per_degre ;
	}

	return 0 ;
}

/* Public functions 							      */
/*============================================================================*/
/**
 * \fn  Std_ReturnType SrvLidar_Initialize(void)
 * \brief this function initializes all lidars
 *
 * \param  void
 *
* \return Std_ReturnType :
*         - E_OK if good return function
*         - E_NOT_OK if not
 */
Std_ReturnType SrvLidar_Initialize(void)
{
    /*! Declaration */
    Std_ReturnType t_Initialize_Status;

    /*! Initialization */
    gb_is_configure = K_FALSE ;
    ge_stateMachine = E_STATE_SRVLIDAR_UNDEFINED ;
    gs_outputData.s_S300.u32_timestamp_sec = 0 ;
    gs_outputData.s_S300.u32_timestamp_ns = 0 ;
    gs_outputData.s_ust1020lx.u32_timestamp_sec = 0 ;
    gs_outputData.s_ust1020lx.u32_timestamp_ns = 0 ;
    gs_outputData.s_lms151.u32_timestamp_sec = 0 ;
    gs_outputData.s_lms151.u32_timestamp_ns = 0 ;
    gs_outputData.s_S300.u8_sensor_status = 0 ;
	gs_outputData.s_ust1020lx.u8_sensor_status = 0 ;
	gs_outputData.s_lms151.u8_sensor_status = 0 ;

    /*! Function Body */
    ge_SrvLidar_error = E_SRVLIDAR_ERR_NO_ERROR ;

    /* Initialize all LIDAR */
    t_Initialize_Status = LibLidarLms151_Initialize() ;


    gt_InitLidar_Status=t_Initialize_Status;

    /*! return code */
    return(t_Initialize_Status);
}

/**
 * \fn  Std_ReturnType SrvLidar_Configure( const ts_SrvLidar_configuration * const ps_configuration )
 * \brief this function configures all lidars
 *
 * \param  ps_configuration : Lidars configuration
 *
* \return Std_ReturnType :
*         - E_OK if good return function
*         - E_NOT_OK if not
 */
Std_ReturnType SrvLidar_Configure( const ts_SrvLidar_configuration * const ps_configuration )
{
    /* Declaration */
    Std_ReturnType t_Configure_Status ;

    /* Initialization */
    t_Configure_Status = E_OK ;

	/* Function Body */
	if( (E_NOT_OK != gt_InitLidar_Status) && (NULL_PTR != ps_configuration) )
	{
		memcpy( &gs_configuration , ps_configuration , sizeof(ts_SrvLidar_configuration) ) ;
		gb_is_configure = K_TRUE ;

	    /* Configure all LIDAR */
		t_Configure_Status |= LibLidarLms151_Configure( &(gs_configuration.s_lms_Configuration) ) ;

		/*! Open all instruments */
    	t_Configure_Status |= LibLidarLms151_Open() ;

		if( E_NOT_OK != t_Configure_Status )
		{
			/*! IF all instruments are correctly opened, then continuous */
			ge_stateMachine = E_STATE_SRVLIDAR_READY ;
		}
		else
		{
			/*! IF at leat one instrument isn't correctly opened, then put state machine in closed mode */
			ge_stateMachine = E_STATE_SRVLIDAR_UNDEFINED ;
		}
	}
	else
	{
		/* Module is not initialized, Halt */
		t_Configure_Status = E_NOT_OK ;
	}

    /* return code */
    return(t_Configure_Status);
}

/**
 * \fn  Std_ReturnType SrvLidar_Manage(void)
 * \brief this function read data from all lidars
 *
 * \param  void
 *
* \return Std_ReturnType :
*         - E_OK if good return function
*         - E_NOT_OK if not
 */
Std_ReturnType SrvLidar_Manage(void)
{
    /*! Declaration */     
    Std_ReturnType t_Manage_Status;
	uint32 u32_time_sec ;
	uint32 u32_time_ns ;
	uint16 u16_i ;
	ts_LibLidarLms151_scan_data s_lms_data ;

    /*! Initialization */
    t_Manage_Status = E_OK ;

    /*! Function Body */
    if( (E_NOT_OK != gt_InitLidar_Status) && (K_FALSE != gb_is_configure) )
    {
    	u32_time_sec = 0 ;
    	u32_time_ns = 0 ;

    	/*! Read current time */
    	t_Manage_Status |= StdLib_GetTime( &u32_time_sec , &u32_time_ns ) ;

		switch( ge_stateMachine )
		{
			case E_STATE_SRVLIDAR_READY :
				s_lms_data.b_is_new_data = K_FALSE ;

				/*! Read all instruments */
				t_Manage_Status |=LibLidarLms151_Manage() ;

				/*! Get datas */
				t_Manage_Status |= LibLidarLms151_Get_Scan( &s_lms_data ) ;

				if( E_NOT_OK != t_Manage_Status )
				{
					/*! IF no errors , then treat datas (all lidar)*/
					if( K_FALSE != s_lms_data.b_is_new_data )
					{
						/*!   IF new LMS151 data */
						gs_outputData.s_lms151.u32_timestamp_sec	= u32_time_sec ;
						gs_outputData.s_lms151.u32_timestamp_ns	= u32_time_ns ;
						gs_outputData.s_lms151.u8_sensor_status 	= 0 ;
						srvLidar_Calculate_angularTickPerRevolution( s_lms_data.u16_dist_len1 , K_SRVLIDAR_LMS151_RANGE ,
															&(gs_outputData.s_lms151.u16_angular_tick_per_revolution) ) ;
						gs_outputData.s_lms151.s16_angle_offset 	= K_SRVLIDAR_LMS151_ANGLE_OFFSET*
								(gs_outputData.s_lms151.u16_angular_tick_per_revolution/K_SRVLIDAR_REVOLUTION_DEGRE) ;
						gs_outputData.s_lms151.u16_impact_count = s_lms_data.u16_dist_len1 ;
						/*!     Convert scan data mm -> m */
						for( u16_i = 0 ; u16_i < gs_outputData.s_lms151.u16_impact_count ; u16_i++ )
						{
							gs_outputData.s_lms151.f32_distance_list[u16_i] = s_lms_data.u16_dist1[u16_i] / 1000.0 ;
						}
					}
				}
				else
				{
					/*! IF at leat one instrument isn't correctly read, then put state machine in closed mode */
					ge_stateMachine = E_STATE_SRVLIDAR_UNDEFINED ;
				}
			break ;

			case E_STATE_SRVLIDAR_UNDEFINED :
			default :
				t_Manage_Status = E_NOT_OK ;
			break ;
		}
    }
    else
    {
		/* Module is not initialized, Halt */
        t_Manage_Status = E_NOT_OK ;
    }

    /*! return code */
    return(t_Manage_Status);
}

/**
 * \fn  Std_ReturnType SrvLidar_Get_Data( const ts_SrvLidar_data * const ps_data )
 * \brief this function returns last lidar datas
 *
 * \param ts_SrvLidar_data * const ps_data : Last lidar datas
 *
* \return Std_ReturnType :
*         - E_OK if module is intialized, configure and pe_state is not NULL
*         - E_NOT_OK if not
 */
Std_ReturnType SrvLidar_Get_Data( ts_SrvLidar_data * const ps_data )
{
    /* Declaration */
    Std_ReturnType t_Get_data_Status;

    /* Initialization */
    t_Get_data_Status = E_OK ;

    /* Function Body */
    if( (E_NOT_OK != gt_InitLidar_Status) && (K_FALSE != gb_is_configure) &&
		(NULL_PTR != ps_data))
    {
    	memcpy( ps_data , &gs_outputData , sizeof(ts_SrvLidar_data) ) ;
    }
    else
    {
    	/* Module is not initialized, Halt */
    	t_Get_data_Status = E_NOT_OK ;
    }

    /* return code */
    return(t_Get_data_Status);
}

/**
 * \fn  Std_ReturnType SrvLidar_Get_State( te_SrvLidar_StateMachine * const pe_state )
 * \brief this function returns actual state machine
 *
 * \param te_SrvLidar_StateMachine * const pe_state : actual state machine
 *
* \return Std_ReturnType :
*         - E_OK if module is intialized, configure and pe_state is not NULL
*         - E_NOT_OK if not
 */
Std_ReturnType SrvLidar_Get_State( te_SrvLidar_StateMachine * const pe_state )
{
    /* Declaration */
    Std_ReturnType t_Get_State_Status;

    /* Initialization */
    t_Get_State_Status = E_OK ;

    /* Function Body */
    if( (NULL_PTR != pe_state) )
    {
    	*pe_state = ge_stateMachine ;
    }
    else
    {
    	/* Module is not initialized, Halt */
    	t_Get_State_Status = E_NOT_OK ;
    }

    /* return code */
    return(t_Get_State_Status);
}

/**
 * \fn  Std_ReturnType SrvLidar_Close(void)
 * \brief this function closes all lidar communication
 *
 * \param void
 *
* \return Std_ReturnType :
*         - E_OK if module is intialized and configure
*         - E_NOT_OK if not
 */
Std_ReturnType SrvLidar_Close(void)
{
    /* Declaration */
    Std_ReturnType t_Get_State_Status;

    /* Initialization */
    t_Get_State_Status = E_OK ;

    /* Function Body */
    if( (E_NOT_OK != gt_InitLidar_Status) && (K_FALSE != gb_is_configure) )
    {
    	/*! Close all instruments */
    	t_Get_State_Status |= LibLidarLms151_Close() ;

    	/*! Reset module state */
    	gt_InitLidar_Status = E_NOT_OK ;
    	gb_is_configure = K_FALSE ;
    	ge_stateMachine = E_STATE_SRVLIDAR_UNDEFINED ;
    }
    else
    {
    	/* Module is not initialized, Halt */
    	t_Get_State_Status = E_NOT_OK ;
    }

    /* return code */
    return(t_Get_State_Status);
}

/**
 * \fn  Std_ReturnType SrvLidar_Get_Error(te_SrvLidar_error *pe_SrvLidar_error )
 * \brief this function will return Error Motor value
 *
 * \param  te_SrvLidar_error * const pe_SrvLidar_error
 *
* \return Std_ReturnType :
*         - E_OK if good return function
*         - E_NOT_OK if not
 */
Std_ReturnType SrvLidar_Get_Error(te_SrvLidar_error *pe_SrvLidar_error )
{
    /*! Declaration */
    Std_ReturnType t_Error_Status;

    /*! Initialization */
    t_Error_Status=E_OK;

    /*! Function Body */
    if(NULL_PTR!=pe_SrvLidar_error)
    {
    	*pe_SrvLidar_error=ge_SrvLidar_error;
    	ge_SrvLidar_error = E_SRVLIDAR_ERR_NO_ERROR ;
    }
    else
    {
		/* Module is not initialized, Halt */
    	t_Error_Status=E_NOT_OK;
    }

    /*! return code */
    return(t_Error_Status);
}
