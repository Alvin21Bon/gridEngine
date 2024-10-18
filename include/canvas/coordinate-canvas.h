#pragma once

#include <lina/lina.h>
#include "canvas/canvas-border.h"
#include "canvas/canvas-pixel.h"
#include "utility/opengl/canvas-rendering.h"

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
		bool isVisible;
	} options;

	// coordinates for the grid start at (0, 0)
	struct CanvasPixel** pixels; 
	// points to the actual location of contigous memory of pixels
	struct CanvasPixel* addressOfPixelArray;
	size_t sizeOfPixelArray;
	size_t numPixels;

	/* TYPICALLY SHOULD NOT BE MODIFIED DIRECTLY*/
	struct CanvasRenderer renderer;
};

/* 
 * === CONSTRUCTOR FUNCTIONS === 
*/
	// space is allocated for the canvasData, MEMORY MUST BE FREED
struct CoordinateCanvas canvas(const char* id, const Vec2 origin, const Vec2 size, 
			       const unsigned int xUnitCnt, const unsigned int yUnitCnt);

/* 
 * === SETTING FUNCTIONS ===
*/

// TRUNCATES OR EXPANDS THE MEMORY ALLOCATED TO CANVASDATA
void canvasSetGrid(struct CoordinateCanvas* const canvas, const unsigned int xUnitCnt, const unsigned int yUnitCnt);
void canvasSetSize(struct CoordinateCanvas* const canvas, const Vec2 newSize);

void canvasToggleBorder(struct CoordinateCanvas* const canvas);
void canvasMakeBorderVisible(struct CoordinateCanvas* const canvas);
void canvasMakeBorderInvisible(struct CoordinateCanvas* const canvas);

void canvasTranslate(struct CoordinateCanvas* const canvas, const Vec2 translate);
void canvasScale(struct CoordinateCanvas* const canvas, const float scalar);

// CANVAS DATA MANIPULATING FUNCTIONS
void canvasFillColor(struct CoordinateCanvas* const canvas, const Vec3 color);
void canvasRowFillColor(struct CoordinateCanvas* const canvas, const int rowNum, const Vec3 color);
void canvasSetPixel(struct CoordinateCanvas* const canvas, const Vec2 pixelCoords, const struct CanvasPixel pixel);
void canvasClear(struct CoordinateCanvas* const canvas);

/*
 * === GETTING FUNCTIONS ===
*/
Vec2 canvasGetBorderOrigin(const struct CoordinateCanvas* const canvas);
Vec2 canvasGetBorderSize(const struct CoordinateCanvas* const canvas);

/*
 * === CANVAS DRAWING FUNCTION ===
*/
void canvasDraw(struct CoordinateCanvas* const canvas, struct GameState* const game); 

/* 
 * === DESTROYER FUNCTIONS ===
*/
void canvasDataFree(struct CoordinateCanvas* canvas);
void canvasDestroy(struct CoordinateCanvas* canvas);

