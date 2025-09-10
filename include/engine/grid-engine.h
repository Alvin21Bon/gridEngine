#pragma once

#include "engine/grid-engine-states.h"
#include "canvas/canvas-array.h"
#include "game/game-object-array.h"
#include "utility/glfw/window.h"

/*
 * A singleton instance of this struct is made on initialization of the engine. This struct basically embodies the entire
 * running session of the engine. APIs to the engine are provided to the user, since none of the GridEngine
 * struct definition is visible to the user
 *
 * The user can edit engine configurations in the user defined engine init function.
*/
struct GridEngine {
	enum GridEngineStates state;
	struct CanvasArray canvasArray;
	struct GameObjectArray gameObjectArray;

	struct GridWindow window;

	// user defined function loop functions
	enum GridEngineStates (*preTick)(struct GridEngine* const);
	enum GridEngineStates(*postTick)(struct GridEngine* const);
	enum GridEngineStates (*pausedStateFunction)(struct GridEngine* const);
};

// user defined init and terminate functions
void initEngine(struct GridEngine* const engine);
void terminateEngine(struct GridEngine* engine);

struct GridEngine gridEngine();
void gridEngineDestroy(struct GridEngine* engine);

// attach user defined loop functions
void gridEngineAttachPreTickFunction(struct GridEngine* engine, enum GridEngineStates (*preTickFunction)(struct GridEngine* const));
void gridEngineAttachPostTickFunction(struct GridEngine* engine, enum GridEngineStates (*postTickFunction)(struct GridEngine* const));
void gridEngineAttachPausedStateFunction(struct GridEngine* const engine, enum GridEngineStates (*pausedStateFunction)(struct GridEngine* const));

