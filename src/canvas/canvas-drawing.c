#include "canvas/canvas-drawing.h"

#include "canvas/coordinate-canvas.h"
#include "canvas/canvas-pixel.h"
#include "utility/color.h"
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
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

void canvasDrawLine(struct CoordinateCanvas* const canvas, const Color color, const uVec2 p0, const uVec2 p1)
{
	// Bresenham's Line Algorithm https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm

	const iVec2 delta = ivec2(abs((int)p1.x - (int)p0.x), abs((int)p1.y - (int)p0.y));
	int stepX = p1.x > p0.x ? 1 : -1;
	int stepY = p1.y > p0.y ? 1 : -1;

	bool isSlopeShallow = delta.x > delta.y;
	if (isSlopeShallow)
	{
		int d = 2 * delta.y - delta.x;
		for (int x = p0.x, y = p0.y; x != p1.x; x += stepX)
		{
			canvasSetPixel(canvas, uvec2(x, y), pixel(color));

			if (d < 0)
			{
				d += 2 * delta.y;
			}
			else
       			{
       				d += 2 * delta.y - 2 * delta.x;
       				y += stepY;
       			}
		}
	}
	else
	{
		int d = delta.y - 2 * delta.x;
		for (int y = p0.y, x = p0.x; y != p1.y; y += stepY)
		{
			canvasSetPixel(canvas, uvec2(x, y), pixel(color));

			if (d > 0)
			{
				d -= 2 * delta.x;
			}
			else
       			{
       				d += 2 * delta.y - 2 * delta.x;
       				x += stepX;
       			}
		}
	}

	canvasSetPixel(canvas, p1, pixel(color)); // this is missed in the for loops
}

