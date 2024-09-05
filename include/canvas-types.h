#ifndef GRID_CANVAS_TYPES_H
#define GRID_CANVAS_TYPES_H

#include "shared.h"

/*
 * ===
 * THIS HEADER COUPLES THESE TYPES TOGETHER SINCE THEY ARE TIGHTLY COUPLED TO THE WHOLE ENGINE 
 * LEADING TO SOME INCLUSION ERRORS 
 * ===
*/

struct CanvasBorder {
	Vec3 color;
	float thickness;
	GLboolean isVisible;
};

// RGB values for color go up to 255
struct CanvasPixel {
	Vec3 color;
	GLboolean isVisible;
};

struct CoordinateCanvas {
	char* id; // name associated for help with searching through arrays of canvases
	Vec2 origin;
	Vec2 size;

	struct {
		unsigned int x, y;
	} gridUnitCnt;
	// dynamically allocated 2D array based on the gridUnitCnt values
	// coordinate for the grid start at (0, 0)
	struct CanvasPixel** canvasData; 
	unsigned long numPixels;
	unsigned long sizeOfCanvasData;
	// points to the actual location of contigous memory of canvas data
	struct CanvasPixel* canvasDataMemoryLocation;

	struct CanvasBorder border;
	GLboolean isMovable;
	GLboolean isVisible;

	/* ALLOCATED OPENGL BUFFERS SAVED FOR DELETION. DO NOT MODIFY */
	struct {
		unsigned int VBO, EBO, VAO;
	} glBuffers;

	/* UNIFORM LOCATION FOR USE IN SHADERS. DO NOT MODIFY */
	int locationOfGridUnitCntUniform;
};

#endif // GRID_CANVAS_TYPES_H
