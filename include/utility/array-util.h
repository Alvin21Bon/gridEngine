#pragma once

#include "shared.h"

// NOTE: last element in array is unmodified
// @returns {bool} false on error
bool SHIFT_ARRAY_ELEMENTS_LEFT(void* const array, size_t idxToShiftAt, const size_t sizeOfElements, const size_t arrayLength)
{
	if (idxToShiftAt == 0) idxToShiftAt = 1; // seg fault if this is done on idx=0
	else if (idxToShiftAt >= arrayLength) 
	{
		printf("SHIFT_ARRAY_ELEMENTS_LEFT ERROR: shift idx outside of array range\n");
		return false;
	}

	char* const arrayFirstAddress = (char*)array;
	char* const arrayLastAddress = arrayFirstAddress + (sizeOfElements * arrayLength);
	char* const idxToShiftAtAddress = arrayFirstAddress + (sizeOfElements * idxToShiftAt);

	memmove(idxToShiftAtAddress - sizeOfElements, idxToShiftAtAddress, arrayLastAddress - idxToShiftAtAddress);
	return true;
}
