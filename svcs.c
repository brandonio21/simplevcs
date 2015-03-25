/*******************************************************************************
 * Filename: svcs.c
 * Author:   Brandon Milton (brandonio21)
 *           http://brandonio21.com
 * Date:     March 14, 2015
 *
 * The purpose of this file is to provide an executable for the workings
 * of svcs (Simple VCS), a simple Version Control System based on the
 * diff and patch UNIX commands. Some of the workings of Simple VCS have
 * been inspired by git, the only VCS I have really used.
 ******************************************************************************/
/* TODO: Fix memory leaks with Valgrind */
#include <unistd.h> 
#include <stdio.h>  
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <libgen.h>


#include "svcs_commands.h"
#include "svcs_strings.h"
#include "svcs_identifiers.h"
#include "svcs_utils.h"

#define CONFIG_DIRECTORY ".svcs"
#define INFO_FILE CONFIG_DIRECTORY"/information.dat"
#define WATCH_DIR CONFIG_DIRECTORY"/0"

int setup(char*, char**);
int watch(char*, char**);

/* Function:   int main(int, char**)
 * Parameters: argc - The number of arguments provided
 *             argv - An array of arguments provided
 * Return:     ERROR_SUCCESS if nothing went wrong
 *             ERROR_FAILURE if something went wrong
 *
 * This method processes the command that the user typed in and delegates
 * the core functionality of SVCS to other methods. This function's main
 * purpose is to deal with any errors in what the user provided SVCS.
 */
int main(int argc, char** argv)
{
  /* First thing we need to do is check for flags. */
  int c;
  int argIndex;
  char* command = NULL;
  char* callingDirectory = NULL;

  while ((c = getopt(argc, argv, "")) != -1)
  {
    switch (c)
    {
        
    }
  }

  /* Now that flags have been checked, let's check to make sure there is
   * more than zero commands */
  /*if (argc - optind > 1)
  {
    printf(ERROR_TOO_MANY_COMMANDS, argc - optind);
    return EXIT_FAILURE;
  }
  */
  if (argc - optind == 0)
  {
    printf(ERROR_NO_COMMANDS);
    return EXIT_FAILURE;
  }

  /* We have only one command. Get the command and prepare for usage */
  command = argv[optind++];
  int result;
  char* message = NULL;
  char* currentPath = strcat(dirname(argv[0]), "/");
  if (strcmp(command, SETUP_COMMAND) == 0 ||
      strcmp(command, SETUP_SHORTHAND) == 0)
  {
    /* The setup command has been called, parse the path */
    result = setup(currentPath, &message);
  }
  else if (strcmp(command, WATCH_COMMAND) == 0 ||
           strcmp(command, WATCH_SHORTHAND) == 0)
  {
    /* The watch command has been called. Make sure that a file was provided */
    if (argc - optind == 0)
    {
      printf(ERROR_WATCH_MUST_SPECIFY_FILE);
      return EXIT_FAILURE;
    }
    result = watch(argv[optind], &message);
  }
  else
  {
    printf(ERROR_INVALID_COMMAND, command);
    return EXIT_FAILURE;
  }

  /* Check to see if any commands returned an error */
  if (result == EXIT_FAILURE)
  {
    printf(message);
    return EXIT_FAILURE;
  }
}

int setup(char* path, char** message)
{
  /* First thing we need to do is see if the config directory already
   * exists */
  char* configPath = strcat(path, CONFIG_DIRECTORY);
  struct stat configDir;
  int statResults;
  FILE* informationFile;
  time_t t = time(NULL);
  struct tm tm = *localtime(&t);

  /* Call stat on where our directory should go to see if it exists */
  statResults = stat(configPath, &configDir);
  if (statResults != -1)
  {
    *message = ERROR_DIR_EXISTS;
    return EXIT_FAILURE;
  }

  mkdir(configPath, 0700);

  /* Now that the directory has been created, create the information file that
   * contains the creation date */
  informationFile = fopen(INFO_FILE, "w");
  fprintf(informationFile, "%s:%d%d%d%d%d%d\n", CREATION_DATE, tm.tm_mon + 1,
      tm.tm_mday, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
  fclose(informationFile);
  
  return EXIT_SUCCESS;
}

int watch(char* filePath, char** message)
{
  /* First thing we need to do is see if the specified file exists */
  int statResults;
  struct stat fileInfo;
  char* destPath;
  FILE* sourceFile;
  FILE* destFile;

  /* Call stat on the file to see if it exists */
  printf(filePath);
  statResults = stat(filePath, &fileInfo);
  if (statResults == -1)
  {
    asprintf(message, ERROR_FILE_DOES_NOT_EXIST, filePath);
    return EXIT_FAILURE;
  }

  /* We are sure that the file exists. Now check to see if the watch
   * directory exists */
  statResults = stat(WATCH_DIR, NULL);
  if (statResults == -1)
  {
    /* The watch directory doesn't exist. Create it */
    mkdir(WATCH_DIR, 0700);
  }

  /* Good. The watch directory and file to watch are present. Now copy the 
   * file to watch into the watch directory */
  destPath = strcat(WATCH_DIR, basename(filePath));
  sourceFile = fopen(filePath, "r");
  destFile = fopen(destPath, "w");
  if (fileCopy(sourceFile, destFile) == EXIT_FAILURE)
  {
    /* An error occured with the copy */
    asprintf(**message, ERROR_WATCH_COPY_FAIL, sourceFile);
    return EXIT_FAILURE;
  }

  /* Now the file is copied so it is considered to be watched. */
  return EXIT_SUCCESS;
}
  
