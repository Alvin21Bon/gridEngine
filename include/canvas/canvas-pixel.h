#pragma once

#include "../utility/shared.h"
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

