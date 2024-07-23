#ifndef GRID_COORDINATE_CANVAS_H
#define GRID_COORDINATE_CANVAS_H

#include "types.h"

/* 
 * === CONSTRUCTOR FUNCTIONS === 
*/
	// space is allocated for the canvasData, MEMORY MUST BE FREED
struct CoordinateCanvas canvas(const Vec2 origin, const Vec2 size, 
			       const unsigned int xUnitCnt, const unsigned int yUnitCnt);

// default empty values
struct CoordinateCanvas canvasInit();

/* 
 * === SETTING FUNCTIONS ===
*/

// TRUNCATES OR EXPANDS THE MEMORY ALLOCATED TO CANVASDATA
struct CoordinateCanvas canvasSetGrid(const struct CoordinateCanvas canvas, const unsigned int xUnitCnt, const unsigned int yUnitCnt);

struct CoordinateCanvas canvasToggleBorder(const struct CoordinateCanvas canvas);
struct CoordinateCanvas canvasMakeBorderVisible(const struct CoordinateCanvas canvas);
struct CoordinateCanvas canvasMakeBorderInvisible(const struct CoordinateCanvas canvas);

struct CoordinateCanvas canvasToggleMovable(const struct CoordinateCanvas canvas);

struct CoordinateCavnas canvasTranslate(const struct CoordinateCanvas canvas, const Vec2 translate);
struct CoordinateCanvas canvasScale(const struct CoordinateCanvas canvas, const float scalar);

// CANVAS DATA MANIPULATING FUNCTIONS
struct CoordinateCanvas canvasFillColor(const struct CoordinateCanvas canvas, const Vec3 color);
struct CoordinateCanvas canvasRowFillColor(const struct CoordinateCanvas canvas, const Vec3 color);
struct CoordinateCanvas canvasSetPixel(const struct CoordinateCanvas canvas, const struct CanvasPixel pixel);

// CANVAS DRAWING FUNCTIONS

	// window and other needed window stuff is provided by the engine
void canvasDraw(const struct CoordinateCanvas canvas); 

/* 
 * === DESTORYER FUNCTIONS ===
*/
void canvasFree(const struct CoordinateCanvas);

#endif // GRID_COORDINATE_CANVAS_H
