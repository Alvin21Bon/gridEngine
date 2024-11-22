#pragma once

#include "game/game-object-array.h"
#include "canvas/canvas-array.h"
#include "utility/glfw/input-data.h"
#include "utility/glfw/time-data.h"

enum GridEngineStates;
struct GridEngine;

struct GameState {
	struct CanvasArray canvasArray;
	struct GameObjectArray gameObjectArray;
	struct InputData input;
	struct InputData previousInput;
	struct TimeData time;

	enum GridEngineStates (*preUpdate)(struct GridEngine* const);
	enum GridEngineStates (*postUpdate)(struct GridEngine* const);
};

struct GameState gameState();
void gameStateDestroy(struct GameState* const gameState);

enum GridEngineStates gridEngineUpdateGameState(struct GridEngine* const engine);
void gridEngineAttachPreUpdateFunction(struct GridEngine* engine, enum GridEngineStates (*preUpdateFunction)(struct GridEngine* const));
void gridEngineAttachPostUpdateFunction(struct GridEngine* engine, enum GridEngineStates (*postUpdateFunction)(struct GridEngine* const));

