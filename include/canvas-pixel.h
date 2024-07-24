#ifndef GRID_CANVAS_PIXEL_H
#define GRID_CANVAS_PIXEL_H

#include "shared.h"

struct CanvasPixel {
	GLboolean isVisible;
	Vec3 color;
};

/* 
 * === CONSTRUCTOR FUNCTIONS ===
*/
struct CanvasPixel pixel(const Vec3 color);
struct CanvasPixel whitePixel();
struct CanvasPixel blackPixel();

void pixelToggleVisibility(struct CanvasPixel* const pixel);

#endif // GRID_CANVAS_PIXEL_H
