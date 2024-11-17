#include "canvas/canvas-pixel.h"

struct CanvasPixel pixel(const Color color)
{
	struct CanvasPixel pixel;
	pixel.color = color;
	pixel.isVisible = true;
	return pixel;
}

void pixelToggleVisibility(struct CanvasPixel* const pixel)
{
	pixel->isVisible = !pixel->isVisible;
}
