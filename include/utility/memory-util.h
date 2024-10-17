#pragma once

#include <string.h>
#include <stdlib.h>

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

	for (int col = 0; col < numColumns; col++)
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
