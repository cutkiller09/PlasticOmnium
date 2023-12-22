#include "TI_Report.h"

#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <limits.h>
#include <libgen.h>


TU16 u16_ti_cases; /*!< Current TI case. */
TU16 u16_ti_steps; /*!< Current TI case step. */

char report_name[PATH_MAX]; /*!< The report file name. */
FILE *fReport;     /*!< Address of the report file. */

/**\brief Create and open a new report.
 * \param[in] program_path path of the program, as given in main args.
 * \return true if the file was created and opened, false otherwise.
 */
TU8 ti_report_open(char *program_path)
{
  char program_name[PATH_MAX]; 
  int i;
 
  report_name[0] = '\0';
  fReport = NULL;

  strncpy(program_name, basename(program_path), sizeof(program_name));
  // Remove program extension
  for (i = 0; i < sizeof(program_name); i++)
  {
    if (program_name[i] == '\0')
      break;
    if (program_name[i] == '.')
    {
      program_name[i] = '\0';
      break;
    }
  } 

  snprintf(report_name, sizeof(report_name), "%s_Results.md",
    program_name);

  printf("Opening report: %s\n", report_name);

  fReport = fopen(report_name, "w");
  if (fReport == NULL)
  {
    printf("Failed to open report\n");
    return false;
  }

  printf("Report opened\n");

  return true;
}

/**\brief Close the report. 
 * \return true if the report was successfully close, false otherwise.
 */
TU8 ti_report_close(void)
{
  TU8 success;

  if (0 == fclose(fReport))
  {
    success = true; 
  }
  else
  {
    success = false;
  }

  return success;
}

