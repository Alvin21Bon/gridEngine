#ifndef GRID_CANVAS_PIXEL_H
#define GRID_CANVAS_PIXEL_H

#include "shared.h"
#include "canvas-types.h"

/* 
 * === CONSTRUCTOR FUNCTIONS ===
*/
struct CanvasPixel pixel(const Vec3 color);

void pixelToggleVisibility(struct CanvasPixel* const pixel);

#endif // GRID_CANVAS_PIXEL_H
