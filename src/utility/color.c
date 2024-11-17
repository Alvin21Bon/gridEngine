#include "utility/color.h"

#include "utility/math-util.h"

Color color(uint r, uint g, uint b)
{
	Vec2 originalRange = vec2(0, 255);
	Vec2 newRange = vec2(0, 1);
	r = MAP_RANGE(r, originalRange, newRange);
	g = MAP_RANGE(g, originalRange, newRange);
	b = MAP_RANGE(b, originalRange, newRange);

	return uvec3(r, g, b);
}
