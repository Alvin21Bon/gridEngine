#include "engine/grid-engine.h"

#include "canvas/canvas-array.h"
#include "game/game-object-array.h"
#include "game/game-object.h"
#include "utility/glfw/window.h"
#include "utility/logging.h"
#include "utility/opengl/canvas-rendering.h"
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

	initCanvasRendering();

	gridEngine.state = GRID_ENGINE_RUNNING;

	LOG(GRID_LOGGING_FULL, __func__, __LINE__, "running user-defined engine initation function...\n");
	initEngine(&gridEngine); // all user-level procedures
	
	return gridEngine;
}

enum GridEngineStates engineTick(struct GridEngine* const engine)
{
	gridWindowUpdate(&engine->window);

	if (engine->state == GRID_ENGINE_PAUSED)
	{
		return engine->pausedStateFunction(engine);
	}

	// used to capture any GRID_ENGINE_PAUSED return value, which will wait for the entire gameStateUpdate to be finished before returning the need for engine to pause
	bool shouldEnginePause = false;
	enum GridEngineStates returnedEngineState;
	
	// PRE-TICK
	returnedEngineState = engine->preTick(engine);
	switch (returnedEngineState)
	{
		case GRID_ENGINE_RUNNING:
			break;
		case GRID_ENGINE_PAUSED:
			shouldEnginePause = true;
			LOG(GRID_LOGGING_FULL, __func__, __LINE__, "engine pause event triggered by pre-tick\n");
			break;

		case GRID_ENGINE_ERROR:
			LOG(GRID_LOGGING_ERROR, __func__, __LINE__, "pre-tick error\n");
		case GRID_ENGINE_SUCCESS:
			return returnedEngineState;

		default:
			LOG(GRID_LOGGING_WARN, __func__,  __LINE__, "returned value must be of enum GridEngineStates\n");
	}

	// PERFORM ALL GAME OBJECT UPDATE AND DRAW FUNCTIONS
	// NOTE: all updates are processed, THEN all draws are called to make sure visual state is accurate
	struct GameObject* object;
	for (int idx = 0; idx < engine->gameObjectArray.num; idx++)
	{
		object = engine->gameObjectArray.elements[idx];

		// object update function
		returnedEngineState = object->update(object, engine);
		switch (returnedEngineState)
		{
			case GRID_ENGINE_RUNNING:
				break;
			case GRID_ENGINE_PAUSED:
				LOG(GRID_LOGGING_FULL, __func__, __LINE__, "engine pause event triggered by object %s\n", object->id);
				shouldEnginePause = true;
				break;

			case GRID_ENGINE_ERROR:
				LOG(GRID_LOGGING_ERROR, __func__, __LINE__, "object (%s) update error\n", object->id);
			case GRID_ENGINE_SUCCESS:
				return returnedEngineState;

			default:
				LOG(GRID_LOGGING_WARN, __func__, __LINE__, "object (%s) update not of enum GridEngineStates\n", object->id);
		}
	}

	struct CanvasArray canvasesToDrawOn;
	struct CoordinateCanvas* canvas;
	for (int idx = 0; idx < engine->gameObjectArray.num; idx++)
	{
		object = engine->gameObjectArray.elements[idx];

		// object draw (on canvases) function
		canvasesToDrawOn = canvasArrayGet(&engine->canvasArray, object->canvasId);
		for (int idx = 0; idx < canvasesToDrawOn.num; idx++)
		{
			canvas = canvasesToDrawOn.elements[idx];
			object->draw(object, canvas);
		}
	}

	// POST-TICK
	returnedEngineState = engine->postTick(engine);
	switch (returnedEngineState)
	{
		case GRID_ENGINE_RUNNING:
			break;
		case GRID_ENGINE_PAUSED:
			LOG(GRID_LOGGING_FULL, __func__, __LINE__, "engine pause event triggered by post-tick\n");
			shouldEnginePause = true;
			break;

		case GRID_ENGINE_ERROR:
			LOG(GRID_LOGGING_ERROR, __func__, __LINE__, "post-tick error\n");
		case GRID_ENGINE_SUCCESS:
			return returnedEngineState;

		default:
			LOG(GRID_LOGGING_WARN, __func__, __LINE__, "post-update return value not of enum GridEngineStates\n");
	}

	return shouldEnginePause ? GRID_ENGINE_PAUSED : GRID_ENGINE_RUNNING;
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
	gameObjectArrayDestroy(&engine->gameObjectArray, engine);
	terminateCanvasRendering();

	LOG(GRID_LOGGING_FULL, __func__, __LINE__, "running user-defined engine terminate function...\n");
	terminateEngine(engine);
}

#pragma weak terminateEngine
void terminateEngine(struct GridEngine* engine) { return; }

