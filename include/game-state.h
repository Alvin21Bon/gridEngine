#ifndef GRID_GAME_STATE_H
#define GRID_GAME_STATE_H

#include "coordinate-canvas.h"
#include "preprocessor.h"

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
	struct CoordinateCanvas canvasRenderingArray[GRID_GAME_MAX_CANVAS_AMT];
	int numCanvases;
	void (*update)(struct GameState*);

	// SHOULD NOT BE ACCESSED OR SET BY THE USER
	GLFWwindow* window;
};

void initGameState(struct GameState* const game); // user defined function

// NULL IF NOT FOUND
struct CoordinateCanvas* gameStateGetCanvas(const struct GameState* const game, const char* const id);
// this is the same as the usual function but also sets the parameter to the idx of the canvas in the array
// NULL IF NOT FOUND
struct CoordinateCanvas* gameStateGetCanvasAndIdx(const struct GameState* const game, const char* const id, int* idxOfCanvas);

// returns copy, NULL if canvas array full
struct GameState* gameStateAddCanvas(struct GameState* const game, const struct CoordinateCanvas canvas);

// return canvas that was removed, NULL if "id" not found in array
struct CoordinateCanvas* gameStateRemoveCanvas(struct GameState* const game, const char* const id);

// called as the final drawing call in the game loop
void gameStateDraw(struct GameState* game);

#endif // GRID_GAME_STATE_H
