#include "utility/color.h"

#include "utility/math-util.h"
#include <lina/lina.h>

Color color(const uint r, const uint g, const uint b)
{
	Vec2 originalRange = vec2(0, 255);
	Vec2 newRange = vec2(0, 1);
	float mappedR = MAP_RANGE(r, originalRange, newRange);
	float mappedG = MAP_RANGE(g, originalRange, newRange);
	float mappedB = MAP_RANGE(b, originalRange, newRange);

	return vec3(mappedR, mappedG, mappedB);
}
