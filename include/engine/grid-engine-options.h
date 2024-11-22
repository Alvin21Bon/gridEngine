#pragma once

#include <sys/types.h>
#include "utility/color.h"
#include <lina/lina.h>

// Logs get sent for logs marked at the current logging level and below
enum LoggingLevels {
	GRID_LOGGING_OFF,
	GRID_LOGGING_ERROR,
	GRID_LOGGING_WARN,
	GRID_LOGGING_FULL
};

enum GridEngineOptions {
	// VALUE TYPE: uint
	GRID_OPTION_MAX_FPS,
	// VALUE TYPE: uint
	GRID_OPTION_BORDER_THICKNESS_MULTIPLIER,
	// VALUE TYPE: Color
	GRID_OPTION_CLEAR_COLOR,
	// VALUE TYPE: char*
	GRID_OPTION_WINDOW_TITLE,
	// VALUE TYPE: uVec2
	GRID_OPTION_WINDOW_SIZE,
	// VALUE TYPE: enum LoggingLevels
	GRID_OPTION_LOGGING_LEVEL
};

extern uint GRID_MAX_FPS;
extern uint GRID_BORDER_THICKNESS_MULTIPLIER;
extern Color GRID_CLEAR_COLOR;
extern char* GRID_WINDOW_TITLE;
extern uVec2 GRID_WINDOW_SIZE;
extern enum LoggingLevels GRID_LOGGING_LEVEL;

void gridEngineChangeOption(const enum GridEngineOptions option, const void* const optionValue);
