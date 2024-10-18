#pragma once

#include "../utility/utility-header.h"
#include "../canvas/canvas-header.h"

#include "game-object.h"

/* 
 * ===
 * GameState is for the user to interact with the engine. Games will load canvases and GameObjects
 * into the arrays. 
 * 
 * A singleton GameState will be made on program run, and will be initiated with a user defined
 * init function, where canvases and game objects will be added to their arrays, and the pre/post
 * update funtions will be deifned
 *
 * NOTE: This singleton GameState is stored in the GLFW window user pointer at all times
 * 	 for use in the GLFW callback functions
 *
 * ===
*/
struct GameState {
	struct InputData inputData;
	// NOTE: arrays will always stay sorted by their IDs
	struct CoordinateCanvas* canvasRenderingArray[GRID_MAX_CANVAS_AMT];
	struct GameObject* objectArray[GRID_MAX_GAME_OBJECTS_AMT];

	// these will *optionally* be defined by the user for functions that will return before and after the game state updates via the game objects
	int (*preUpdate)(struct GameState*);
	int (*postUpdate)(struct GameState*);

	// SHOULD NOT BE MODIFIED BY THE USER
	struct {
		GLFWwindow* window;
		int numCanvases;
		int numObjects;

		struct {
			// seperate programs are used for rendering the canvas and the canvas border
			ShaderProgram canvas;
			ShaderProgram border;
			ShaderProgram currentlyActive;
		} programs;

		struct {
			Uniform canvasGridUnitCnt;
			Uniform borderColor;
			Uniform canvasBottomLeftCoordsInNDC;
		} uniforms;

		struct {
			// these are all the standard cursors the engine will use
			GLFWcursor* crosshair;
			GLFWcursor* hResize;
			GLFWcursor* vResize;
		} cursors;

		// TODO: this struct getting way too bulky. this definitely needs to be seperated into seperate structs at some point and better design, anonymous enum :/
		enum CursorStates cursorState;
		
	} gameInfo;

	// these values are modified every frame of the game loop function
	struct {
		double currentTime;
		double previousTime;
		double deltaTime;
		double FPS;
	} timeData;

	struct InputData previousInputData;

	// used for mouse interactions with canvases. Is set to the canvas currently being interacted with, NULL otherwise
	// WARN: this must be set to NULL if the canvas is removed from the array so no dangling pointers exist
	// WARN: any use of this pointer must check for a NULL value
	struct CoordinateCanvas* targetCanvasForMouseInteractions;
};

struct GameState gameState(GLFWwindow* window, const ShaderProgram canvas, const ShaderProgram border);

int defaultFunctionForPreAndPostUpdate(struct GameState* game); // do nothing function
void gameStateAttachPreUpdateFunction(struct GameState* game, int (*preUpdateFunction)(struct GameState*));
void gameStateAttachPostUpdateFunction(struct GameState* game, int (*postUpdateFunction)(struct GameState*));

void initGame(struct GameState* const game); // user defined function

void gameStateUpdateTime(struct GameState* const game);

// =========================================================================================
// ARRAY ADDING, REMOVING, AND QUERYING FUNCTIONS

// NULL IF NOT FOUND
struct CoordinateCanvas* gameStateGetCanvas(const struct GameState* const game, const uint id);
struct GameObject* gameStateGetObject(const struct GameState* const game, const uint id);

// these functions will allocate memory in the heap and store it in the array
// returns pointer to the heap data, NULL if arrays are full
struct CoordinateCanvas* gameStateAddCopyOfCanvas(struct GameState* const game, const struct CoordinateCanvas canvas);
struct GameObject* gameStateAddCopyOfObject(struct GameState* const game, const struct GameObject* const object, const size_t sizeOfGameObject);

// these functions will call the destroyer functions, free the allocated heap memory, and remove the pointer from the arrays
// return 1 on success, 0 if canvas not found 
int gameStateRemoveCanvas(struct GameState* const game, const uint id);
int gameStateRemoveObject(struct GameState* const game, const uint id);

// ARRAY ADDING, REMOVING, AND QUERYING FUNCTIONS
// =========================================================================================

// called as the final drawing call in the game loop
void gameStateDraw(struct GameState* const game);

// setting function to ensure the correct program is in use. NOTE: will change the program back to what is originally was if changed
void gameStateSetGridUnitCntUniform(struct GameState* const game, const unsigned int xCnt, const unsigned int yCnt);
void gameStateSetBorderColorUniform(struct GameState* const game, const Vec3 color);
void gameStateSetCanvasBottomLeftCoordsUniform(struct GameState* const game, const Vec2 coordsInNDC);

// WARN: USE THIS FUNCTION INSTEAD TO UPDATE THE CURRENTLY ACTIVE PROGRAM
void gameStateUseProgram(struct GameState* const game, ShaderProgram program);

// WARN: SAME AS ABOVE, USE THIS FUNCTION TO UPDATE THE SET CURSOR ACCORDINGLY
void gameStateSetCursorState(struct GameState* const game, int cursorState); // hacky..

// removes every canvas and object in the rendering array, destroys every shader program, and destroys every cursor
// NOTE: a complete destroying of the engine is done in a seperate encapsulating functions which also calls gameStateDestroy 
void gameStateDestroy(struct GameState* game);

