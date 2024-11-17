#pragma once 

#include <lina/lina.h>
#include <sys/types.h>

typedef uVec3 Color;

// @param color values should be in the range [0, 255]
// @param color values get mapped to the range [0, 1]
Color color(uint r, uint g, uint b);
