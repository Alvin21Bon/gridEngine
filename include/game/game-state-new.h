#pragma once

#include "game-object-array.h"
#include "../canvas/canvas-array.h"
#include "../utility/glfw/input-data.h"
#include "../utility/glfw/time-data.h"

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
struct GameState1 {
	struct CanvasArray canvasArray;
	struct GameObjectArray gameObjectArray;
	struct InputData input;
	struct TimeData time;

	bool (*preUpdate)(struct GameState1*);
	bool (*postUpdate)(struct GameState1*);
};

struct GameState1 gameState1();

void gameState1AttachPreUpdateFunction(struct GameState1* gameState, bool (*preUpdateFunction)(struct GameState1*));
void gameState1AttachPostUpdateFunction(struct GameState1* gameState, bool (*postUpdateFunction)(struct GameState1*));

void gameState1Destroy(struct GameState1* const gameState);
