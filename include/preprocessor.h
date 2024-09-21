#ifndef GRID_PREPROCESSOR_H
#define GRID_PREPROCESSOR_H

#include <lina/lina.h>

#define GRID_GAME_MAX_CANVAS_AMT 64
#define GRID_GAME_MAX_FPS 60

typedef int Uniform;

static inline int MIN(int a, int b)
{
	return (a < b ? a : b);
}

// map a number from one range of numbers to another range of numbers
static inline float MAP_RANGE(float input, Vec2 originalRange, Vec2 newRange)
{
	return (input - originalRange.elements[0]) * ((newRange.elements[1] - newRange.elements[0]) / (originalRange.elements[1] - originalRange.elements[0])) + newRange.elements[0];
}

#endif // GRID_PREPROCESSOR_H
