#pragma once

// Logs get sent for logs marked at the current logging level and below
enum LoggingLevels {
	GRID_LOGGING_OFF,
	GRID_LOGGING_ERROR,
	GRID_LOGGING_WARN,
	GRID_LOGGING_FULL
};

// @param funcName should always be "__func__"
// @param lineNum should always be "__LINE__"
void LOG(const enum LoggingLevels logLevel, const char* funcName, const int lineNum, const char* format, ...);
