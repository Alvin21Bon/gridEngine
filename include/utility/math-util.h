#pragma once

#include <lina/lina.h>

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

// maps an input vector from one grid to another grid
static inline Vec2 MAP_BOXED_RANGE(Vec2 input, Vec2 originalBotLeft, Vec2 originalTopRight, Vec2 newBotLeft, Vec2 newTopRight)
{
	return vec2(MAP_RANGE(input.x, vec2(originalBotLeft.x, originalTopRight.x), vec2(newBotLeft.x, newTopRight.x)), MAP_RANGE(input.y, vec2(originalBotLeft.y, originalTopRight.y), vec2(newBotLeft.y, newTopRight.y)));
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

