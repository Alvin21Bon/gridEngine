#include "canvas/coordinate-canvas.h"

#include "canvas/canvas-pixel.h"
#include "canvas/canvas-border.h"
#include "utility/opengl/canvas-rendering.h"
#include "utility/math-util.h"
#include "utility/memory-util.h"
#include "utility/constants.h"
#include "utility/color.h"
#include <math.h>
#include <lina/lina.h>
#include <sys/types.h>
#include <string.h>

struct CoordinateCanvas canvas(const char* id, const Vec2 origin, const Vec2 size, const uVec2 gridUnitCnt)
{
	struct CoordinateCanvas canvas;

	canvas.id = id;
	canvas.origin = origin;
	canvasSetSize(&canvas, size); // to abide by min canvas dimensions
	// gridUnitCnt is set down below
	canvas.aspectRatio = canvas.size.width / canvas.size.height;
	canvas.border = border(color(255, 255, 255), 1);

	canvas.options.isMoveableWithMouse = true;
	canvas.options.isMoveable = true;
	canvas.options.isScalableWithMouse = true;
	canvas.options.isScalable = true;
	canvas.options.shouldMaintainAspectRatio = false;
	canvas.options.isVisible = true;

	canvas.pixels = nullptr; // set to express to canvasSetGrid function that a pixel array has never been allocated yet
	canvasSetGrid(&canvas, gridUnitCnt); // to avoid gridUnitCnts values of 0

	canvasCreateRenderer(&canvas);

	return canvas;
}

void canvasSetGrid(struct CoordinateCanvas* const canvas, uVec2 newGridUnitCnt)
{
	// avoid gridUnitCnts of 0 to avoid pixel array being set to nullptr
	const uint MIN_GRID_UNIT_CNT = 1;
	newGridUnitCnt = uvec2(MAX(MIN_GRID_UNIT_CNT, newGridUnitCnt.x), MAX(MIN_GRID_UNIT_CNT, newGridUnitCnt.y));
	
	if (canvas->pixels == nullptr) // pixel array is set to nullptr before this function call in the constructor
		canvas->pixels = (struct CanvasPixel**)ALLOCATE_2D_ARRAY(newGridUnitCnt.x, newGridUnitCnt.y, sizeof(struct CanvasPixel));
	else
		canvas->pixels = (struct CanvasPixel**)REALLOCATE_2D_ARRAY((void**)canvas->pixels, canvas->gridUnitCnt.x, canvas->gridUnitCnt.y, newGridUnitCnt.x, newGridUnitCnt.y, sizeof(struct CanvasPixel));
	canvas->addressOfPixelArray = canvas->pixels[0];

	canvas->gridUnitCnt = newGridUnitCnt;
	canvas->numPixels = newGridUnitCnt.x * newGridUnitCnt.y;
}
void canvasSetSize(struct CoordinateCanvas* const canvas, const Vec2 newSize)
{
	const uint MIN_CANVAS_DIMENSION = 10;
	if (!canvas->options.shouldMaintainAspectRatio)
	{
		canvas->size = vec2(MAX(MIN_CANVAS_DIMENSION, newSize.width), MAX(MIN_CANVAS_DIMENSION, newSize.height));
		canvas->aspectRatio = canvas->size.width / canvas->size.height;
		return;
	}

	Vec2 deltaSize = vec2Sub(newSize, canvas->size);
	Vec2 newSizeWithAspectRatio;
	// the one that changed the most should take precedent in how maintaining aspect ratio is handled
	if (fabsf(deltaSize.width) > fabsf(deltaSize.height)) 
		newSizeWithAspectRatio = vec2(newSize.width, newSize.width / canvas->aspectRatio);
	else 
		newSizeWithAspectRatio = vec2(newSize.height * canvas->aspectRatio, newSize.height);

	if (newSizeWithAspectRatio.width < MIN_CANVAS_DIMENSION || newSizeWithAspectRatio.height < MIN_CANVAS_DIMENSION)
		newSizeWithAspectRatio = canvas->aspectRatio > 1 ? vec2(MIN_CANVAS_DIMENSION * canvas->aspectRatio, MIN_CANVAS_DIMENSION) : vec2(MIN_CANVAS_DIMENSION, MIN_CANVAS_DIMENSION / canvas->aspectRatio);

	canvas->size = newSizeWithAspectRatio;
}
void canvasScale(struct CoordinateCanvas* const canvas, const float scalar)
{
	canvasSetSize(canvas, vec2Scaled(canvas->size, scalar));
}
void canvasTranslate(struct CoordinateCanvas* const canvas, const Vec2 translate)
{
	canvas->origin = vec2Add(canvas->origin, translate);
}

void canvasToggleBorder(struct CoordinateCanvas* const canvas)
{
	borderToggleVisibility(&canvas->border);
}
void canvasMakeBorderVisible(struct CoordinateCanvas* const canvas)
{
	canvas->border.isVisible = true;
}
void canvasMakeBorderInvisible(struct CoordinateCanvas* const canvas)
{
	canvas->border.isVisible = false;
}

Vec2 canvasGetBorderOrigin(const struct CoordinateCanvas* const canvas)
{
	float borderThicknessInPixels = canvas->border.thickness * GRID_BORDER_THICKNESS_MULTIPLIER;
	return vec2Sub(canvas->origin, vec2Fill(borderThicknessInPixels));
}
Vec2 canvasGetBorderSize(const struct CoordinateCanvas* const canvas)
{
	float borderThicknessInPixels = canvas->border.thickness * GRID_BORDER_THICKNESS_MULTIPLIER;
	return vec2Add(canvas->size, vec2Fill(2 * borderThicknessInPixels));
}

void canvasFillColor(struct CoordinateCanvas* const canvas, const Color color)
{
	struct CanvasPixel fillPixel = pixel(color);

	// traverse through grid linearely since it is contiguous memory
	for (int idx = 0; idx < canvas->numPixels; idx++)
	{
		canvas->addressOfPixelArray[idx] = fillPixel;
	}
}
void canvasRowFillColor(struct CoordinateCanvas* const canvas, const uint rowNum, const Color color)
{
	if (rowNum >= canvas->gridUnitCnt.y) return;

	struct CanvasPixel fillPixel = pixel(color);
	for (int columnIdx = 0; columnIdx < canvas->gridUnitCnt.x; columnIdx++)
	{
		canvas->pixels[columnIdx][rowNum] = fillPixel;
	}
}
void canvasSetPixel(struct CoordinateCanvas* const canvas, const uVec2 pixelCoords, const struct CanvasPixel pixel)
{
	if (pixelCoords.x >= canvas->gridUnitCnt.x || pixelCoords.y >= canvas->gridUnitCnt.y) return;

	canvas->pixels[pixelCoords.x][pixelCoords.y] = pixel;
}
void canvasClear(struct CoordinateCanvas* const canvas)
{
	memset(canvas->addressOfPixelArray, 0, canvas->numPixels * sizeof(struct CanvasPixel));
}

void canvasDestroy(struct CoordinateCanvas* canvas)
{
	DESTROY_2D_ARRAY((void**)canvas->pixels);
	canvasDestroyRenderer(canvas);
}

