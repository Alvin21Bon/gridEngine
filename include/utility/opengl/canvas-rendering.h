#pragma once

#include <sys/types.h>
#include "../../canvas/canvas-header.h"

/*
 * This struct encapsulates any rendering or utilization of opengl from the canvas.
 * Thus, the CoordinateCanvas struct will simply handle any data of the canvas, and
 * this struct handles getting the canvas to render to the screen
*/
struct CanvasRenderer {
	uint VBO, EBO, VAO;
	size_t sizeOfVertexData;
};

struct CanvasRenderer canvasRenderer(const struct CoordinateCanvas* const canvas);

void canvasRendererCreateGLObjects(struct CanvasRenderer* const canvasRenderer);
void canvasRendererAllocateBuffers(struct CanvasRenderer* const canvasRenderer);
void canvasRendererFillBuffers(struct CanvasRenderer* const canvasRenderer, const struct CoordinateCanvas* const canvas);
