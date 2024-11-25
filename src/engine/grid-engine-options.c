#include "engine/grid-engine-options.h"

#include "utility/color.h"
#include "utility/logging.h"
#include <lina/lina.h>
#include <sys/types.h>
#include <stdio.h>
#include <errno.h>
#include <ctype.h>

// default values
uint GRID_MAX_FPS = 60;
uint GRID_BORDER_THICKNESS_MULTIPLIER = 1;
Color GRID_CLEAR_COLOR = {.r = 0.2, .g = 0.3, .b = 0.3};
char* GRID_WINDOW_TITLE = "My Grid Engine Project";
uVec2 GRID_WINDOW_SIZE = {.width = 1000, .height = 1000};
enum LoggingLevels GRID_LOGGING_LEVEL = GRID_LOGGING_WARN;
bool GRID_LOGGING_DISABLE_FILEPATH_EXISTS_WARNING = false;
char* GRID_LOGGING_FILEPATH = "";

void gridEngineChangeOption(const enum GridEngineOptions option, const void* const optionValue)
{
	switch (option)
	{
		case GRID_OPTION_MAX_FPS:
			GRID_MAX_FPS = *(uint*)optionValue; 
			break;
		case GRID_OPTION_BORDER_THICKNESS_MULTIPLIER:
			GRID_BORDER_THICKNESS_MULTIPLIER = *(uint*)optionValue; 
			break;
		case GRID_OPTION_CLEAR_COLOR:
			GRID_CLEAR_COLOR = *(Color*)optionValue;
			break;
		case GRID_OPTION_WINDOW_TITLE:
			GRID_WINDOW_TITLE = (char*)optionValue;
			break;
		case GRID_OPTION_WINDOW_SIZE:
			GRID_WINDOW_SIZE = *(uVec2*)optionValue; 
			break;
		case GRID_OPTION_LOGGING_LEVEL:
			enum LoggingLevels castedLoggingLevel = *(enum LoggingLevels*)optionValue;
			if (castedLoggingLevel < GRID_LOGGING_OFF || castedLoggingLevel > GRID_LOGGING_FULL)
			{
				printf("OPTION SETTING WARN: logging level value must be of enum LoggingLevels\n"); // replace with logging function 
				return;
			}
			GRID_LOGGING_LEVEL = castedLoggingLevel;
			break;
		case GRID_OPTION_LOGGING_DISABLE_FILEPATH_EXISTS_WARNING:
			GRID_LOGGING_DISABLE_FILEPATH_EXISTS_WARNING = *(bool*)optionValue;
			break;
		case GRID_OPTION_LOGGING_FILEPATH:
			// TODO: check if file already exists, issue warning. do u wanna overwrite this file? get input "y" or "n"
			// TODO: MAX FILE PATH SIZE OF 4096 characters
			// TODO: make new option to disable the file already exists warning
			// TODO: if "n", do not set the filepath variable and go on. if "y", then go on and fopen write to the file to clear contents
			// TODO: if fopen returns an error, do some error analysis. LOG that error, do not set variable. for example, insufficient permissions. 
			// TODO: if everything is fine, then set the variable and write to the file, then close it right after. we now have a clean open file we can log to. 
			// try to fopen the filepath with fopen r. if successful, that means the file exists. issue the warning if not disbaled.
			// if NULL, that means the file does not exist, go on. try to make the file with fopen w. if NULL, check the error codes, and LOG accordingly. 
			// if successful, now we can set the variable and remember to close the file pointer. 
			FILE* fp;
			char* castedLoggingFilepath = (char*)optionValue;
			fp = fopen(castedLoggingFilepath, "r");
			if (fp != NULL) // filepath already exists
			{
				if (!GRID_LOGGING_DISABLE_FILEPATH_EXISTS_WARNING)
				{
					char retrievedChar;
					do
					{
						printf("%s already exists. Do you want to overwrite this file? (y/n): ", castedLoggingFilepath);
						retrievedChar = getchar();
						retrievedChar = tolower(retrievedChar);
					}
					while (retrievedChar != 'y' && retrievedChar != 'n');
				}
			}
			break;
		default:
			printf("OPTION SETTING WARN: option must be of enum GridEngineOptions\n");
			return;
	}
}
