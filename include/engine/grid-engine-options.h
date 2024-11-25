#pragma once

#include <sys/types.h>
#include <lina/lina.h>
#include "utility/color.h"
#include "utility/logging.h"

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
	GRID_OPTION_LOGGING_LEVEL,
	// VALUE TYPE: bool
	GRID_OPTION_LOGGING_DISABLE_FILEPATH_EXISTS_WARNING,
	// VALUE TYPE: char*
	GRID_OPTION_LOGGING_FILEPATH,
};

extern uint GRID_MAX_FPS;
extern uint GRID_BORDER_THICKNESS_MULTIPLIER;
extern Color GRID_CLEAR_COLOR;
extern char* GRID_WINDOW_TITLE;
extern uVec2 GRID_WINDOW_SIZE;
extern enum LoggingLevels GRID_LOGGING_LEVEL;
extern bool GRID_LOGGING_DISABLE_FILEPATH_EXISTS_WARNING;
extern char* GRID_LOGGING_FILEPATH;

void gridEngineChangeOption(const enum GridEngineOptions option, const void* const optionValue);
