#pragma once

#include <lina/lina.h>

struct Viewport {
	iVec2 origin;
	iVec2 size;
};

void viewportUse(const struct Viewport* const viewport);
