/*******************************************************************************
 * Filename: svcs_utils.c
 * Author:   Brandon Milton (brandonio21)
 *           http://brandonio21.com
 * Date:     April 19, 2015
 *
 * This file contains all implementations for headers that are decalred in
 * svcs_utls.h. In rather poor style, these implementations were originally
 * declared in the header file.
 ******************************************************************************/

#include "svcs_utils.h"

int fileCopy(FILE* source, FILE* destination)
{
  /* First create the buffer */
  char buffer[BUFSIZ];
  size_t n; /* Keeps track of the amount read and written */

  while ((n = fread(buffer, sizeof(char), sizeof(buffer), source)) > 0)
  {
    if (fwrite(buffer, sizeof(char), n, destination) != n)
      return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
