#pragma once

#include <sys/types.h>
#include "utility/color.h"

// Logs get sent for logs marked at the current logging level and below
enum LoggingLevels {
	GRID_LOGGING_OFF,
	GRID_LOGGING_ERROR,
	GRID_LOGGING_WARN,
	GRID_LOGGING_FULL
};

enum GridEngineOptions {
	GRID_OPTION_MAX_FPS,
	GRID_OPTION_BORDER_THICKNESS_MULTIPLIER,
	GRID_OPTION_CLEAR_COLOR,
	GRID_OPTION_WINDOW_TITLE,
	GRID_OPTION_WINDOW_WIDTH,
	GRID_OPTION_WINDOW_HEIGHT,
	GRID_OPTION_LOGGING_LEVEL
};

extern uint GRID_MAX_FPS;
extern uint GRID_BORDER_THICKNESS_MULTIPLIER;
extern Color GRID_CLEAR_COLOR;
extern char* GRID_WINDOW_TITLE;
extern uint GRID_WINDOW_WIDTH;
extern uint GRID_WINDOW_HEIGHT;
extern enum LoggingLevels GRID_LOGGING_LEVEL;

void gridEngineChangeOption(const enum GridEngineOptions option, const void* const optionValue);
