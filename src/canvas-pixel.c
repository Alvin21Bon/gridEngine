#include "../include/grid-engine.h"

/* 
 * === CONSTRUCTOR FUNCTIONS ===
*/
struct CanvasPixel pixel(const Vec3 color)
{
	struct CanvasPixel pixel;
	pixel.color = color;
	pixel.isVisible = GL_TRUE;
	return pixel;
}

void pixelToggleVisibility(struct CanvasPixel* const pixel)
{
	pixel->isVisible = !pixel->isVisible;
}
