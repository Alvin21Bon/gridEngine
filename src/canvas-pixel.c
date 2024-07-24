#include "../include/grid-engine.h"

/* 
 * === CONSTRUCTOR FUNCTIONS ===
*/
struct CanvasPixel pixel(const Vec3 color)
{
	struct CanvasPixel pixel = {GL_TRUE, color};
	return pixel;
}
struct CanvasPixel whitePixel() { return pixel(vec3(255,255,255)); }
struct CanvasPixel blackPixel() { return pixel(vec3(0,0,0)); }

void pixelToggleVisibility(struct CanvasPixel* const pixel)
{
	pixel->isVisible = !pixel->isVisible;
}
