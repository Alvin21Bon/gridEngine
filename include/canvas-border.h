#ifndef GRID_CANVAS_BORDER_H
#define GRID_CANVAS_BORDER_H

#include "shared.h"
#include "canvas-types.h"

/* 
 * === CONSTRUCTOR FUNCTIONS ===
*/
struct CanvasBorder border(const Vec3 color, const float thickness);

/* 
 * === SETTING FUNCTIONS ===
*/
void borderToggleVisibility(struct CanvasBorder* const border);

#endif // GRID_CANVAS_BORDER_H
