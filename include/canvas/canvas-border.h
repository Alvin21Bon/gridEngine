#pragma once

#include <lina/lina.h>
#include "utility/color.h"

struct CanvasBorder {
	Color color;
	float thickness;
	bool isVisible;
};

struct CanvasBorder border(const Color color, const float thickness);

void borderToggleVisibility(struct CanvasBorder* const border);

