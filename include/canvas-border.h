#ifndef GRID_CANVAS_BORDER_H
#define GRID_CANVAS_BORDER_H

#include "shared.h"

struct CanvasBorder {
	GLboolean isVisible;
	Vec3 color;
	float thickness;
};

/* 
 * === CONSTRUCTOR FUNCTIONS ===
*/
const struct CanvasBorder border(const Vec3 color, const float thickness);

/* 
 * === SETTING FUNCTIONS ===
*/
void borderToggleVisibility(struct CanvasBorder* const border);

#endif // GRID_CANVAS_BORDER_H
