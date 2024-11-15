#pragma once

#include <sys/types.h>
#include "viewports.h"

struct CoordinateCanvas;
struct ShaderProgramManager;

/*
 * This struct encapsulates any rendering or utilization of opengl from the canvas.
 * Thus, the CoordinateCanvas struct will simply handle any data of the canvas, and
 * this struct and functions handles getting the canvas to render to the screen. 
*/
struct CanvasRenderer {
	uint VBO, VAO; // EBO is the same across all canvas renderers, thus does not need to be stored here
	size_t sizeOfVertexBuffer;
	struct Viewport canvasViewport;
	struct Viewport borderViewport;
};

void canvasCreateRenderer(struct CoordinateCanvas* const canvas);
void canvasUpdateRenderer(struct CoordinateCanvas* const canvas);
void canvasDestroyRenderer(struct CoordinateCanvas* const canvas);

void canvasDraw(struct CoordinateCanvas* const canvas, struct ShaderProgramManager* const shaderProgramManager);
