#ifndef GRID_GAME_STATE_H
#define GRID_GAME_STATE_H

#include "shared.h"
#include "input-data.h"
#include "canvas-types.h"

/* 
 * ===
 * GameState is for the user to interact with the engine. Games will load canvases into the 
 * array member, and also implement the update() function which will be called on game loop.
 * 
 * A singleton GameState will be made on program run, and will be initiated with a user defined
 * init function, where canvas will be hooked into the array and the update function will be
 * defined
 *
 * NOTE: This singleton GameState is stored in the GLFW window user pointer at all times
 * 	 for use in the GLFW callback functions
 *
 * ===
*/
struct GameState {
	// NOTE: arrays will always stay sorted by their IDs
	struct CoordinateCanvas* canvasRenderingArray[GRID_GAME_MAX_CANVAS_AMT];
	int (*update)(struct GameState*);

	// SHOULD NOT BE MODIFIED BY THE USER
	struct {
		GLFWwindow* window;
		int numCanvases;

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
		
	} gameInfo;

	// these values are modified every frame of the game loop function
	struct {
		double currentTime;
		double previousTime;
		double deltaTime;
		double FPS;
	} timeData;

	struct InputData inputData;
};

struct GameState gameState(GLFWwindow* window, const ShaderProgram canvas, const ShaderProgram border);

void initGameState(struct GameState* const game); // user defined function

void gameStateUpdateTime(struct GameState* const game);

// NULL IF NOT FOUND
struct CoordinateCanvas* gameStateGetCanvas(const struct GameState* const game, const uint id);

// this function will allocate the canvas in the heap and store it in the array
// returns pointer to canvas in the array, NULL if canvas array full
struct CoordinateCanvas* gameStateAddCopyOfCanvas(struct GameState* const game, const struct CoordinateCanvas canvas);

// this function will destroy the canvas, free the allocated memory for the canvas, and remove the pointer from the array
// return 1 on success, 0 if canvas not found 
int gameStateRemoveCanvas(struct GameState* const game, const uint id);

// called as the final drawing call in the game loop
void gameStateDraw(struct GameState* const game);

// setting function to ensure the correct program is in use. NOTE: will change the program back to what is originally was if changed
void gameStateSetGridUnitCntUniform(struct GameState* const game, const unsigned int xCnt, const unsigned int yCnt);
void gameStateSetBorderColorUniform(struct GameState* const game, const Vec3 color);
void gameStateSetCanvasBottomLeftCoordsUniform(struct GameState* const game, const Vec2 coordsInNDC);

// WARN: USE THIS FUNCTION INSTEAD TO UPDATE THE CURRENTLY ACTIVE PROGRAM
void gameStateUseProgram(struct GameState* const game, ShaderProgram program);

// removes every canvas in the rendering array and destroys every shader program
// NOTE: a complete destroying of the engine is done in a seperate encapsulating functions which also calls gameStateDestroy 
void gameStateDestory(struct GameState* game);

#endif // GRID_GAME_STATE_H
