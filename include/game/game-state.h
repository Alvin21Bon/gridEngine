#pragma once

#include "game-object-array.h"
#include "canvas/canvas-array.h"
#include "utility/glfw/input-data.h"
#include "utility/glfw/time-data.h"

/* 
 * ===
 * GameState is for the user to interact with the engine. This is the interface in which user
 * can most directly develop the content of their programs.
 * 
 * A singleton GameState will be made on program run, and will be a member of a bigger 
 * GridEngine singleton that encompassess all engine functions. This GameState is the only
 * thing passed to the user for their program development. 
 * ===
*/
struct GameState {
	struct CanvasArray canvasArray;
	struct GameObjectArray gameObjectArray;
	struct InputData input;
	struct TimeData time;

	bool (*preUpdate)(struct GameState*);
	bool (*postUpdate)(struct GameState*);
};

struct GameState gameState();

void gameStateAttachPreUpdateFunction(struct GameState* gameState, bool (*preUpdateFunction)(struct GameState*));
void gameStateAttachPostUpdateFunction(struct GameState* gameState, bool (*postUpdateFunction)(struct GameState*));

void gameStateDestroy(struct GameState* const gameState);
