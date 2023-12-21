#ifndef _TI_REPORTS_H_
#define _TI_REPORTS_H_

#include "main_types.h"

#include <stdio.h>
#include <limits.h>


/*******************************************************************************
 * List of macros needed to generate the report.
 ******************************************************************************/

/*
 * Macro to print step details
 */
#define MACRO_PRINT_TI(...) { \
  printf("           : "); printf(__VA_ARGS__); printf("\n"); fflush(stdout); \
  fprintf(fReport, "- *"); fprintf(fReport, __VA_ARGS__); fprintf(fReport, "*\n"); \
}

#define MACRO_PRINT_TI_SUCCESS(...) { \
  printf("           : "); printf(__VA_ARGS__); printf("\n"); fflush(stdout); \
  fprintf(fReport, "- <span style=\"color:green\">"); fprintf(fReport, __VA_ARGS__); fprintf(fReport, "</span>\n"); \
}

#define MACRO_PRINT_TI_WARNING(...) { \
  printf("           : "); printf(__VA_ARGS__); printf("\n"); fflush(stdout); \
  fprintf(fReport, "- <span style=\"color:orange\">"); fprintf(fReport, __VA_ARGS__); fprintf(fReport, "</span>\n"); \
}

#define MACRO_PRINT_TI_ERROR(...) { \
  printf("           : "); printf(__VA_ARGS__); printf("\n"); fflush(stdout); \
  fprintf(fReport, "- <span style=\"color:red\">"); fprintf(fReport, __VA_ARGS__); fprintf(fReport, "</span>\n"); \
}

/*
 * Case related macros
 */
#define MACRO_START_TI(u16_num_case, sz_title_case) { \
  printf("\n- Case %d : \\************ %s ************\\ \n" , u16_num_case , sz_title_case ); \
  fflush(stdout); \
  fprintf(fReport, "\n# Case %d : %s\n" , u16_num_case , sz_title_case ); \
}

#define MACRO_ASSERT_TI(u16_num_case , u16_error_count , u16_num_error_expected ) { \
		printf("- Case %d : tested_value = %d (expected : %d) : %s\n" , u16_num_case, u16_error_count, u16_num_error_expected, (u16_num_error_expected==u16_error_count)?"SUCCESS":"FAILED"); \
    fflush( stdout ); \
    fprintf(fReport, "\n**[%s]** (Case %d) tested_value = %d (expected : %d)\n", (u16_num_error_expected==u16_error_count)?"<span style=\"color:green\">SUCCESS</span>":"<span style=\"color:red\">FAILED</span>", u16_num_case, u16_error_count, u16_num_error_expected); \
}

#define MACRO_END_TI(sz_title_case) { \
  printf( "\n%s\n" , sz_title_case ); \
  fflush( stdout ); \
  fprintf( fReport, "\n**%s**\n" , sz_title_case ); \
}

/*
 * Steps related macros
 */
#define MACRO_START_STEP_TI(u16_num_case, u16_num_step, sz_title_case) { \
  printf( "- Case %d-%d : %s\n" , u16_num_case , u16_num_step, sz_title_case ) ;  \
  fflush( stdout ) ; \
  fprintf( fReport, "#### - Case %d-%d : %s\n" , u16_num_case , u16_num_step, sz_title_case ) ;  \
}

#define MACRO_ASSERT_STEP_TI(u16_num_case, u16_num_step, u16_error_count, u16_num_error_expected) { \
  printf("- Case %d-%d : tested_value = %d (expected : %d) : %s\n" , u16_num_case, u16_num_step, u16_error_count, u16_num_error_expected, (u16_num_error_expected==u16_error_count)?"SUCCESS":"FAILED"); \
  fflush(stdout); \
  fprintf(fReport, "\n**[%s]** (Case %d-%d) tested_value = %d (expected : %d)\n", (u16_num_error_expected==u16_error_count)?"<span style=\"color:green\">SUCCESS</span>":"<span style=\"color:red\">FAILED</span>", u16_num_case , u16_num_step, u16_error_count, u16_num_error_expected); \
}

#define MACRO_SKIPPED_STEP_TI(u16_num_case, u16_num_step) { \
  printf("- Case %d-%d : SKIPPED\n", u16_num_case, u16_num_step); \
  fflush(stdout); \
  fprintf(fReport, "\n**[<span style=\"color:orange\">SKIPPED</span>]** (Case %d-%d)\n", u16_num_case, u16_num_step); \
}



extern TU16 u16_ti_cases; /*!< Current TI case. */
extern TU16 u16_ti_steps; /*!< Current TI case step. */

extern char report_name[PATH_MAX]; /*!< The report file name. */
extern FILE *fReport;     /*!< Address of the report file. */


TU8 ti_report_open(char *program_path);
TU8 ti_report_close(void);

#endif