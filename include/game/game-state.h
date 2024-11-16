#pragma once

#include "game-object-array.h"
#include "canvas/canvas-array.h"
#include "utility/glfw/input-data.h"
#include "utility/glfw/time-data.h"

enum GridEngineStates;

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

	enum GridEngineStates (*preUpdate)(struct GameState* const);
	enum GridEngineStates (*postUpdate)(struct GameState* const);
};

struct GameState gameState();

void gameStateAttachPreUpdateFunction(struct GameState* gameState, enum GridEngineStates (*preUpdateFunction)(struct GameState* const));
void gameStateAttachPostUpdateFunction(struct GameState* gameState, enum GridEngineStates (*postUpdateFunction)(struct GameState* const));

void gameStateDestroy(struct GameState* const gameState);
