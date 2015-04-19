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

int fileCopy(FILE* source, FILE* destination);



#endif
