#pragma once

#include <string.h>
#include <stdlib.h>
#include "utility/math-util.h"

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
		pointerArray[col] = memoryFor2DArray + (col * numRows * sizeOfType);
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

