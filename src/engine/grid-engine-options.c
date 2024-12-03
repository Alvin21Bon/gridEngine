#include "engine/grid-engine-options.h"

#include "utility/color.h"
#include "utility/logging.h"
#include "utility/user-response.h"
#include "utility/file-util.h"
#include <lina/lina.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
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
				LOG(GRID_LOGGING_WARN, "option value for GRID_OPTION_LOGGING_LEVEL not of enum LoggingLevels\n");
				return;
			}
			GRID_LOGGING_LEVEL = castedLoggingLevel;
			break;
		case GRID_OPTION_LOGGING_DISABLE_FILEPATH_EXISTS_WARNING:
			GRID_LOGGING_DISABLE_FILEPATH_EXISTS_WARNING = *(bool*)optionValue;
			break;
		case GRID_OPTION_LOGGING_FILEPATH:
			char* castedLoggingFilepath = (char*)optionValue;
			const size_t MAX_FILEPATH_LENGTH = 4096;
			if (strlen(castedLoggingFilepath) > MAX_FILEPATH_LENGTH)
			{
				LOG(GRID_LOGGING_WARN, "given grid logging filepath option value is longer than the max filepath size of %d\n", MAX_FILEPATH_LENGTH);
				return;
			}

			// check for file exists so user can choose to overwrite it if warning is not disabled
			if (CHECK_FILE_EXISTS(castedLoggingFilepath)) 
			{
				if (!GRID_LOGGING_DISABLE_FILEPATH_EXISTS_WARNING)
				{
					const size_t PROMPT_EXTRA_BUFFER_SPACE = 1000;
					char prompt[MAX_FILEPATH_LENGTH + PROMPT_EXTRA_BUFFER_SPACE];
					sprintf(prompt, "%s already exists. Do you want to overwrite this file? (y/n): ", castedLoggingFilepath);

					char response = PROMPT_CHAR_RESPONSE_FROM_USER("YyNn", prompt);
					response = tolower(response);

					if (response == 'n' || response == EOF) return;
				}
			}

			bool wasFileCreatedOrCleared = CREATE_OR_CLEAR_FILE(castedLoggingFilepath);
			if (!wasFileCreatedOrCleared)
			{
				LOG(GRID_LOGGING_ERROR, "%s given as logging filepath value was unable to be created or cleared\n", castedLoggingFilepath);
				return;
			}

			GRID_LOGGING_FILEPATH = castedLoggingFilepath;
			break;
		default:
			LOG(GRID_LOGGING_WARN, "option in gridEngineChangeOption must be of enum GridEngineOptions\n");
			return;
	}
}
