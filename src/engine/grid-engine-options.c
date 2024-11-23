#include "engine/grid-engine-options.h"

#include "utility/color.h"
#include "utility/logging.h"
#include <lina/lina.h>
#include <sys/types.h>
#include <stdio.h>

// default values
uint GRID_MAX_FPS = 60;
uint GRID_BORDER_THICKNESS_MULTIPLIER = 1;
Color GRID_CLEAR_COLOR = {.r = 0.2, .g = 0.3, .b = 0.3};
char* GRID_WINDOW_TITLE = "My Grid Engine Project";
uVec2 GRID_WINDOW_SIZE = {.width = 1000, .height = 1000};
enum LoggingLevels GRID_LOGGING_LEVEL = GRID_LOGGING_WARN;
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
		case GRID_OPTION_LOGGING_FILEPATH:
			GRID_LOGGING_FILEPATH = (char*)optionValue;
			break;
		default:
			printf("OPTION SETTING WARN: option must be of enum GridEngineOptions\n");
			return;
	}
}
