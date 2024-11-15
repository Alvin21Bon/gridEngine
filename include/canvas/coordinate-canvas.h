#pragma once

#include <lina/lina.h>
#include "canvas/canvas-border.h"
#include "canvas/canvas-pixel.h"
#include "utility/opengl/canvas-rendering.h"

struct CoordinateCanvas {
	const char* id; 
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
	size_t numPixels;

	/* TYPICALLY SHOULD NOT BE MODIFIED DIRECTLY*/
	struct CanvasRenderer renderer;
};

struct CoordinateCanvas canvas(const char* id, const Vec2 origin, const Vec2 size, const uVec2 gridUnitCnt);

// NOTE: THIS FUNCTION TRUNCATES OR EXPANDS THE MEMORY ALLOCATED TO CANVASDATA
// NOTE: This function clamps gridUnitCnt values to a minimum of 1
void canvasSetGrid(struct CoordinateCanvas* const canvas, uVec2 newGridUnitCnt);
void canvasSetSize(struct CoordinateCanvas* const canvas, const Vec2 newSize);
void canvasScale(struct CoordinateCanvas* const canvas, const float scalar);
void canvasTranslate(struct CoordinateCanvas* const canvas, const Vec2 translate);

void canvasToggleBorder(struct CoordinateCanvas* const canvas);
void canvasMakeBorderVisible(struct CoordinateCanvas* const canvas);
void canvasMakeBorderInvisible(struct CoordinateCanvas* const canvas);

Vec2 canvasGetBorderOrigin(const struct CoordinateCanvas* const canvas);
Vec2 canvasGetBorderSize(const struct CoordinateCanvas* const canvas);

void canvasFillColor(struct CoordinateCanvas* const canvas, const Vec3 color);
void canvasRowFillColor(struct CoordinateCanvas* const canvas, const uint rowNum, const Vec3 color);
void canvasSetPixel(struct CoordinateCanvas* const canvas, const uVec2 pixelCoords, const struct CanvasPixel pixel);
void canvasClear(struct CoordinateCanvas* const canvas);

void canvasDataFree(struct CoordinateCanvas* canvas);
void canvasDestroy(struct CoordinateCanvas* canvas);

