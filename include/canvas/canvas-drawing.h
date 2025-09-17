#pragma once

#include "utility/color.h"
#include "canvas/canvas-pixel.h"
#include <sys/types.h>
#include <lina/lina.h>

struct CoordinateCanvas;

void canvasFillColor(struct CoordinateCanvas* const canvas, const Color color);
void canvasRowFillColor(struct CoordinateCanvas* const canvas, const uint rowNum, const Color color);
void canvasSetPixel(struct CoordinateCanvas* const canvas, const uVec2 pixelCoords, const struct CanvasPixel pixel);
void canvasClear(struct CoordinateCanvas* const canvas);

