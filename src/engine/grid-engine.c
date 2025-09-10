#include "engine/grid-engine.h"

#include "canvas/canvas-array.h"
#include "game/game-object-array.h"
#include "utility/glfw/window.h"
#include "utility/logging.h"
#include "engine/grid-engine-states.h"

static enum GridEngineStates defaultPreTick(struct GridEngine* const engine) { return GRID_ENGINE_RUNNING; }
static enum GridEngineStates defaultPostTick(struct GridEngine* const engine) { return GRID_ENGINE_RUNNING; }
static enum GridEngineStates defaultPaused(struct GridEngine* const engine)
{
	if (inputDataIsKeyDown(&engine->window.input, GLFW_KEY_ESCAPE)) 
		return GRID_ENGINE_RUNNING;

	return GRID_ENGINE_PAUSED;
}

struct GridEngine gridEngine()
{
	struct GridEngine gridEngine;
	gridEngine.window = gridWindow();
	gridEngine.canvasArray = canvasArray();
	gridEngine.gameObjectArray = gameObjectArray();

	gridEngine.preTick = defaultPreTick;
	gridEngine.postTick = defaultPostTick;
	gridEngine.pausedStateFunction = defaultPaused;

	gridEngine.state = GRID_ENGINE_RUNNING;

	initEngine(&gridEngine); // all user-level procedures
	return gridEngine;
}

void gridEngineAttachPreTickFunction(struct GridEngine* engine, enum GridEngineStates (*preTickFunction)(struct GridEngine* const))
{
	engine->preTick = preTickFunction;
}
void gridEngineAttachPostTickFunction(struct GridEngine* engine, enum GridEngineStates (*postTickFunction)(struct GridEngine* const))
{
	engine->postTick = postTickFunction;
}
void gridEngineAttachPausedStateFunction(struct GridEngine* const engine, enum GridEngineStates (*pausedStateFunction)(struct GridEngine* const))
{
	engine->pausedStateFunction = pausedStateFunction;
}

void gridEngineDestroy(struct GridEngine* engine)
{
	LOG(GRID_LOGGING_FULL, __func__, __LINE__, "Terminating engine...\n");
	gridWindowDestroy(&engine->window);
	canvasArrayDestroy(&engine->canvasArray);
	gameObjectArrayDestroy(&engine->gameObjectArray);

	terminateEngine(engine);
}

