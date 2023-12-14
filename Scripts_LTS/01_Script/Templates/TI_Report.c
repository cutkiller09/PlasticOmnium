#include "TI_Reports.h"

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
  time_t timestamp; 
  struct tm *t; 
  int i;

  u16_ti_cases = 0;
  u16_ti_steps = 0;
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

  timestamp = time(NULL); 
  t = localtime(&timestamp); 

  snprintf(report_name, sizeof(report_name), "%04u%02u%02u_%02u%02u%02u_%s_report.md",
    1900 + t->tm_year, t->tm_mon, t->tm_mday,
    t->tm_hour, t->tm_min, t->tm_sec,
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

