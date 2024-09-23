#ifndef GRID_COORDINATE_CANVAS_H
#define GRID_COORDINATE_CANVAS_H

#include "shared.h"
#include "canvas-types.h"
#include "game-state.h"

/* 
 * === CONSTRUCTOR FUNCTIONS === 
*/
	// space is allocated for the canvasData, MEMORY MUST BE FREED
struct CoordinateCanvas canvas(const uint id, const Vec2 origin, const Vec2 size, 
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

#endif // GRID_COORDINATE_CANVAS_H
//
