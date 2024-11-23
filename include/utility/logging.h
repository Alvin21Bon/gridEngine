#pragma once

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "engine/grid-engine-options.h"
#include "engine/grid-engine-globals.h"

// Logs get sent for logs marked at the current logging level and below
enum LoggingLevels {
	GRID_LOGGING_OFF,
	GRID_LOGGING_ERROR,
	GRID_LOGGING_WARN,
	GRID_LOGGING_FULL
};

static inline void LOG(const enum LoggingLevels logLevel, const char* format, ...)
{
	if (logLevel > GRID_LOGGING_LEVEL) return;
	if (logLevel < GRID_LOGGING_OFF || logLevel > GRID_LOGGING_FULL)
	{
		LOG(GRID_LOGGING_WARN, "logLevel not of enum LoggingLevels was given to LOG\n");
		return;
	}

	const int MAX_BUFFER_SIZE_OF_LOG_TAG = 32;
	char logTag[MAX_BUFFER_SIZE_OF_LOG_TAG]; // prepend log level and frame count to log messages 
	switch (logLevel)
	{
		case GRID_LOGGING_OFF:
			LOG(GRID_LOGGING_WARN, "logLevel of GRID_LOGGING_OFF was given to LOG\n");
			return;
		case GRID_LOGGING_ERROR: 
			snprintf(logTag, MAX_BUFFER_SIZE_OF_LOG_TAG, "[ERROR] %lu: ", GRID_FRAME_COUNTER);
			break;
		case GRID_LOGGING_WARN:
			snprintf(logTag, MAX_BUFFER_SIZE_OF_LOG_TAG, "[WARN] %lu: ", GRID_FRAME_COUNTER);
			break;
		case GRID_LOGGING_FULL:
			snprintf(logTag, MAX_BUFFER_SIZE_OF_LOG_TAG, "%lu: ", GRID_FRAME_COUNTER);
			break;
	}

	// only open file if GRID_LOGGING_FILEPATH was manually set (default has a length of 0)
	FILE* fp;
	if (strlen(GRID_LOGGING_FILEPATH) == 0) fp = nullptr;
	else fp = fopen(GRID_LOGGING_FILEPATH, "w");

	// if error, print to stderr
	// if filepath is set, print to file also inlcuding the console. vprintf

	va_list args;
	va_start(args, format);
	{
		size_t bufferSizeOfGivenFormattedLog = vsnprintf(NULL, 0, format, args) + 1; // +1 for \0
		char* givenFormattedLog[bufferSizeOfGivenFormattedLog];

		// TODO: FINISH IMPLEMENTING CONCATNSTE FOMATTED LOG AND LOG TAG. DO FILE IF FILE EXISTS. 
	}
	va_end(args);
	if (fp != nullptr) fclose(fp);
}
