#include "canvas/canvas-pixel.h"

#include "utility/math-util.h"

/* 
 * === CONSTRUCTOR FUNCTIONS ===
*/
struct CanvasPixel pixel(const Vec3 color)
{
	struct CanvasPixel pixel;
	pixelSetColor(&pixel, color);
	pixel.isVisible = true;
	return pixel;
}

/* 
 * === SETTING FUNCTIONS ===
*/
void pixelToggleVisibility(struct CanvasPixel* const pixel)
{
	pixel->isVisible = !pixel->isVisible;
}
void pixelSetColor(struct CanvasPixel* const pixel, const Vec3 color)
{
	Vec2 originalRange = vec2(0, 255);
	Vec2 newRange = vec2(0, 1);
	Vec3 mappedColor = vec3(MAP_RANGE(color.r, originalRange, newRange),
			 	MAP_RANGE(color.g, originalRange, newRange),
			 	MAP_RANGE(color.b, originalRange, newRange));
	pixel->color = mappedColor;
}
