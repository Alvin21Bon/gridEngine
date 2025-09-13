#include "utility/logging.h"

#include "engine/grid-engine-options.h"
#include "engine/grid-engine-globals.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

void LOG(const enum LoggingLevels logLevel, const char* funcName, const int lineNum, const char* format, ...)
{
	if (logLevel > GRID_LOGGING_LEVEL) return;
	if (logLevel < GRID_LOGGING_OFF || logLevel > GRID_LOGGING_FULL)
	{
		LOG(GRID_LOGGING_WARN, __func__, __LINE__, "logLevel not of enum LoggingLevels was given to LOG\n");
		return;
	}

	const int MAX_BUFFER_SIZE_OF_LOG_TAG = strlen(funcName) + 64;
	char logTag[MAX_BUFFER_SIZE_OF_LOG_TAG]; // prepend log level, funcName, lineCount, and frame count to log messages 
	switch (logLevel)
	{
		case GRID_LOGGING_OFF:
			LOG(GRID_LOGGING_WARN, __func__, __LINE__, "GRID_LOGGING_OFF should not pass logging level checks\n");
			return;
		case GRID_LOGGING_ERROR: 
			snprintf(logTag, MAX_BUFFER_SIZE_OF_LOG_TAG, "[ERROR] %lu %s@%d: ", GRID_FRAME_COUNTER, funcName, lineNum);
			break;
		case GRID_LOGGING_WARN:
			snprintf(logTag, MAX_BUFFER_SIZE_OF_LOG_TAG, "[WARN] %lu %s@%d: ", GRID_FRAME_COUNTER, funcName, lineNum);
			break;
		case GRID_LOGGING_FULL:
			snprintf(logTag, MAX_BUFFER_SIZE_OF_LOG_TAG, "%lu: ", GRID_FRAME_COUNTER); // no need to tag funcName of lineNum
			break;
	}

	va_list args;
	va_start(args, format);
	
	// NOTE: keep type as int so I can check for encoding error return value negative number
	const int bufferSizeOfGivenFormattedLog = vsnprintf(NULL, 0, format, args) + 1; // +1 for \0
	va_end(args); 
	if (bufferSizeOfGivenFormattedLog <= 0) // also check equal to 0 due to the +1 added onto it
	{
		LOG(GRID_LOGGING_ERROR, __func__, __LINE__, "encoding error\n");
		return;
	}

	// NOTE: vsnprintf consumes args, thus we must end and start it again
	va_start(args, format);
	char givenFormattedLog[bufferSizeOfGivenFormattedLog];
	vsnprintf(givenFormattedLog, bufferSizeOfGivenFormattedLog, format, args); // now stores the supplied log message with formatting
	va_end(args);
	
	const size_t bufferSizeOfFinalOutputLog = bufferSizeOfGivenFormattedLog + MAX_BUFFER_SIZE_OF_LOG_TAG; // final output log is guaranteed to fit in this buffer
	char finalOutputLog[bufferSizeOfFinalOutputLog];
	strcpy(finalOutputLog, logTag);
	strcat(finalOutputLog, givenFormattedLog); // final output log is now done processing

	// now we can output the log message to the console, and, if it exists, to the logging filepath
	const bool wasLoggingFilepathOptionSet = strlen(GRID_LOGGING_FILEPATH) != 0;
	FILE* fp = wasLoggingFilepathOptionSet ? fopen(GRID_LOGGING_FILEPATH, "a") : nullptr;

	if (logLevel == GRID_LOGGING_ERROR) fprintf(stderr, "%s", finalOutputLog);
	else printf("%s", finalOutputLog);
		
	if (fp != nullptr)
	{
		fprintf(fp, "%s", finalOutputLog);
		fclose(fp);
	}
	
	return;
}

