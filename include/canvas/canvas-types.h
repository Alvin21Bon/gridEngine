#pragma once

#include "../utility/shared.h"

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
	uint id; // user may make enum of canvas IDs 
	Vec2 origin;
	Vec2 size;
	float aspectRatio; 

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
	GLboolean isVisible;

	GLboolean isMovableWithMouse;
	GLboolean isScalableWithMouse;
	GLboolean shouldMaintainAspectRatio;

	/* ALLOCATED OPENGL BUFFERS SAVED FOR DELETION. DO NOT MODIFY */
	struct {
		unsigned int VBO, EBO, VAO;
	} glBuffers;
};

