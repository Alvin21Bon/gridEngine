#include "../include/grid-engine.h"

static struct CanvasPixel** allocate2DPixelArray(unsigned int numColumns, unsigned int numRows)
{
	// 1. allocate array of POINTERS to pixels
	// 2. allocate contiguous memory enough to store entire grid of pixels
	// 3. increment through array of POINTERS to pixels, point them to the 
	//    contigous memory in such a way that it takes the form of the 2D array
	// 4. to FREE the 2D array, first free the contiguous memory (stored at arr[0]),
	//    then free the array of POINTERS (stored at arr)
	
	struct CanvasPixel** canvas2DArray = malloc(numColumns * sizeof(struct CanvasPixel*)); // 1
	struct CanvasPixel* contiguousGridMemory = malloc(numColumns * numRows * sizeof(struct CanvasPixel)); // 2
	
	for (int i = 0; i < numColumns; i++) // 3
	{
		canvas2DArray[i] = contiguousGridMemory + (i * numRows);
	}

	return canvas2DArray;
}

/* 
 * === CONSTRUCTOR FUNCTIONS === 
*/
	// space is allocated for the canvasData, MEMORY MUST BE FREED (see above allocate function)
struct CoordinateCanvas canvas(const char* const id, const Vec2 origin, const Vec2 size, 
			       const unsigned int xUnitCnt, const unsigned int yUnitCnt)
{
	struct CoordinateCanvas canvas;
	canvas.id = id;
	canvas.origin = origin;
	canvas.size = size;
	canvas.gridUnitCnt.x = xUnitCnt;
	canvas.gridUnitCnt.y = yUnitCnt;
	canvas.border = border(vec3(255,255,255), 1);
	canvas.isMovable = GL_TRUE;
	canvas.isVisible = GL_TRUE;
	// dynamically allocate 2D array of CanvasPixels for canvasData
	canvas.canvasData = allocate2DPixelArray(xUnitCnt, yUnitCnt);

	return canvas;
}

/* 
 * === SETTING FUNCTIONS ===
*/

// TRUNCATES OR EXPANDS THE MEMORY ALLOCATED TO CANVASDATA
void canvasSetGrid(struct CoordinateCanvas* const canvas, const unsigned int xUnitCnt, const unsigned int yUnitCnt)
{
	// due to the structure of the canvas grid data, a call to realloc would not suffice
	// since it would not accurately truncate or expand rows of the grid.
	// We must essentially implement a custom realloc to cover all bases
	
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
	canvasDataFree(canvas);
	canvas->canvasData = newCanvas2DArray;
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

void canvasToggleMovable(struct CoordinateCanvas* const canvas)
{
	canvas->isMovable = !canvas->isMovable;
}

void canvasTranslate(struct CoordinateCanvas* const canvas, const Vec2 translate)
{
	canvas->origin = vec2Add(canvas->origin, translate);
}
void canvasScale(struct CoordinateCanvas* const canvas, const float scalar)
{
	canvas->size = vec2Scaled(canvas->size, scalar);
}

// CANVAS DATA MANIPULATING FUNCTIONS
void canvasFillColor(struct CoordinateCanvas* const canvas, const Vec3 color)
{
	struct CanvasPixel fillPixel = pixel(color);

	// traverse through grid linearely since it is contiguous memory
	int numPixels = canvas->gridUnitCnt.x * canvas->gridUnitCnt.y;
	for (int idx = 0; idx < numPixels; idx++)
	{
		canvas->canvasData[0][idx] = fillPixel;
	}
}
// NOTE: grid coordinates start at (0, 0)
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

// CANVAS DRAWING FUNCTIONS
void canvasDraw(GLFWwindow* window, const struct CoordinateCanvas* const canvas); 

/* 
 * === DESTORYER FUNCTIONS ===
*/
void canvasDataFree(struct CoordinateCanvas* canvas)
{
	// first free the grid of pixels
	free(canvas->canvasData[0]);
	// then free the array of pointers to each column of pixels
	free(canvas->canvasData);
}

