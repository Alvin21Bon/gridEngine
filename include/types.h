#ifndef GRID_TYPES_H
#define GRID_TYPES_H

#include "shared.h"

struct CanvasPixel {
	GLboolean isSolid;
	Vec3 color;
};

struct CanvasBorder {
	GLboolean isVisible;
	Vec3 color;
	float thickness;
};

struct CoordinateCanvas {
	Vec2 origin;
	Vec2 size;
	struct gridUnitCnt {
		unsigned int x, y;
	};
	struct CanvasPixel** canvasData; // dynamically allocated 2D array based on the gridUnitCnt values
	struct CanvasBorder border;
};

#endif // GRID_TYPES_H
