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
	size_t sizeOfPixelArray;
	struct Viewport canvasViewport;
	struct Viewport borderViewport;
};

struct CanvasRenderer canvasRenderer(struct CoordinateCanvas* const canvas);
void canvasUpdateRenderer(struct CoordinateCanvas* const canvas);
void canvasRendererDestroy(struct CanvasRenderer* const canvasRenderer);

void canvasRendererCreateGLObjects(struct CanvasRenderer* const canvasRenderer);
void canvasUpdateRendererViewports(struct CoordinateCanvas* const canvas);
void canvasAllocateRendererVertexBuffer(struct CoordinateCanvas* const canvas);
void canvasUpdateRendererVertexBuffer(struct CoordinateCanvas* const canvas);

void canvasDraw(struct CoordinateCanvas* const canvas, struct ShaderProgramManager* const shaderProgramManager);
