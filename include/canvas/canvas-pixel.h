#pragma once

#include <lina/lina.h>
#include "utility/color.h"

struct CanvasPixel {
	Color color;
	bool isVisible;
};

struct CanvasPixel pixel(const Color color);

void pixelToggleVisibility(struct CanvasPixel* const pixel);

