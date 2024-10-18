#pragma once

#include <sys/types.h>
#include "viewports.h"

struct CoordinateCanvas;
struct ShaderProgramManager;

/*
 * This struct encapsulates any rendering or utilization of opengl from the canvas.
 * Thus, the CoordinateCanvas struct will simply handle any data of the canvas, and
 * this struct handles getting the canvas to render to the screen. 
 *
 * These functionalities work entirely indepdentely of one another. I feel
 * like this makes the functionality limits of every struct very nicely defined. 
 * This allows for rendering data to be modified and tweaked without needing for the
 * entire canvas data to change. 
*/
struct CanvasRenderer {
	uint VBO, EBO, VAO;
	size_t sizeOfVertexData;

	struct {
		struct Viewport canvasViewport;
		struct Viewport borderViewport;
		size_t numOfInstances; // for rendering the pixels
		bool shouldBeRendered;
	} settings;
};

struct CanvasRenderer canvasRenderer(const struct CoordinateCanvas* const canvas);
void canvasRendererDestory(struct CanvasRenderer* const canvasRenderer);

void canvasRendererUpdateSettings(struct CanvasRenderer* const canvasRenderer, const struct CoordinateCanvas* const canvas);
void canvasRendererCreateGLObjects(struct CanvasRenderer* const canvasRenderer);
void canvasRendererAllocateBuffers(struct CanvasRenderer* const canvasRenderer);
void canvasRendererFillBuffers(struct CanvasRenderer* const canvasRenderer, const struct CoordinateCanvas* const canvas);

void canvasRender(const struct CanvasRenderer* const canvasRenderer, struct ShaderProgramManager* const shaderProgramManager);
