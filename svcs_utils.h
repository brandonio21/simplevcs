/*******************************************************************************
 * Filename: svcs_utils.h
 * Author:   Brandon Milton (brandonio21)
 *           http://brandonio21.com
 * Date:     March 25, 2015
 *
 * The purpose of this file is to provie functionality that is needed for svcs
 * to operate, but is considered to be utility functionality, such as file 
 * copying, etc.
 ******************************************************************************/
#ifndef SVCS_UTILS
#define SVCS_UTILS
#include <stdio.h>
#include <stdlib.h>

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



#endif
