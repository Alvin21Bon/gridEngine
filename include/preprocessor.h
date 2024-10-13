#pragma once

#include <lina/lina.h>

#define GRID_GAME_MAX_CANVAS_AMT 64
#define GRID_GAME_MAX_GAME_OBJECTS_AMT 512
#define GRID_GAME_MAX_FPS 60

typedef int Uniform;

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

