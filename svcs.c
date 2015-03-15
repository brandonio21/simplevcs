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
#include <unistd.h> 
#include <stdio.h>  
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>


#include "svcs_commands.h"
#include "svcs_strings.h"

#define CONFIG_DIRECTORY ".svcs"

int setup(char*, char**);

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
  char* command;
  char* callingDirectory;

  while ((c = getopt(argc, argv, "")) != -1)
  {
    switch (c)
    {
        
    }
  }

  /* Now that flags have been checked, let's check to make sure there is
   * only one command */
  if (argc - optind > 1)
  {
    printf(ERROR_TOO_MANY_COMMANDS, argc - optind);
    return EXIT_FAILURE;
  }
  else if (argc - optind == 0)
  {
    printf(ERROR_NO_COMMANDS);
    return EXIT_FAILURE;
  }

  /* We have only one command. Get the command and prepare for usage */
  command = argv[optind];
  int result;
  char* message;
  if (strcmp(command, SETUP_COMMAND) == 0 ||
      strcmp(command, SETUP_SHORTHAND) == 0)
  {
    /* The setup command has been called, parse the path */
    callingDirectory = strrchr(argv[0], '/');
    callingDirectory = strndup(argv[0], 
        strlen(argv[0]) - strlen(callingDirectory) + 1);
    result = setup(callingDirectory, &message);
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

  /* Call stat on where our directory should go to see if it exists */
  statResults = stat(configPath, &configDir);
  if (statResults != -1)
  {
    *message = ERROR_DIR_EXISTS;
    return EXIT_FAILURE;
  }

  mkdir(configPath, 0700);
  return EXIT_SUCCESS;
}