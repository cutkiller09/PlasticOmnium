/*============================================================================*/
/*                        	ROBOSOFT                                      */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*/
/*! 
 * $Source: Service_Autotests.c 
 * $Author: Slo 
 * $Date: 2015/12/08  
 */
/*============================================================================*/
/* DESCRIPTION :                                                              */
/** \file  Service_Autotests.c 
  * \brief Fichier Source du Service Autotests
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
#include "Service_Autotests.h" 

/* Constants and types  						      */
/*============================================================================*/ 
   
/* Variables 								      */
/*============================================================================*/ 
static Std_ReturnType gt_InitAutotests_Status=E_NOT_OK; /* Global Initialization variable */

/* Private functions prototypes 					      */
/*============================================================================*/ 

/* Inline functions 							      */
/*============================================================================*/

/* Private functions 							      */
/*============================================================================*/

/* Public functions 							      */
/*============================================================================*/


/**
 * \fn  Std_ReturnType SrvAut_Initialize(void) 
 * \brief this function <TBD>
 *
 * \param  <parameter-name> { parameter description }
 *
* \return Std_ReturnType :
*         - E_OK if good return function
*         - E_NOT_OK if not
 */
Std_ReturnType SrvAut_Initialize(void) 
{
    /*! Declaration */     
    Std_ReturnType t_Initialize_Status; 

    /*! Initialization */
    t_Initialize_Status=E_OK;

    /*! Function Body */
    gt_InitAutotests_Status=E_OK;
    t_Initialize_Status=gt_InitAutotests_Status;

    /*! return code */
    return(t_Initialize_Status);
}

/**
 * \fn  Std_ReturnType SrvAut_Pbit(void)
 * \brief this function <TBD>
 *
 * \param  <parameter-name> { parameter description }
 *
* \return Std_ReturnType :
*         - E_OK if good return function
*         - E_NOT_OK if not
 */
Std_ReturnType SrvAut_Pbit(void)
{
    /*! Declaration */
    Std_ReturnType t_Launch_Status;

    /*! Initialization */
    t_Launch_Status=E_OK;

    /*! Function Body */
    if(E_NOT_OK!=gt_InitAutotests_Status)
    {
		;
    }
    else
    {
    	/* Module is not initialized, Halt */
        t_Launch_Status=E_NOT_OK;
    }

    /*! return code */
    return(t_Launch_Status);
}

/**
 * \fn  Std_ReturnType SrvAut_Cbit(void)
 * \brief this function <TBD>
 *
 * \param  <parameter-name> { parameter description }
 *
* \return Std_ReturnType :
*         - E_OK if good return function
*         - E_NOT_OK if not
 */
Std_ReturnType SrvAut_Cbit(void)
{
    /*! Declaration */     
    Std_ReturnType t_Launch_Status; 

    /*! Initialization */
    t_Launch_Status=E_OK;

    /*! Function Body */
    if(E_NOT_OK!=gt_InitAutotests_Status)
    {
        ;
    }
    else
    {
    	/* Module is not initialized, Halt */
        t_Launch_Status=E_NOT_OK;
    }

    /*! return code */
    return(t_Launch_Status);
}

/**
 * \fn  Std_ReturnType SrvAut_Get_State(void) 
 * \brief this function <TBD>
 *
 * \param  <parameter-name> { parameter description }
 *
* \return Std_ReturnType :
*         - E_OK if good return function
*         - E_NOT_OK if not
 */
Std_ReturnType SrvAut_Get_State(void) 
{
    /*! Declaration */     
    Std_ReturnType t_Get_State_Status; 

    /*! Initialization */
    t_Get_State_Status=E_OK;

    /*! Function Body */
    if(E_NOT_OK!=gt_InitAutotests_Status)
    {
        ;
    }
    else
    {
	/* Module is not initialized, Halt */
	;
    }

    /*! return code */
    return(t_Get_State_Status);
}
