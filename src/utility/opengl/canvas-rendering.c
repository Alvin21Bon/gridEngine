#include "utility/opengl/canvas-rendering.h"

#include "canvas/coordinate-canvas.h"
#include "canvas/canvas-pixel.h"
#include "utility/opengl/glad.h"
#include "utility/opengl/shader-programs.h"
#include "utility/opengl/viewports.h"
#include "utility/color.h"
#include "utility/constants.h"
#include <stddef.h>
#include <lina/lina.h>
#include <sys/types.h>

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
		glUseProgram(shaderProgramManager->canvas);
		{
			viewportUse(&canvas->renderer.canvasViewport);
			glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, canvas->numPixels);
		}

		if (canvas->border.isVisible)
		{
			glUseProgram(shaderProgramManager->border);
			{
				viewportUse(&canvas->renderer.borderViewport);
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			}
		}
	}
}


static void canvasRendererCreateGLObjects(struct CanvasRenderer* const canvasRenderer)
{
	glGenBuffers(1, &canvasRenderer->VBO);
	glGenVertexArrays(1, &canvasRenderer->VAO);
}
static void canvasRendererVertexSpecification(struct CanvasRenderer* const canvasRenderer)
{
	// these are made here and are static so the same buffers can be used across ALL made VAOS
	static uint unitSquareVBO = 0;
	static uint EBO = 0;
	// these ifs make it so that these buffers are only generated and filled with data ONCE, so there is 
	// only ever one buffer each for the unitsquarevbo and EBO data that every single VAO will point to
	if (unitSquareVBO == 0)
	{
		float unitSquareModelData[4][2] = {
			{0, 0}, // bottom left
			{0, 1}, // top left
			{1, 1}, // top right
			{1, 0} // bottom right
		};
		glGenBuffers(1, &unitSquareVBO);
		glBindBuffer(GL_ARRAY_BUFFER, unitSquareVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(unitSquareModelData), unitSquareModelData, GL_STATIC_DRAW);
	}
	if (EBO == 0)
	{
		uint unitSquareIndices[2][3] = {
			{0, 1, 2},
			{0, 2, 3}
		};
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unitSquareIndices), unitSquareIndices, GL_STATIC_DRAW);
	}

	glBindVertexArray(canvasRenderer->VAO);
	{
		// index buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

		// pixel data vertex attribute pointers
		glBindBuffer(GL_ARRAY_BUFFER, canvasRenderer->VBO);
		{
			// instanced array: color
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(struct CanvasPixel), (void*)0);
			glVertexAttribDivisor(0, 1);

			// instanced array: isVisible
			glEnableVertexAttribArray(1);
			glVertexAttribIPointer(1, 1, GL_UNSIGNED_BYTE, sizeof(struct CanvasPixel), (void*)sizeof(Color));
			glVertexAttribDivisor(1, 1);
		}

		// unit square model data vertex attribute pointers
		glBindBuffer(GL_ARRAY_BUFFER, unitSquareVBO);
		{
			// unit square model position attribute
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
		}
	}
}
static void canvasRendererAllocateVertexBuffer(struct CanvasRenderer* const canvasRenderer)
{
	glBindBuffer(GL_ARRAY_BUFFER, canvasRenderer->VBO);
		glBufferData(GL_ARRAY_BUFFER, canvasRenderer->sizeOfVertexBuffer, NULL, GL_DYNAMIC_DRAW);
}
static void canvasUpdateRendererViewports(struct CoordinateCanvas* const canvas)
{
	struct CanvasRenderer* const renderer = &canvas->renderer;

	renderer->canvasViewport.origin = ivec2(canvas->origin.x, canvas->origin.y);
	renderer->canvasViewport.size = ivec2(canvas->size.width, canvas->size.height);

	Vec2 borderOrigin = canvasGetBorderOrigin(canvas);
	Vec2 borderSize = canvasGetBorderSize(canvas);
	renderer->borderViewport.origin = ivec2(borderOrigin.x, borderOrigin.y);
	renderer->borderViewport.size = ivec2(borderSize.width, borderSize.height);
}
static void canvasUpdateRendererVertexBuffer(struct CoordinateCanvas* const canvas)
{
	glBindBuffer(GL_ARRAY_BUFFER, canvas->renderer.VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, canvas->renderer.sizeOfVertexBuffer, canvas->addressOfPixelArray);
}

