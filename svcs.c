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
/* TODO: Calling "add" on a file without first calling "setup" should give an
 *       error message telling the user to call "setup" */
/* TODO: Calling "watch" on a file without first calling "setup" should give
 *       an error message telling the user to call "setup" */

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
#define WATCH_DIR CONFIG_DIRECTORY"/0/"
#define STAGE_DIR CONFIG_DIRECTORY"/stage/"

int setup(const char*, char**);
int watch(const char*, char**);
int add(const char*, char**);

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
  else if (strcmp(command, ADD_COMMAND) == 0 ||
           strcmp(command, ADD_SHORTHAND) == 0)
  {
    /* The add command has been called. Make sure that a file was provided */
    if (argc - optind == 0)
    {
      printf(ERROR_ADD_MUST_SPECIFY_FILE);
      return EXIT_FAILURE;
    }
    result = add(argv[optind], &message);
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
  return EXIT_SUCCESS;
}

/* Function:   setup(char*, char**)
 * Parameters: path - The path of the directory to initialize as a svcs dir
 *             message - A pointer to a string that will contain error msgs
 * Return:     EXIT_SUCCESS - No error occured
 *             EXIT_FAILURE - Error(s) occurred and message is set
 * This function sets up the SVCS directory in the path specified by creating
 * the directory (If it doesn't exist) and creating an information file that
 * is filled with information pertaining to the SVCS directory.
 */
int setup(const char* path, char** message)
{
  /* First thing we need to do is see if the config directory already
   * exists */
  char configPath[BUFSIZ];
  strcpy(configPath, path);
  strcat(configPath, CONFIG_DIRECTORY);
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
  /* Make sure that it was opened properly */
  if (informationFile == NULL)
  {
    *message = ERROR_SETUP_INVALID_INFO_FILE;
    return EXIT_FAILURE;
  }

  fprintf(informationFile, "%s:%d%d%d%d%d%d\n", CREATION_DATE, tm.tm_mon + 1,
      tm.tm_mday, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
  fclose(informationFile);
  
  return EXIT_SUCCESS;
}

/* Function:   watch(char*, char**)
 * Parameters: filePath - The path to the file to watch
 *             message -  A pointer to a string that will contain error msgs
 * Return:     EXIT_SUCCESS - No error occured
 *             EXIT_FAILURE - Error(s) occurred and message is set
 * This function creates the watch directory (If it doesn't exist) and then
 * makes a copy of the specified file in the watch directory. This will allow
 * us to diff the contents of the file in the watch directory with files that
 * are being changed. 
 */
int watch(const char* filePath, char** message)
{
  /* First thing we need to do is see if the specified file exists */
  int statResults;
  struct stat fileInfo;
  char destPath[BUFSIZ];
  FILE* sourceFile;
  FILE* destFile;
  char watchFilePath[BUFSIZ];

  /* Call stat on the file to see if it exists */
  statResults = stat(filePath, &fileInfo);
  if (statResults == -1)
  {
    asprintf(message, ERROR_FILE_DOES_NOT_EXIST, filePath);
    return EXIT_FAILURE;
  }

  /* We are sure that the file exists. Now check to see if the watch
   * directory exists */
  statResults = stat(WATCH_DIR, &fileInfo);
  if (statResults == -1)
  {
    /* The watch directory doesn't exist. Create it */
    mkdir(WATCH_DIR, 0700);
  }

  /* Good. The watch directory and file to watch are present. Now copy the 
   * file to watch into the watch directory */
  strcpy(watchFilePath, filePath);
  strcpy(destPath, WATCH_DIR);
  strcat(destPath, watchFilePath);
  sourceFile = fopen(filePath, "r");
  destFile = fopen(destPath, "w");
  if (sourceFile == NULL)
  {
    asprintf(message, ERROR_WATCH_READ_FAIL, filePath);
    return EXIT_FAILURE;
  }
  if (destFile == NULL)
  {
    asprintf(message, ERROR_WATCH_WRITE_FAIL, filePath);
    return EXIT_FAILURE;
  }
  if (fileCopy(sourceFile, destFile) == EXIT_FAILURE)
  {
    /* An error occured with the copy */
    asprintf(message, ERROR_WATCH_COPY_FAIL, filePath);
    return EXIT_FAILURE;
  }

  /* Now the file is copied so it is considered to be watched. */
  return EXIT_SUCCESS;
}

int add(const char* filePath, char** message)
{
  int statResults;
  struct stat fileInformation;
  char stageFilename[BUFSIZ];
  char workingFilePath[BUFSIZ];

  /* First, make sure that the file exists */
  statResults = stat(filePath, &fileInformation);
  if (statResults == -1)
  {
    asprintf(message, ERROR_FILE_DOES_NOT_EXIST, filePath);
    return EXIT_FAILURE;
  }

  /* Now make sure that the stage directory exists */
  statResults = stat(STAGE_DIR, &fileInformation);
  if (statResults == -1)
    mkdir(STAGE_DIR, 0700);

  /* The stage directory and file to stage are present. Make the link */
  strcpy(workingFilePath, filePath);
  strcpy(stageFilename, STAGE_DIR);
  strcat(stageFilename, basename(workingFilePath));
  printf("checking %s\n", workingFilePath);
  printf("Copying %s\n", filePath);
  printf("To %s\n", stageFilename);
  if (link(filePath, stageFilename) == -1)
  {
    asprintf(message, ERROR_ADD_LINK_FAILED, filePath);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
