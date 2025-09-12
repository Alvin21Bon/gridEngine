#pragma once

#include <lina/lina.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>


/*
 * ===========================================
 * LOGGING 
 * ===========================================
*/

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


/*
 * ===========================================
 * COLOR 
 * ===========================================
*/

typedef Vec3 Color;

// @param color values should be in the range [0, 255]
// @param color values get mapped to the range [0, 1]
Color color(const uint r, const uint g, const uint b);


/*
 * ===========================================
 * MATH UTILITY
 * ===========================================
*/

static inline float MIN(float a, float b)
{
	return (a < b ? a : b);
}
static inline float MAX(float a, float b)
{
	return (a > b ? a: b);
}

// map a number from one range of numbers to another range of numbers
static inline float MAP_RANGE(float input, Vec2 originalRange, Vec2 newRange)
{
	return (input - originalRange.elements[0]) * ((newRange.elements[1] - newRange.elements[0]) / (originalRange.elements[1] - originalRange.elements[0])) + newRange.elements[0];
}

static inline bool IS_IN_RANGE(float input, Vec2 range)
{
	return range.elements[0] <= input && input <= range.elements[1];
}

// bottom left and top right vecs create a box, and checks whether this box captures the input vec
static inline bool IS_IN_BOXED_RANGE(Vec2 input, Vec2 bottomLeft, Vec2 topRight)
{
	return (IS_IN_RANGE(input.x, vec2(bottomLeft.x, topRight.x)) && IS_IN_RANGE(input.y, vec2(bottomLeft.y, topRight.y)));
}


/*
 * ===========================================
 * MEMORY UTILITY
 * ===========================================
*/

// Takes a splice of memory on range [start, end) and copies it to another address
// @exception {SegmentationFault}
static inline void MEMMOVE_RANGE(void* const destination, void* const spliceRangeStart, void* const spliceRangeEnd)
{
	memmove(destination, spliceRangeStart, (char*)spliceRangeEnd - (char*)spliceRangeStart);
}

// NOTE: 2D array is zeroed out
// @returns {nullptr} if any arguments are 0
static inline void** ALLOCATE_2D_ARRAY(const size_t numColumns, const size_t numRows, const size_t sizeOfType)
{
	if (numColumns == 0 || numRows == 0 || sizeOfType == 0) return nullptr;

	void** pointerArray = malloc(numColumns * sizeof(void*));
	void* memoryFor2DArray = calloc(numColumns * numRows, sizeOfType); 

	for (size_t col = 0; col < numColumns; col++)
	{
		pointerArray[col] = (char*)memoryFor2DArray + (col * numRows * sizeOfType);
	}

	return pointerArray;
}

static inline void DESTROY_2D_ARRAY(void** array)
{
	free(array[0]);
	array[0] = nullptr;

	free(array);
	array = nullptr;
}

// allocates new 2D array grid space and copies fitting data from old 2D array. destroys the old 2D array
// @returns {nullptr} if any num arguments are 0
static inline void** REALLOCATE_2D_ARRAY(void** array, const size_t oldNumColumns, const size_t oldNumRows, const size_t newNumColumns, const size_t newNumRows, const size_t sizeOfType)
{
	if (oldNumColumns == 0 || oldNumRows == 0 || newNumColumns == 0 || newNumRows == 0 || sizeOfType == 0) 
	{
		// still destroys here in case user sets 2D array to function return value, thus losing the old 2D array
		DESTROY_2D_ARRAY(array); 
		return nullptr;
	}

	void** new2DArray = ALLOCATE_2D_ARRAY(newNumColumns, newNumRows, sizeOfType);

	size_t smallerNumColumns = MIN(oldNumColumns, newNumColumns);
	size_t smallerNumRows = MIN(oldNumRows, newNumRows);
	for (size_t newColumnIdx = 0; newColumnIdx < smallerNumColumns; newColumnIdx++)
	{
		memcpy(new2DArray[newColumnIdx], array[newColumnIdx], smallerNumRows * sizeOfType);
	}

	DESTROY_2D_ARRAY(array);

	return new2DArray;
}


/*
 * ===========================================
 * FILE UTILITY
 * ===========================================
*/

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
			LOG(GRID_LOGGING_ERROR, __func__, __LINE__, "%s is invalid\n", filename);
			break;
		case EACCES:
			LOG(GRID_LOGGING_ERROR, __func__, __LINE__, "insufficient permissions to write to %s\n", filename);
			break;
		default:
			LOG(GRID_LOGGING_ERROR, __func__, __LINE__, "%s results in an error: %s\n", filename, strerror(errno));
			break;
	}

	return false;
}


/*
 * ===========================================
 * USER RESPONSE UTILITY
 * ===========================================
*/

// this is simply a wrapper for the getline function that also prints the prompt to the screen
// @param {responsePtr} same as getline, should be freed
static inline ssize_t GET_USER_RESPONSE_WITH_PROMPT(char** responsePtr, size_t* allocatedBufferSize, const char* const prompt)
{
	ssize_t numCharsRead;

	printf("%s", prompt);
	numCharsRead = getline(responsePtr, allocatedBufferSize, stdin);
	printf("\n");

	return numCharsRead;
}

// @returns {char} the char the user responded with 
// @returns {EOF} if next getline reaches EOF
static inline char PROMPT_CHAR_RESPONSE_FROM_USER(const char* const charResponseList, const char* const prompt)
{
	char* retrievedLineBufferPtr = nullptr;
	size_t sizeOfRetrievedLineBuffer = 0;
	ssize_t numCharsRead;
	char userResponseChar;

	while ((numCharsRead = GET_USER_RESPONSE_WITH_PROMPT(&retrievedLineBufferPtr, &sizeOfRetrievedLineBuffer, prompt)) != EOF)
	{
		if (numCharsRead > 2) continue; // only 1 character + \n
		
		userResponseChar = retrievedLineBufferPtr[0];
		if (strchr(charResponseList, userResponseChar)) break;
	}
	free(retrievedLineBufferPtr);

	return numCharsRead == EOF ? EOF : userResponseChar;
}


/*
 * ===========================================
 * GRID ENGINE STATES
 * ===========================================
*/

// These values are returned from the GameState pre/post update functions, and any of the
// user defined GameObject update functions. 
// GRID_ENGINE_SUCCESS and GRID_ENGINE_ERROR signal to cease the game loop on a success or failure condition
// GRID_ENGINE_RUNNING dicates normal game behavior, and signals to progress with the game loop as normal
// GRID_ENGINE_PAUSED signals a pause, which will hault game updates until the state is changed
enum GridEngineStates {
	GRID_ENGINE_SUCCESS, 
	GRID_ENGINE_ERROR, 

	GRID_ENGINE_RUNNING,
	GRID_ENGINE_PAUSED
};


/*
 * ===========================================
 * OPAQUE GRID ENGINE 
 * ===========================================
*/

struct GridEngine;

void gridEngineAttachPreTickFunction(struct GridEngine* engine, enum GridEngineStates (*preTickFunction)(struct GridEngine* const));
void gridEngineAttachPostTickFunction(struct GridEngine* engine, enum GridEngineStates (*postTickFunction)(struct GridEngine* const));
void gridEngineAttachPausedStateFunction(struct GridEngine* const engine, enum GridEngineStates (*pausedStateFunction)(struct GridEngine* const));


/*
 * ===========================================
 * INPUT FETCHING
 * ===========================================
*/

