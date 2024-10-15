#pragma once

#include "shared.h"

// Takes a splice of memory on range [start, end) and copies it to another address
// @exception {SegmentationFault}
static inline void MEMMOVE_RANGE(void* const destination, void* const spliceRangeStart, void* const spliceRangeEnd)
{
	memmove(destination, spliceRangeStart, (char*)spliceRangeEnd - (char*)spliceRangeStart);
}

