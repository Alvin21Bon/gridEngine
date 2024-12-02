#pragma once

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "utility/logging.h"

// NOTE: only ever checks if file exists. does not take into account other errors, such as insufficient permissions
static inline bool CHECK_FILE_EXISTS(const char* filename)
{
	FILE* fp = fopen(filename, "r");

	// must check for NULL value first, to know if errno was actually set by fopen call
	bool doesFileExist;
	doesFileExist = (fp == nullptr && errno == ENOENT) ? false : true;

	if (fp != nullptr) fclose(fp);
	return doesFileExist;
}

// @returns {false} if file was unable to be created or cleared
static inline bool CREATE_OR_CLEAR_FILE(const char* filename)
{
	FILE* fp = fopen(filename, "w");
	if (fp != nullptr)
	{
		fclose(fp);
		return true;
	}

	// here, fp == null. just log some possible error values
	switch (errno)
	{
		case ENOENT:
		case ENOTDIR:
		case EISDIR:
			LOG(GRID_LOGGING_ERROR, "%s given to CREATE_OR_CLEAR_FILE is invalid\n", filename);
			break;
		case EACCES:
			LOG(GRID_LOGGING_ERROR, "insufficient permissions to write to %s given to CREATE_OR_CLEAR_FILE\n", filename);
			break;
		default:
			LOG(GRID_LOGGING_ERROR, "%s given to CREATE_OR_CLEAR_FILE results in an error: %s\n", filename, strerror(errno));
			break;
	}

	return false;
}
