/*============================================================================*/
/*                        	Plastic Omnium	                                  */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*/
/*! 
 * $Source: TI_$COMPONENT.c 
 * $Author:  
 * $Date:  
 */
/*============================================================================*/
/* DESCRIPTION :                                                              */
/** \file  TI_$COMPONENT.c 
  * \brief Source file  
*/
/*============================================================================*/
/* COPYRIGHT (C) Plastic Omnium	                                              */ 
/* ALL RIGHTS RESERVED                                                        */
/*                                                                            */
/* The reproduction, transmission, or use of this document or its content is  */
/* not permitted without express written authority. Offenders will be liable  */
/* for damages.                                                               */
/* All rights, including rights created by patent grant or registration of a  */
/* utility model or design, are reserved.                                     */
/*                                                                            */
/*============================================================================*/ 


#define MACRO_START_UT(u16_num_case, sz_title_case) printf( "\nCase %d : \\************ %s ************\\ \n" , u16_num_case , sz_title_case ) ;
#define MACRO_ASSERT_UT(u16_num_case , u16_error_count , u16_num_error_expected ) \
		printf( "Case %d : tested_value = %d (expected : %d) : %s\n" , \
				u16_num_case , \
				u16_error_count , \
				u16_num_error_expected , \
				(u16_num_error_expected==u16_error_count)?"SUCCESS":"FAILED" ) ;

/* Includes 								      */
/*============================================================================*/  
#include "$COMPONENT.h" 

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
 * \fn  Std_ReturnType main(void)  
 * \brief this function is the entry point of the integration test
 *
 * \param  
 *
* \return Std_ReturnType :
*         - E_OK if good return function
*         - E_NOT_OK if not
 */
int main(void)
{
    /*! Declaration */     
    Std_ReturnType t_main_Status;  

    /*! Initialization */
 
    /*! Function Body */

	/***************************************************************************************/
	/* CASE 1 : Test call all functions if service not initialized */
	MACRO_START_UT( u16_i , "Test call all functions if service not initialized" )  

    /*! return code */ 
    return(t_main_Status);
} 
