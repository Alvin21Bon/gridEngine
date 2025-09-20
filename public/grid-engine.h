#pragma once

#include <lina/lina.h>
#include <GLFW/glfw3.h>
#include <sys/types.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>


struct GridEngine;


/*
 * ===========================================
 * GLOBALS
 * ===========================================
*/

extern ulong GRID_FRAME_COUNTER;


/*
 * ===========================================
 * LOGGING 
 * ===========================================
*/

// Logs get sent for logs marked at the current logging level and below
enum LoggingLevels {
	GRID_LOGGING_OFF,
	GRID_LOGGING_ERROR,
	GRID_LOGGING_WARN,
	GRID_LOGGING_FULL
};

// @param funcName should always be "__func__"
// @param lineNum should always be "__LINE__"
void LOG(const enum LoggingLevels logLevel, const char* funcName, const int lineNum, const char* format, ...);


/*
 * ===========================================
 * COLOR 
 * ===========================================
*/

typedef Vec3 Color;

// @param color values should be in the range [0, 255]
// @param color values get mapped to the range [0, 1]
Color color(const uint r, const uint g, const uint b);


/*
 * ===========================================
 * MATH UTILITY
 * ===========================================
*/

static inline float MIN(float a, float b)
{
	return (a < b ? a : b);
}
static inline float MAX(float a, float b)
{
	return (a > b ? a: b);
}

// map a number from one range of numbers to another range of numbers
static inline float MAP_RANGE(float input, Vec2 originalRange, Vec2 newRange)
{
	return (input - originalRange.elements[0]) * ((newRange.elements[1] - newRange.elements[0]) / (originalRange.elements[1] - originalRange.elements[0])) + newRange.elements[0];
}

// maps an input vector from one grid to another grid
static inline Vec2 MAP_BOXED_RANGE(Vec2 input, Vec2 originalBotLeft, Vec2 originalTopRight, Vec2 newBotLeft, Vec2 newTopRight)
{
	return vec2(MAP_RANGE(input.x, vec2(originalBotLeft.x, originalTopRight.x), vec2(newBotLeft.x, newTopRight.x)), MAP_RANGE(input.y, vec2(originalBotLeft.y, originalTopRight.y), vec2(newBotLeft.y, newTopRight.y)));
}

static inline bool IS_IN_RANGE(float input, Vec2 range)
{
	return range.elements[0] <= input && input <= range.elements[1];
}

// bottom left and top right vecs create a box, and checks whether this box captures the input vec
static inline bool IS_IN_BOXED_RANGE(Vec2 input, Vec2 bottomLeft, Vec2 topRight)
{
	return (IS_IN_RANGE(input.x, vec2(bottomLeft.x, topRight.x)) && IS_IN_RANGE(input.y, vec2(bottomLeft.y, topRight.y)));
}


/*
 * ===========================================
 * MEMORY UTILITY
 * ===========================================
*/

// Takes a splice of memory on range [start, end) and copies it to another address
// @exception {SegmentationFault}
static inline void MEMMOVE_RANGE(void* const destination, void* const spliceRangeStart, void* const spliceRangeEnd)
{
	memmove(destination, spliceRangeStart, (char*)spliceRangeEnd - (char*)spliceRangeStart);
}

// NOTE: 2D array is zeroed out
// @returns {nullptr} if any arguments are 0
static inline void** ALLOCATE_2D_ARRAY(const size_t numColumns, const size_t numRows, const size_t sizeOfType)
{
	if (numColumns == 0 || numRows == 0 || sizeOfType == 0) return nullptr;

	void** pointerArray = malloc(numColumns * sizeof(void*));
	void* memoryFor2DArray = calloc(numColumns * numRows, sizeOfType); 

	for (size_t col = 0; col < numColumns; col++)
	{
		pointerArray[col] = (char*)memoryFor2DArray + (col * numRows * sizeOfType);
	}

	return pointerArray;
}

static inline void DESTROY_2D_ARRAY(void** array)
{
	free(array[0]);
	array[0] = nullptr;

	free(array);
	array = nullptr;
}

// allocates new 2D array grid space and copies fitting data from old 2D array. destroys the old 2D array
// @returns {nullptr} if any num arguments are 0
static inline void** REALLOCATE_2D_ARRAY(void** array, const size_t oldNumColumns, const size_t oldNumRows, const size_t newNumColumns, const size_t newNumRows, const size_t sizeOfType)
{
	if (oldNumColumns == 0 || oldNumRows == 0 || newNumColumns == 0 || newNumRows == 0 || sizeOfType == 0) 
	{
		// still destroys here in case user sets 2D array to function return value, thus losing the old 2D array
		DESTROY_2D_ARRAY(array); 
		return nullptr;
	}

	void** new2DArray = ALLOCATE_2D_ARRAY(newNumColumns, newNumRows, sizeOfType);

	size_t smallerNumColumns = MIN(oldNumColumns, newNumColumns);
	size_t smallerNumRows = MIN(oldNumRows, newNumRows);
	for (size_t newColumnIdx = 0; newColumnIdx < smallerNumColumns; newColumnIdx++)
	{
		memcpy(new2DArray[newColumnIdx], array[newColumnIdx], smallerNumRows * sizeOfType);
	}

	DESTROY_2D_ARRAY(array);

	return new2DArray;
}


/*
 * ===========================================
 * FILE UTILITY
 * ===========================================
*/

// NOTE: only ever checks if file exists. does not take into account other errors, such as insufficient permissions
static inline bool CHECK_FILE_EXISTS(const char* filename)
{
	FILE* fp = fopen(filename, "r");

	// must check for NULL value first, to know if errno was actually set by fopen call
	bool doesFileExist;
	doesFileExist = (fp == nullptr && errno == ENOENT) ? false : true;

	if (fp != nullptr) fclose(fp);
	return doesFileExist;
}

// @returns {false} if file was unable to be created or cleared
static inline bool CREATE_OR_CLEAR_FILE(const char* filename)
{
	FILE* fp = fopen(filename, "w");
	if (fp != nullptr)
	{
		fclose(fp);
		return true;
	}

	// here, fp == null. just log some possible error values
	switch (errno)
	{
		case ENOENT:
		case ENOTDIR:
		case EISDIR:
			LOG(GRID_LOGGING_ERROR, __func__, __LINE__, "%s is invalid\n", filename);
			break;
		case EACCES:
			LOG(GRID_LOGGING_ERROR, __func__, __LINE__, "insufficient permissions to write to %s\n", filename);
			break;
		default:
			LOG(GRID_LOGGING_ERROR, __func__, __LINE__, "%s results in an error: %s\n", filename, strerror(errno));
			break;
	}

	return false;
}


/*
 * ===========================================
 * USER RESPONSE UTILITY
 * ===========================================
*/

// this is simply a wrapper for the getline function that also prints the prompt to the screen
// @param {responsePtr} same as getline, should be freed
static inline ssize_t GET_USER_RESPONSE_WITH_PROMPT(char** responsePtr, size_t* allocatedBufferSize, const char* const prompt)
{
	ssize_t numCharsRead;

	printf("%s", prompt);
	numCharsRead = getline(responsePtr, allocatedBufferSize, stdin);
	printf("\n");

	return numCharsRead;
}

// @returns {char} the char the user responded with 
// @returns {EOF} if next getline reaches EOF
static inline char PROMPT_CHAR_RESPONSE_FROM_USER(const char* const charResponseList, const char* const prompt)
{
	char* retrievedLineBufferPtr = nullptr;
	size_t sizeOfRetrievedLineBuffer = 0;
	ssize_t numCharsRead;
	char userResponseChar;

	while ((numCharsRead = GET_USER_RESPONSE_WITH_PROMPT(&retrievedLineBufferPtr, &sizeOfRetrievedLineBuffer, prompt)) != EOF)
	{
		if (numCharsRead > 2) continue; // only 1 character + \n
		
		userResponseChar = retrievedLineBufferPtr[0];
		if (strchr(charResponseList, userResponseChar)) break;
	}
	free(retrievedLineBufferPtr);

	return numCharsRead == EOF ? EOF : userResponseChar;
}


/*
 * ===========================================
 * GRID ENGINE STATES
 * ===========================================
*/

// These values are returned from the GameState pre/post update functions, and any of the
// user defined GameObject update functions. 
// GRID_ENGINE_SUCCESS and GRID_ENGINE_ERROR signal to cease the game loop on a success or failure condition
// GRID_ENGINE_RUNNING dicates normal game behavior, and signals to progress with the game loop as normal
// GRID_ENGINE_PAUSED signals a pause, which will hault game updates until the state is changed
enum GridEngineStates {
	GRID_ENGINE_SUCCESS, 
	GRID_ENGINE_ERROR, 

	GRID_ENGINE_RUNNING,
	GRID_ENGINE_PAUSED
};


/*
 * ===========================================
 * GRID ENGINE OPTIONS
 * ===========================================
*/

enum GridEngineOptions {
	// VALUE TYPE: uint
	GRID_OPTION_MAX_FPS,
	// VALUE TYPE: uint
	GRID_OPTION_BORDER_THICKNESS_MULTIPLIER,
	// VALUE TYPE: Color
	GRID_OPTION_CLEAR_COLOR,
	// VALUE TYPE: char*
	GRID_OPTION_WINDOW_TITLE,
	// VALUE TYPE: uVec2
	GRID_OPTION_WINDOW_SIZE,
	// VALUE TYPE: enum LoggingLevels
	GRID_OPTION_LOGGING_LEVEL,
	// VALUE TYPE: bool
	GRID_OPTION_LOGGING_DISABLE_FILEPATH_EXISTS_WARNING,
	// VALUE TYPE: char*
	GRID_OPTION_LOGGING_FILEPATH,
	// VALUE TYPE: bool
	GRID_OPTION_LOGGING_ENABLE_FPS,
};

extern uint GRID_MAX_FPS;
extern uint GRID_BORDER_THICKNESS_MULTIPLIER;
extern Color GRID_CLEAR_COLOR;
extern char* GRID_WINDOW_TITLE;
extern uVec2 GRID_WINDOW_SIZE;
extern enum LoggingLevels GRID_LOGGING_LEVEL;
extern bool GRID_LOGGING_DISABLE_FILEPATH_EXISTS_WARNING;
extern char* GRID_LOGGING_FILEPATH;
extern bool GRID_LOGGING_ENABLE_FPS;

void gridEngineChangeOption(const enum GridEngineOptions option, const void* const optionValue);


/*
 * ===========================================
 * ATTACHING USER-DEFINED FUNCTIONS
 * ===========================================
*/

void gridEngineAttachPreTickFunction(struct GridEngine* engine, enum GridEngineStates (*preTickFunction)(struct GridEngine* const));
void gridEngineAttachPostTickFunction(struct GridEngine* engine, enum GridEngineStates (*postTickFunction)(struct GridEngine* const));
void gridEngineAttachPausedStateFunction(struct GridEngine* const engine, enum GridEngineStates (*pausedStateFunction)(struct GridEngine* const));


/*
 * ===========================================
 * INPUT FETCHING
 * ===========================================
*/

bool isKeyDown(const struct GridEngine* const engine, const int key);
ulong getNumFramesKeyPressed(const struct GridEngine* const engine, const int key);
ulong getNumFramesKeyReleased(const struct GridEngine* const engine, const int key);
bool isKeyFirstFramePressed(const struct GridEngine* const engine, const int key);
bool isMouseButtonDown(const struct GridEngine* const engine, const int button);
ulong getNumFramesMouseButtonPressed(const struct GridEngine* const engine, const int button);
ulong getNumFramesMouseButtonReleased(const struct GridEngine* const engine, const int button);
bool isMouseButtonFirstFramePressed(const struct GridEngine* const engine, const int button);
dVec2 getCursorPos(const struct GridEngine* const engine);
dVec2 getPrevCursorPos(const struct GridEngine* const engine);
dVec2 getDeltaCursorPos(const struct GridEngine* const engine);
bool isCursorMoving(const struct GridEngine* const engine);


/*
 * ===========================================
 * TIME FETCHING
 * ===========================================
*/

double getCurrentTime(const struct GridEngine* const engine);
double getPrevTime(const struct GridEngine* const engine);
double getDeltaTime(const struct GridEngine* const engine);
// short-hand for getDeltaTime
double dt(const struct GridEngine* const engine); 
double getFPS(const struct GridEngine* const engine);


/*
 * ===========================================
 * CURSOR SHAPE MANAGING
 * ===========================================
*/

enum CursorShapes {
	GRID_CURSOR_ARROW, 
	GRID_CURSOR_IBEAM,
	GRID_CURSOR_CROSSHAIR,
	GRID_CURSOR_POINTING,
	GRID_CURSOR_RESIZE_HORIZONTAL,
	GRID_CURSOR_RESIZE_VERTICAL,
	GRID_CURSOR_RESIZE_TOPLEFT_BOTRIGHT,
	GRID_CURSOR_RESIZE_TOPRRIGHT_BOTLEFT,
	GRID_CURSOR_RESIZE_ALL,
	GRID_CURSOR_NOT_ALLOWED
};

enum CursorShapes getCursorShape(const struct GridEngine* const engine);
void setCursorShape(struct GridEngine* const engine, const enum CursorShapes shape);


/*
 * ===========================================
 * CANVAS BORDER
 * ===========================================
*/

struct CanvasBorder {
	Color color;
	float thickness;
	bool isVisible;
};

struct CanvasBorder border(const Color color, const float thickness);


/*
 * ===========================================
 * CANVAS PIXEL
 * ===========================================
*/

struct CanvasPixel {
	Color color;
	bool isVisible;
};

struct CanvasPixel pixel(const Color color);


/*
 * ===========================================
 * COORDINATE CANVAS
 * ===========================================
*/

// NOTE: the following two structs are mainly only included because they are needed for the canvas struct definition

struct Viewport {
	iVec2 origin;
	iVec2 size;
};
struct CanvasRenderer {
	uint VBO, VAO; // EBO is the same across all canvas renderers, thus does not need to be stored here
	size_t sizeOfVertexBuffer;
	struct Viewport canvasViewport;
	struct Viewport borderViewport;
};

struct CoordinateCanvas {
	const char* id; 
	Vec2 origin;
	Vec2 size;
	uVec2 gridUnitCnt;
	float aspectRatio; 
	struct CanvasBorder border;

	struct {
		bool isMoveableWithMouse;
		bool isMoveable; // if false, overrides mouse movement
		bool isScalableWithMouse;
		bool isScalable; // if false, overrides mouse scaling
		bool shouldMaintainAspectRatio;
		bool isVisible;
	} options;

	// coordinates for the grid start at (0, 0)
	struct CanvasPixel** pixels; 
	// points to the actual location of contigous memory of pixels
	struct CanvasPixel* addressOfPixelArray;
	size_t numPixels;

	/* TYPICALLY SHOULD NOT BE MODIFIED DIRECTLY*/
	struct CanvasRenderer renderer;
};

struct CoordinateCanvas canvas(const char* id, const Vec2 origin, const Vec2 size, const uVec2 gridUnitCnt);
void canvasSetGrid(struct CoordinateCanvas* const canvas, uVec2 newGridUnitCnt);
void canvasSetSize(struct CoordinateCanvas* const canvas, const Vec2 newSize);
void canvasScale(struct CoordinateCanvas* const canvas, const float scalar);
void canvasTranslate(struct CoordinateCanvas* const canvas, const Vec2 translate);
void canvasToggleBorder(struct CoordinateCanvas* const canvas);
void canvasMakeBorderVisible(struct CoordinateCanvas* const canvas);
void canvasMakeBorderInvisible(struct CoordinateCanvas* const canvas);
Vec2 canvasGetBorderOrigin(const struct CoordinateCanvas* const canvas);
Vec2 canvasGetBorderSize(const struct CoordinateCanvas* const canvas);


/*
 * ===========================================
 * CANVAS DRAWING
 * ===========================================
*/

void canvasFillColor(struct CoordinateCanvas* const canvas, const Color color);
void canvasRowFillColor(struct CoordinateCanvas* const canvas, const uint rowNum, const Color color);
void canvasSetPixel(struct CoordinateCanvas* const canvas, const uVec2 pixelCoords, const struct CanvasPixel pixel);
void canvasClear(struct CoordinateCanvas* const canvas);
void canvasDrawLine(struct CoordinateCanvas* const canvas, const Color color, const uVec2 p0, const uVec2 p1);


/*
 * ===========================================
 * MANAGING ENGINE CANVAS ARRAY
 * ===========================================
*/

#define GRID_MAX_CANVAS_AMT 128 
struct CanvasArray {
	struct CoordinateCanvas* elements[GRID_MAX_CANVAS_AMT];
	size_t num;
};

struct CanvasArray getAllCanvases(const struct GridEngine* const engine);

// Allocates canvas to the heap and stores it in engine canvas array
// @param {canvas} gets modified to point to new canvas on heap
// @returns {bool} false if engine canvas array full
bool attachCanvas(struct GridEngine* const engine, const struct CoordinateCanvas* canvas);

// @returns {bool} false if id not found
// NOTE: this destroys the removed canvases
bool removeCanvas(struct GridEngine* const engine, const char* id);

// @returns array of canvases that match the ID
// NOTE: up to the user to check if array has a num of 0
// WARN: modifications to the returned canvases also modify the engine's canvas array
struct CanvasArray getCanvas(const struct GridEngine* const engine, const char* id);


/*
 * ===========================================
 * GAME OBJECT
 * ===========================================
*/

/* 
 *
 * GameObject will be "inhereted" by users to create their own extended game objects. These will be stored in the 
 * GameState by casting the pointer to one of GameObject. This will allow for the engine to call the update() and
 * draw() functions within the game loop's update function.
 *
 * NOTE: typically, the update function will make any changes to the user defined game object data and ensure that the
 * 	 game object is in a valid state. Any call to the GameObject draw call assumes the game object is in a valid
 * 	 state. the draw function will modify the canvas accordingly.
 *
 * NOTE: the destroyer function is called when the game object is removed from the game state so necessary cleanup for the user defined object can be done
 *
 * NOTE: in the draw calls, it is GUARANTEED the target canvas exists since the engine checks this before the draw function is called
*/
struct GameObject {
	const char* id;
	char* canvasId; // stores ID since storing a pointer to the canvas could be dangerous if canvas is already removed from the renderingArray
	bool enabled; // if disabled, will not do anything during engine tick
	enum GridEngineStates (*update)(struct GameObject* const, struct GridEngine* const);
	void (*draw)(const struct GameObject* const, struct CoordinateCanvas* const);
	void (*destroy)(struct GameObject*, struct GridEngine* const);
};

struct GameObject gameObject(const char* id, char* canvasId);


/*
 * ===========================================
 * MANAGING ENGINE GAME OBJECT ARRAY
 * ===========================================
*/

#define GRID_MAX_GAME_OBJECTS_AMT 4096
struct GameObjectArray {
	struct GameObject* elements[GRID_MAX_GAME_OBJECTS_AMT];
	size_t num;
};

struct GameObjectArray getAllGameObjects(const struct GridEngine* const engine);

// Allocates object to the heap and stores it in the engine object array
// @param {sizeOfGameObject} since GameObjects extend the base GameObject struct, this is to know how much space to allocate
// @param {gameObject} pointer gets changed to point to new game object on heap
// @returns {bool} false if engine game object array full
bool attachGameObject(struct GridEngine* const engine, const struct GameObject* object, const size_t sizeOfObject);

// @returns {bool} false if id not found
// NOTE: this destroys the removed game objects
bool removeGameObject(struct GridEngine* const engine, const char* id);

// @returns array of game objects that match the ID
// NOTE: up to the user to check if gameObjectArrat has a num of 0 if id not found
// WARN: any altering of the GameObjects in the returned array will impact the original engine GameObjectArray
struct GameObjectArray getGameObject(const struct GridEngine* const engine, const char* id);


/*
 * ===========================================
 * BUILT IN CURSOR CANVAS INTERACTIONS
 * ===========================================
*/

enum CursorCanvasInteractionTypes {
	IDLE,
	MOVING,
	L_RESIZE,
	R_RESIZE,
	U_RESIZE,
	D_RESIZE
};

struct InteractedCanvas {
	struct CoordinateCanvas* target;
	enum CursorCanvasInteractionTypes type;
};

struct BuiltInCursorCanvasInteractions {
	struct GameObject parent;
	struct InteractedCanvas interaction;
};

struct BuiltInCursorCanvasInteractions builtInCursorCanvasInteractions();


/*
 * ===========================================
 * BUILT IN CURSOR DRAWING
 * ===========================================
*/

struct BuiltInCursorDrawing {
	struct GameObject parent;
	Color penColor;
	uVec2 cursorPosMappedToCanvas;
	uVec2 prevCursorPosMappedToCanvas;
};

struct BuiltInCursorDrawing builtInCursorDrawing();
void cursorDrawingChangePenColor(struct BuiltInCursorDrawing* const cursorDrawing, const Color color);


/*
 * ===========================================
 * EXTRA HELPERS
 * ===========================================
*/

struct CoordinateCanvas* getCanvasUnderCursor(const struct GridEngine* const engine);

