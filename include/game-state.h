#ifndef GRID_GAME_STATE_H
#define GRID_GAME_STATE_H

#include "shared.h"
#include "coordinate-canvas.h"

/* 
 * ===
 * GameState is for the user to interact with the engine. Games will load canvases into the 
 * array member, and also implement the update() function which will be called on game loop.
 * 
 * A singleton GameState will be made on program run, and will be initiated with a user defined
 * init function, where canvas will be hooked into the array and the update function will be
 * defined
 * ===
*/
struct GameState {
	struct CoordinateCanvas* canvasRenderingArray[GRID_GAME_MAX_CANVAS_AMT];
	void (*update)(struct GameState*);

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
		} uniforms;
		
	} gameInfo;
};

struct GameState gameState(GLFWwindow* window, const ShaderProgram canvas, const ShaderProgram border);

void initGameState(struct GameState* const game); // user defined function

// NULL IF NOT FOUND
struct CoordinateCanvas* gameStateGetCanvas(const struct GameState* const game, const char* const id);
// this is the same as the usual function but also sets the parameter to the idx of the canvas in the array
// NULL IF NOT FOUND
struct CoordinateCanvas* gameStateGetCanvasAndIdx(const struct GameState* const game, const char* const id, int* idxOfCanvas);

// returns copy, NULL if canvas array full
struct GameState* gameStateAddCanvas(struct GameState* const game, const struct CoordinateCanvas* canvas);

// return canvas that was removed, NULL if "id" not found in array
struct CoordinateCanvas* gameStateRemoveCanvas(struct GameState* const game, const char* const id);

// called as the final drawing call in the game loop
void gameStateDraw(struct GameState* game);

// setting function to ensure the correct program is in use. NOTE: will change the program back to what is originally was if changed
void gameStateSetGridUnitCntUniform(struct GameState* const game, const unsigned int xCnt, const unsigned int yCnt);

// WARN: USE THIS FUNCTION INSTEAD TO UPDATE THE CURRENTLY ACTIVE PROGRAM
void gameStateUseProgram(struct GameState* const game, ShaderProgram program);

#endif // GRID_GAME_STATE_H
