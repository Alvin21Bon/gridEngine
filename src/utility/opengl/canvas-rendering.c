#include "utility/opengl/canvas-rendering.h"

#include "canvas/coordinate-canvas.h"
#include "canvas/canvas-pixel.h"
#include "utility/opengl/glad.h"
#include "utility/opengl/shader-programs.h"
#include "utility/opengl/viewports.h"
#include <stddef.h>

// NOTE: these static functions are all used to either create the renderer in the constructor, or to update 
// 	 specific values in the renderer in the update function.

static void canvasRendererCreateGLObjects(struct CanvasRenderer* const canvasRenderer);
static void canvasRendererVertexSpecification(struct CanvasRenderer* const canvasRenderer);
static void canvasRendererAllocateVertexBuffer(struct CanvasRenderer* const canvasRenderer);
static void canvasUpdateRendererViewports(struct CoordinateCanvas* const canvas);
static void canvasUpdateRendererVertexBuffer(struct CoordinateCanvas* const canvas);


void canvasCreateRenderer(struct CoordinateCanvas* const canvas)
{
	struct CanvasRenderer* renderer = &canvas->renderer;
	renderer->sizeOfVertexBuffer = 0; // initially set to zero so update function can correctly allocate the VBO 
	
	canvasRendererCreateGLObjects(renderer);
	canvasRendererVertexSpecification(renderer);

	canvasUpdateRenderer(canvas);
}
void canvasUpdateRenderer(struct CoordinateCanvas* const canvas)
{
	size_t newSizeOfVertexBuffer = canvas->numPixels * sizeof(struct CanvasPixel);
	if (canvas->renderer.sizeOfVertexBuffer != newSizeOfVertexBuffer)
	{
		canvas->renderer.sizeOfVertexBuffer = newSizeOfVertexBuffer;
		canvasRendererAllocateVertexBuffer(&canvas->renderer);
	}

	canvasUpdateRendererVertexBuffer(canvas);
	canvasUpdateRendererViewports(canvas);
}
void canvasDestroyRenderer(struct CoordinateCanvas* const canvas)
{
	glDeleteBuffers(1, &canvas->renderer.VBO);
	glDeleteVertexArrays(1, &canvas->renderer.VAO);
}
void canvasDraw(struct CoordinateCanvas* const canvas, struct ShaderProgramManager* const shaderProgramManager)
{
	// skip entire graphics pipeline if canvas is not visible
	if (!canvas->options.isVisible) return;

	// setting up renderer and uniforms before drawing
	canvasUpdateRenderer(canvas);
	shaderProgramManagerSetCanvasUniforms(shaderProgramManager, canvas);

	glBindVertexArray(canvas->renderer.VAO);
	{
		shaderProgramManagerUseProgram(shaderProgramManager, shaderProgramManager->programs.canvas);
		{
			viewportUse(&canvas->renderer.canvasViewport);
			glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, canvas->numPixels);
		}

		if (canvas->border.isVisible)
		{
			shaderProgramManagerUseProgram(shaderProgramManager, shaderProgramManager->programs.border);
			{
				viewportUse(&canvas->renderer.borderViewport);
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			}
		}
	}
}


static void canvasRendererCreateGLObjects(struct CanvasRenderer* const canvasRenderer);
static void canvasRendererVertexSpecification(struct CanvasRenderer* const canvasRenderer);
static void canvasRendererAllocateVertexBuffer(struct CanvasRenderer* const canvasRenderer);
static void canvasUpdateRendererViewports(struct CoordinateCanvas* const canvas);
static void canvasUpdateRendererVertexBuffer(struct CoordinateCanvas* const canvas);

