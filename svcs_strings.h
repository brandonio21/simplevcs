/*******************************************************************************
 * Filename: svcs_strings.h
 * Author:   Brandon Milton (brandonio21)
 *           http://brandonio21.com
 * Date:     Martch 14, 2015
 *
 * The purpose of this file is to define the strings that are used throughout
 * SVCS. This will allow for easy customization before compiling.
 ******************************************************************************/
#ifndef SVCS_STRINGS
#define SVCS_STRINGS

#define ERROR_TOO_MANY_COMMANDS "Error: svcs only accepts 1 command. \
  You have supplied %d commands\n"

#define ERROR_NO_COMMANDS "Error: You must provide svcs with a command.\n"

#define ERROR_INVALID_COMMAND "Error: %s is an invalid svcs command.\n"


#define ERROR_DIR_EXISTS "Error: .svcs directory already exists!\n"
#define ERROR_FILE_DOES_NOT_EXIST "Error: File %s doesn't exist.\n"
#define ERROR_SETUP_INVALID_INFO_FILE "Error: Could not create information file\n";

#define ERROR_WATCH_MUST_SPECIFY_FILE "Error: Must specify file to watch.\n"
#define ERROR_WATCH_COPY_FAIL "Error: Could not copy %s to watch directory.\n"
#define ERROR_WATCH_READ_FAIL "Error: Could not open %s for reading.\n"
#define ERROR_WATCH_WRITE_FAIL "Error: Could not open %s for writing.\n"
#endif
