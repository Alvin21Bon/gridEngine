#include "utility/logging.h"

#include "engine/grid-engine-options.h"
#include "engine/grid-engine-globals.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

void LOG(const enum LoggingLevels logLevel, const char* format, ...)
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

	const bool wasLoggingFilepathOptionSet = strlen(GRID_LOGGING_FILEPATH) != 0;
	FILE* fp = wasLoggingFilepathOptionSet ? fopen(GRID_LOGGING_FILEPATH, "a") : nullptr;
	va_list args;
	va_start(args, format);
	{
		// NOTE: keep type as int so I can check for encoding error return value negative number
		const int bufferSizeOfGivenFormattedLog = vsnprintf(NULL, 0, format, args) + 1; // +1 for \0
		if (bufferSizeOfGivenFormattedLog <= 0) // also check equal to 0 due to the +1 added onto it
		{
			LOG(GRID_LOGGING_ERROR, "log message given to LOG causes an encoding error\n");
			goto CLEAN_UP_FUNCTION_AND_RETURN;
		}
		
		char givenFormattedLog[bufferSizeOfGivenFormattedLog];
		vsnprintf(givenFormattedLog, bufferSizeOfGivenFormattedLog, format, args); // now stores the supplied log message with formatting
		
		const size_t bufferSizeOfFinalOutputLog = bufferSizeOfGivenFormattedLog + MAX_BUFFER_SIZE_OF_LOG_TAG; // final output log is guaranteed to fit in this buffer
		char finalOutputLog[bufferSizeOfFinalOutputLog];
		strcpy(finalOutputLog, logTag);
		strcat(finalOutputLog, givenFormattedLog); // final output log is now done processing

		// now we can output the log message to the console, and, if it exists, to the logging filepath
		if (logLevel == GRID_LOGGING_ERROR)
			fprintf(stderr, "%s", finalOutputLog);
		else
			printf("%s", finalOutputLog);

		if (fp != nullptr)
			fprintf(fp, "%s", finalOutputLog);
	}
	CLEAN_UP_FUNCTION_AND_RETURN:
	va_end(args);
	if (fp != nullptr) fclose(fp);
	return;
}

