#include "engine/grid-engine-options.h"

#include "utility/color.h"
#include <sys/types.h>
#include <stdio.h>

// default values
uint GRID_MAX_FPS = 60;
uint GRID_BORDER_THICKNESS_MULTIPLIER = 1;
Color GRID_CLEAR_COLOR = {.r = 0.2, .g = 0.3, .b = 0.3};
char* GRID_WINDOW_CREATION_TITLE = "My Grid Engine Project";
uint GRID_WINDOW_CREATION_WIDTH = 1000;
uint GRID_WINDOW_CREATION_HEIGHT = 1000;
enum LoggingLevels GRID_LOGGING_LEVEL = GRID_LOGGING_WARN;

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
		case GRID_OPTION_WINDOW_CREATION_TITLE:
			GRID_WINDOW_CREATION_TITLE = (char*)optionValue;
			break;
		case GRID_OPTION_WINDOW_CREATION_WIDTH:
			GRID_WINDOW_CREATION_WIDTH = *(uint*)optionValue; 
			break;
		case GRID_OPTION_WINDOW_CREATION_HEIGHT:
			GRID_WINDOW_CREATION_HEIGHT = *(uint*)optionValue; 
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
		default:
			printf("OPTION SETTING WARN: option must be of enum GridEngineOptions\n");
			return;
	}
}
