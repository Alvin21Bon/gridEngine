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
};

void initGameState(struct GameState* const game); // user defined function

// returns copy, NULL if canvas array full
struct GameState* gameStateAddCanvas(struct GameState* const game, const struct CoordinateCanvas canvas);

// return copy, NULL if "id" not found in array
struct GameState* gameStateRemoveCanvas(struct GameState* const game, const char* const id);

#endif // GRID_GAME_STATE_H
