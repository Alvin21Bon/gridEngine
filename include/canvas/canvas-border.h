#pragma once

#include "../utility/utility-header.h"

struct CanvasBorder {
	Vec3 color;
	float thickness;
	bool isVisible;
};

/* 
 * === CONSTRUCTOR FUNCTIONS ===
*/
struct CanvasBorder border(const Vec3 color, const float thickness);

/* 
 * === SETTING FUNCTIONS ===
*/
void borderToggleVisibility(struct CanvasBorder* const border);

// use this function to map colors (0,255) to (0, 1)
void borderSetColor(struct CanvasBorder* const border, const Vec3 color);

