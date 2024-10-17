#include "../../include/master-header.h"

/* 
 * === ALLOCATE CANVAS DATA MEMORY AND INITIALIZE/ALLOCATE/FILL OPENGL BUFFERS FOR RENDERING ===
*/
static struct CanvasPixel** allocate2DPixelArray(unsigned int numColumns, unsigned int numRows)
{
	// 1. allocate array of POINTERS to pixels
	// 2. allocate contiguous memory enough to store entire grid of pixels
	// 3. increment through array of POINTERS to pixels, point them to the 
	//    contigous memory in such a way that it takes the form of the 2D array
	// 4. to FREE the 2D array, first free the contiguous memory (stored at arr[0]),
	//    then free the array of POINTERS (stored at arr)
	
	struct CanvasPixel** canvas2DArray = malloc(numColumns * sizeof(struct CanvasPixel*)); // 1
						// calloc so checks for isVisible turn to GL_FALSE :/
	struct CanvasPixel* contiguousGridMemory = calloc(numColumns * numRows, sizeof(struct CanvasPixel)); // 2
	
	for (int i = 0; i < numColumns; i++) // 3
	{
		canvas2DArray[i] = contiguousGridMemory + (i * numRows);
	}

	return canvas2DArray;
}

// openGL stuff starts here
static void canvasInitializeGLBuffers(struct CoordinateCanvas* const canvas)
{
	glGenBuffers(1, &(canvas->glBuffers.VBO));
	glGenBuffers(1, &(canvas->glBuffers.EBO));
	glGenVertexArrays(1, &(canvas->glBuffers.VAO));
}
static void canvasAllocateAndFillVBO(struct CoordinateCanvas* const canvas)
{
	// create grid square unit model data
		// origin is bottom left for easy positioning in the grid during rendering
	const float lengthOfNDCRange = 2; // NDC range in openGL is (-1, 1)
	float canvasSquareUnitWidthNormalized = lengthOfNDCRange / canvas->gridUnitCnt.x;
	float canvasSquareUnitHeightNormalized = lengthOfNDCRange / canvas->gridUnitCnt.y;
	float canvasSquareUnitModel[4][2] = {
		{0.0, 0.0}, // bottom left
		{0.0, canvasSquareUnitHeightNormalized}, // top left
		{canvasSquareUnitWidthNormalized, canvasSquareUnitHeightNormalized}, // top right
		{canvasSquareUnitWidthNormalized, 0.0} // bottom right
	};

	// allocate memory (memory layout: canvas data first, square unit model second)
	unsigned long sizeOfBuffer = canvas->sizeOfCanvasData + sizeof(canvasSquareUnitModel);

	glBindBuffer(GL_ARRAY_BUFFER, canvas->glBuffers.VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeOfBuffer, NULL, GL_DYNAMIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, canvas->sizeOfCanvasData, canvas->canvasDataMemoryLocation); // canvas data contigous first
		glBufferSubData(GL_ARRAY_BUFFER, canvas->sizeOfCanvasData, sizeof(canvasSquareUnitModel), canvasSquareUnitModel); // square unit model second
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
// this function encompasses formatting vertex attributes, EBOS, and instanced arrays (anything VAO state)
static void canvasSetVAOVertexSpecification(struct CoordinateCanvas* const canvas)
{
	unsigned int squareUnitIndices[2][3] = {
		{0, 1, 2},
		{0, 2, 3}
	};

	glBindVertexArray(canvas->glBuffers.VAO);
		// index buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, canvas->glBuffers.EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(squareUnitIndices), squareUnitIndices, GL_STATIC_DRAW);
		
		// vertex attribute pointers
		glBindBuffer(GL_ARRAY_BUFFER, canvas->glBuffers.VBO);
			// instanced array: color
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(struct CanvasPixel), (void*)0);
			glVertexAttribDivisor(0, 1);

			// instanced array: isVisible
			glEnableVertexAttribArray(1);
			glVertexAttribIPointer(1, 1, GL_UNSIGNED_BYTE, sizeof(struct CanvasPixel), (void*)sizeof(Vec3));
			glVertexAttribDivisor(1, 1);
			
			// square unit model position attribute
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)canvas->sizeOfCanvasData);
	glBindVertexArray(0);
}
static void canvasSetUpVAOState(struct CoordinateCanvas* const canvas)
{
	canvasInitializeGLBuffers(canvas);
	canvasAllocateAndFillVBO(canvas);
	canvasSetVAOVertexSpecification(canvas);
}

static void canvasUpdateVBOCanvasData(struct CoordinateCanvas* const canvas)
{
	glBindBuffer(GL_ARRAY_BUFFER, canvas->glBuffers.VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, canvas->sizeOfCanvasData, canvas->canvasDataMemoryLocation);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/* 
 * === CONSTRUCTOR FUNCTIONS === 
*/
	// space is allocated for the canvasData, MEMORY MUST BE FREED (see above allocate function)
	// allocted buffers for openGL must also be destroyed
struct CoordinateCanvas canvas(const char* id, const Vec2 origin, const Vec2 size, 
			       const unsigned int xUnitCnt, const unsigned int yUnitCnt)
{
	struct CoordinateCanvas canvas;
	canvas.id = id;
	canvas.origin = origin;
	canvasSetSize(&canvas, size);
	canvas.gridUnitCnt.x = xUnitCnt;
	canvas.gridUnitCnt.y = yUnitCnt;
	canvas.aspectRatio = canvas.size.width / canvas.size.height;

	canvas.border = border(vec3(255,255,255), 1);
	canvas.isVisible = GL_TRUE;
	canvas.isMovableWithMouse = GL_TRUE;
	canvas.isScalableWithMouse = GL_TRUE;
	canvas.shouldMaintainAspectRatio = GL_FALSE;

	// dynamically allocate 2D array of CanvasPixels for canvasData
	canvas.canvasData = allocate2DPixelArray(xUnitCnt, yUnitCnt);
	canvas.canvasDataMemoryLocation = canvas.canvasData[0];
	canvas.numPixels = xUnitCnt * yUnitCnt;
	canvas.sizeOfCanvasData = canvas.numPixels * sizeof(struct CanvasPixel);

	// set up canvas VAO to be drawable
	canvasSetUpVAOState(&canvas);

	return canvas;
}

/* 
 * === SETTING FUNCTIONS ===
*/

// TRUNCATES OR EXPANDS THE MEMORY ALLOCATED TO CANVASDATA
// REALLOCATES THE VBO AND FILLS IN NEW DATA. VAO SHOULD NOT NEED TO BE RESPECIFIED SINCE VBO WILL HAVE THE SAME ID
void canvasSetGrid(struct CoordinateCanvas* const canvas, const unsigned int xUnitCnt, const unsigned int yUnitCnt)
{
	// due to the structure of the canvas grid data, a call to realloc would not suffice
	// since it would not accurately truncate or expand rows of the grid.
	// We must essentially implement a custom realloc to cover all bases

	if (xUnitCnt <= 0 || yUnitCnt <= 0) printf("canvasSetGrid WARNING: grid units cannot be zero!\n"); // haha my error handling is trash
	
	// allocated new space for 2D array
	struct CanvasPixel** newCanvas2DArray = allocate2DPixelArray(xUnitCnt, yUnitCnt);

	// COPY ALL FITTING DATA FROM ORIGINAL 2D ARRAY TO NEW ALLOCATED 2D ARRAY
	unsigned int smallerColumnCnt = MIN(xUnitCnt, canvas->gridUnitCnt.x);
	unsigned int smallerRowCnt = MIN(yUnitCnt, canvas->gridUnitCnt.y);
	for (int newColumnIdx = 0; newColumnIdx < smallerColumnCnt; newColumnIdx++)
	{
		memcpy(newCanvas2DArray[newColumnIdx], canvas->canvasData[newColumnIdx], smallerRowCnt * sizeof(struct CanvasPixel));
	}

	// update canvas
	canvas->gridUnitCnt.x = xUnitCnt;
	canvas->gridUnitCnt.y = yUnitCnt;
	canvas->numPixels = xUnitCnt * yUnitCnt;
	canvas->sizeOfCanvasData = canvas->numPixels * sizeof(struct CanvasPixel);

	canvasDataFree(canvas);
	canvas->canvasData = newCanvas2DArray;
	canvas->canvasDataMemoryLocation = canvas->canvasData[0];

	// update vertex array buffer
	// NOTE: this should free the last buffer and simply reallocate another space in GPU memory for the vertex buffer, while also keeping the same VBO ID.
	canvasAllocateAndFillVBO(canvas);
	canvasSetVAOVertexSpecification(canvas); // needs to be respecified since model data is now at a different offset. NOTE: Might be able to avoid this by making model data first in array buffer?
}
void canvasSetSize(struct CoordinateCanvas* const canvas, const Vec2 newSize)
{
	const uint MIN_CANVAS_DIMENSION = 10;
	if (!canvas->shouldMaintainAspectRatio)
	{
		canvas->size = vec2(MAX(MIN_CANVAS_DIMENSION, newSize.width), MAX(MIN_CANVAS_DIMENSION, newSize.height));
		return;
	}

	Vec2 deltaSize = vec2Sub(newSize, canvas->size);
	Vec2 newSizeWithAspectRatio;
	if (fabsf(deltaSize.width) > fabsf(deltaSize.height)) // the one that changed the most should take precedent in how maintaining aspect ratio is handled
		newSizeWithAspectRatio = vec2(newSize.width, newSize.width / canvas->aspectRatio);
	else 
		newSizeWithAspectRatio = vec2(newSize.height * canvas->aspectRatio, newSize.height);

	if (newSizeWithAspectRatio.width < MIN_CANVAS_DIMENSION || newSizeWithAspectRatio.height < MIN_CANVAS_DIMENSION)
		newSizeWithAspectRatio = canvas->aspectRatio > 1 ? vec2(MIN_CANVAS_DIMENSION * canvas->aspectRatio, MIN_CANVAS_DIMENSION) : vec2(MIN_CANVAS_DIMENSION, MIN_CANVAS_DIMENSION / canvas->aspectRatio);

	canvas->size = newSizeWithAspectRatio;
}

void canvasToggleBorder(struct CoordinateCanvas* const canvas)
{
	borderToggleVisibility(&(canvas->border));
}
void canvasMakeBorderVisible(struct CoordinateCanvas* const canvas)
{
	canvas->border.isVisible = GL_TRUE;
}
void canvasMakeBorderInvisible(struct CoordinateCanvas* const canvas)
{
	canvas->border.isVisible = GL_FALSE;
}

void canvasTranslate(struct CoordinateCanvas* const canvas, const Vec2 translate)
{
	canvas->origin = vec2Add(canvas->origin, translate);
}
void canvasScale(struct CoordinateCanvas* const canvas, const float scalar)
{
	canvasSetSize(canvas, vec2Scaled(canvas->size, scalar));
}

// CANVAS DATA MANIPULATING FUNCTIONS
void canvasFillColor(struct CoordinateCanvas* const canvas, const Vec3 color)
{
	struct CanvasPixel fillPixel = pixel(color);

	// traverse through grid linearely since it is contiguous memory
	for (int idx = 0; idx < canvas->numPixels; idx++)
	{
		canvas->canvasDataMemoryLocation[idx] = fillPixel;
	}
}
void canvasRowFillColor(struct CoordinateCanvas* const canvas, const int rowNum, const Vec3 color)
{
	// ERR
	if (rowNum >= canvas->gridUnitCnt.y) return;

	struct CanvasPixel fillPixel = pixel(color);
	for (int columnIdx = 0; columnIdx < canvas->gridUnitCnt.x; columnIdx++)
	{
		canvas->canvasData[columnIdx][rowNum] = fillPixel;
	}
}
void canvasSetPixel(struct CoordinateCanvas* const canvas, const Vec2 pixelCoords, const struct CanvasPixel pixel)
{
	// ERR
	if (pixelCoords.x >= canvas->gridUnitCnt.x || pixelCoords.y >= canvas->gridUnitCnt.y) return;

	// TODO: DAMN, FORGOT THAT VEC2 IS ONLY FLOATS. MIGHT NEED TO MAKE AN INT VERSION
	canvas->canvasData[(int)pixelCoords.x][(int)pixelCoords.y] = pixel;
}
void canvasClear(struct CoordinateCanvas* const canvas)
{
	memset(canvas->canvasDataMemoryLocation, 0, canvas->sizeOfCanvasData);
}

/*
 * === GETTING FUNCTIONS ===
*/
static int canvasGetBorderPixelThickness(const struct CoordinateCanvas* const canvas)
{
	const int THICKNESS_MULTIPLIER = 1;
	return THICKNESS_MULTIPLIER * canvas->border.thickness;
}
Vec2 canvasGetBorderOrigin(const struct CoordinateCanvas* const canvas)
{
	return vec2(canvas->origin.x - canvasGetBorderPixelThickness(canvas), canvas->origin.y - canvasGetBorderPixelThickness(canvas));
}
Vec2 canvasGetBorderSize(const struct CoordinateCanvas* const canvas)
{
	return vec2(canvas->size.width + (2 * canvasGetBorderPixelThickness(canvas)), canvas->size.height + (2 * canvasGetBorderPixelThickness(canvas)));
}

/*
 * === CANVAS DRAWING FUNCTION ===
*/
		// openGL window context already set by engine
void canvasDraw(struct CoordinateCanvas *const canvas, struct GameState* const game)
{
	// skip entire graphics pipeline if canvas is not visible
	if (canvas->isVisible == GL_FALSE) return;

	glBindVertexArray(canvas->glBuffers.VAO);
		/* BORDER RENDERING */
		if (canvas->border.isVisible)
		{
			gameStateUseProgram(game, game->gameInfo.programs.border);
			gameStateSetBorderColorUniform(game, canvas->border.color);

			// viewport encloses the canvas viewport
			Vec2 borderOrigin = canvasGetBorderOrigin(canvas);
			Vec2 borderSize = canvasGetBorderSize(canvas);
			
			glViewport(borderOrigin.x, borderOrigin.y, borderSize.width, borderSize.height);

			Vec2 NDCrange = vec2(-1, 1);
			Vec2 canvasOriginInNDC = vec2(MAP_RANGE(canvas->origin.x, vec2(borderOrigin.x, borderOrigin.x + borderSize.width), NDCrange), 
						      MAP_RANGE(canvas->origin.y, vec2(borderOrigin.y, borderOrigin.y + borderSize.height), NDCrange));
			gameStateSetCanvasBottomLeftCoordsUniform(game, canvasOriginInNDC);
			
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

		/* CANVAS RENDERING */
		gameStateUseProgram(game, game->gameInfo.programs.canvas);

		canvasUpdateVBOCanvasData(canvas);
		gameStateSetGridUnitCntUniform(game, canvas->gridUnitCnt.x, canvas->gridUnitCnt.y);

		// viewport encompasses the canvas boundary in the GLFW window for easy positioning of each square unit in the canvas grid
		glViewport(canvas->origin.x, canvas->origin.y, canvas->size.width, canvas->size.height);
		glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, canvas->numPixels);
	glBindVertexArray(0);
}

/* 
 * === DESTROYER FUNCTIONS ===
*/
void canvasDataFree(struct CoordinateCanvas* canvas)
{
	// first free the grid of pixels
	free(canvas->canvasData[0]);
	// then free the array of pointers to each column of pixels
	free(canvas->canvasData);
}
void canvasDestroy(struct CoordinateCanvas* canvas)
{
	// free the canvas data
	// TODO: USE NEW MEMORY UTIL FOR 2D ARRAYS
	canvasDataFree(canvas);

	// destroy the allocated buffers
	glDeleteBuffers(1, &(canvas->glBuffers.VBO));
	glDeleteBuffers(1, &(canvas->glBuffers.EBO));
	glDeleteVertexArrays(1, &(canvas->glBuffers.VAO));
}
