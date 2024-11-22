#include "game/game-state.h"

#include "canvas/canvas-array.h"
#include "game/game-object-array.h"
#include "game/game-object.h"
#include "engine/grid-engine-states.h"
#include "engine/grid-engine.h"
#include "utility/glfw/time-data.h"
#include "utility/glfw/input-data.h"
#include "glfw.h"
#include <stdio.h> // replace with logging thing later

static enum GridEngineStates defaultPreAndPostUpdateFunction(struct GridEngine* const engine) {return GRID_ENGINE_RUNNING;}

struct GameState gameState()
{
	struct GameState gameState;
	gameState.canvasArray = canvasArray();
	gameState.gameObjectArray = gameObjectArray();
	gameState.time = timeData();
	gameState.input = inputData();
	gameState.previousInput = inputData();

	gameState.preUpdate = defaultPreAndPostUpdateFunction;
	gameState.postUpdate = defaultPreAndPostUpdateFunction;

	return gameState;
}

void gameStateDestroy(struct GameState* const gameState)
{
	printf("Destroying Game State...\n");
	printf("Destroying CANVAS ARRAY\n");
	canvasArrayDestroy(&gameState->canvasArray);

	printf("Destroying GAME OBJECT ARRAY\n");
	gameObjectArrayDestroy(&gameState->gameObjectArray);
}

enum GridEngineStates gridEngineUpdateGameState(struct GridEngine* const engine)
{
	struct GameState* gameState = &engine->gameState;

	timeDataUpdate(&gameState->time);
	gameState->previousInput = gameState->input;
	glfwPollEvents(); // updates the input member in the gameState
	
	// used to capture any GRID_ENGINE_PAUSED return value, which will wait for the entire gameStateUpdate to be finished before returning the need for engine to pause
	bool shouldEnginePause = false;

	// PRE-UPDATE
	enum GridEngineStates updateFunctionReturnedState;
	updateFunctionReturnedState = gameState->preUpdate(engine);
	
	switch (updateFunctionReturnedState)
	{
		case GRID_ENGINE_RUNNING:
			break;
		case GRID_ENGINE_PAUSED:
			shouldEnginePause = true;
			break;

		default:
			printf("PRE UPDATE ERROR: returned value must be of enum GridEngineStates\n");
		case GRID_ENGINE_ERROR:
			printf("PRE UPDATE ERROR\n");
		case GRID_ENGINE_SUCCESS:
			return updateFunctionReturnedState;
	}

	// PERFORM ALL GAME OBJECT UPDATE AND DRAW FUNCTIONS
	struct GameObject* object;
	struct CanvasArray canvasesToDrawOn;
	struct CoordinateCanvas* canvas;
	for (int idx = 0; idx < gameState->gameObjectArray.num; idx++)
	{
		object = gameState->gameObjectArray.elements[idx];

		// object update function
		updateFunctionReturnedState = object->update(object, engine);
		switch (updateFunctionReturnedState)
		{
			case GRID_ENGINE_RUNNING:
				break;
			case GRID_ENGINE_PAUSED:
				shouldEnginePause = true;
				break;

			default:
				printf("OBJECT (%s) UPDATE ERROR: returned value must be of enum GridEngineStates\n", object->id);
			case GRID_ENGINE_ERROR:
				printf("OBJECT (%s) UPDATE ERROR\n", object->id);
			case GRID_ENGINE_SUCCESS:
				return updateFunctionReturnedState;
		}

		// object draw (on canvases) function
		canvasesToDrawOn = canvasArrayGet(&gameState->canvasArray, object->canvasId);
		if (canvasesToDrawOn.num == 0)
			printf("OBJECT (%s) DRAW WARN: canvas (%s) is not in canvasArray\n", object->id, object->canvasId);

		for (int idx = 0; idx < canvasesToDrawOn.num; idx++)
		{
			canvas = canvasesToDrawOn.elements[idx];
			object->draw(object, canvas);
		}
	}

	// POST-UPDATE
	updateFunctionReturnedState = gameState->postUpdate(engine);
	switch (updateFunctionReturnedState)
	{
		case GRID_ENGINE_RUNNING:
			break;
		case GRID_ENGINE_PAUSED:
			shouldEnginePause = true;
			break;

		default:
			printf("POST UPDATE ERROR: returned value must be of enum GridEngineStates\n");
		case GRID_ENGINE_ERROR:
			printf("POST UPDATE ERROR\n");
		case GRID_ENGINE_SUCCESS:
			return updateFunctionReturnedState;
	}

	// GAME STATE IS NOW FULLY UPDATED. 
	return shouldEnginePause ? GRID_ENGINE_PAUSED : GRID_ENGINE_RUNNING;
}

void gridEngineAttachPreUpdateFunction(struct GridEngine* engine, enum GridEngineStates (*preUpdateFunction)(struct GridEngine* const))
	{engine->gameState.preUpdate = preUpdateFunction;}
void gridEngineAttachPostUpdateFunction(struct GridEngine* engine, enum GridEngineStates (*postUpdateFunction)(struct GridEngine* const))
	{engine->gameState.postUpdate = postUpdateFunction;}
