#pragma once

#include "../utility/utility-header.h"

/*
 * ===
 * THIS HEADER COUPLES THESE TYPES TOGETHER SINCE THEY ARE TIGHTLY COUPLED TO THE WHOLE ENGINE 
 * LEADING TO SOME INCLUSION ERRORS 
 * ===
*/

struct CanvasBorder {
	Vec3 color;
	float thickness;
	bool isVisible;
};

// RGB values for color go up to 255
struct CanvasPixel {
	Vec3 color;
	bool isVisible;
};

struct CoordinateCanvas {
	char* id; 
	Vec2 origin;
	Vec2 size;
	uVec2 gridUnitCnt;
	float aspectRatio; 
	struct CanvasBorder border;

	struct {
		bool isMoveableWithMouse;
		bool isMoveable; // if false, overrides mouse movement
		bool isScalableWithMouse;
		bool isScalable; // if false, overrides mouse scaling
		bool shouldMaintainAspectRatio;
	} options;

	// coordinates for the grid start at (0, 0)
	struct CanvasPixel** pixels; 
	// points to the actual location of contigous memory of pixels
	struct CanvasPixel* addressOfPixelArray;

	unsigned long numPixels;
	unsigned long sizeOfCanvasData;

	GLboolean isVisible;

	/* ALLOCATED OPENGL BUFFERS SAVED FOR DELETION. DO NOT MODIFY */
	struct {
		unsigned int VBO, EBO, VAO;
	} glBuffers;
};

