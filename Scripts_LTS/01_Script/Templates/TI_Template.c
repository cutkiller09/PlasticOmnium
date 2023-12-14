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
#include "TI_Reports.h"

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
	t_main_Status = E_OK;
 
    /*! Function Body */
	if (!ti_report_open(argv[0]))
    {
      printf("Fatal: Failed to create new report\n");
      return -1;
    }

    /*
     * Legend
     */
    fprintf(fReport, "Legend:\n");
    fprintf(fReport, "- *Italic*: debug/traces\n");
    fprintf(fReport, "- <span style=\"color:green\">Green</span>: success message\n");
    fprintf(fReport, "- <span style=\"color:orange\">Orange</span>: warning message\n");
    fprintf(fReport, "- <span style=\"color:red\">Red</span>: error message\n");
    fprintf(fReport, "\n");

	/*************************************************************************************/
	/* CASE 0  Nominal test case														*/
	MACRO_ASSERT_TI( u16_i , "Nominal Test Case" )  

	// t_main_Status = TBD Function call to be tested ;
    MACRO_ASSERT_TI( u16_error_count , t_main_Status , E_OK );

	/*************************************************************************************/
	/* 						Tests To Be completed 										 */
	/*************************************************************************************/

	MACRO_END_TI("TI completed");

    printf("\nReport file name: %s\n", report_name);

    ti_report_close();

    /*! return code */ 
    return(t_main_Status);
} 
