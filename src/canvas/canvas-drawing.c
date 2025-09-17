#include "canvas/canvas-drawing.h"

#include "canvas/coordinate-canvas.h"
#include "canvas/canvas-pixel.h"
#include "utility/color.h"
#include <string.h>
#include <sys/types.h>
#include <lina/lina.h>

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

