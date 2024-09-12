#ifndef GRID_CANVAS_PIXEL_H
#define GRID_CANVAS_PIXEL_H

#include "shared.h"
#include "canvas-types.h"

/* 
 * === CONSTRUCTOR FUNCTIONS ===
*/
struct CanvasPixel pixel(const Vec3 color);

/* 
 * === SETTING FUNCTIONS ===
*/
void pixelToggleVisibility(struct CanvasPixel* const pixel);

// use this function to map colors (0,255) to (0, 1)
void pixelSetColor(struct CanvasPixel* const pixel, const Vec3 color);

#endif // GRID_CANVAS_PIXEL_H
