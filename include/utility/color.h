#pragma once 

#include <lina/lina.h>
#include <sys/types.h>

typedef Vec3 Color;

// @param color values should be in the range [0, 255]
// @param color values get mapped to the range [0, 1]
Color color(const uint r, const uint g, const uint b);
