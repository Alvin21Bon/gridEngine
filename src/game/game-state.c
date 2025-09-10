// #include "game/game-state.h"
//
// #include "canvas/canvas-array.h"
// #include "game/game-object-array.h"
// #include "game/game-object.h"
// #include "engine/grid-engine-states.h"
// #include "engine/grid-engine.h"
// #include "utility/glfw/time-data.h"
// #include "utility/glfw/input-data.h"
// #include "utility/logging.h"
// #include "glfw.h"
//
// static enum GridEngineStates defaultPreAndPostUpdateFunction(struct GridEngine* const engine) {return GRID_ENGINE_RUNNING;}
//
// struct GameState gameState()
// {
// 	struct GameState gameState;
// 	gameState.canvasArray = canvasArray();
// 	gameState.gameObjectArray = gameObjectArray();
// 	gameState.time = timeData();
// 	gameState.input = inputData();
// 	gameState.previousInput = inputData();
//
// 	gameState.preUpdate = defaultPreAndPostUpdateFunction;
// 	gameState.postUpdate = defaultPreAndPostUpdateFunction;
//
// 	return gameState;
// }
//
// void gameStateDestroy(struct GameState* const gameState)
// {
// 	LOG(GRID_LOGGING_FULL, __func__, __LINE__, "Destroying Game State...\n");
// 	LOG(GRID_LOGGING_FULL, __func__, __LINE__, "Destorying Canvas Array...\n");
// 	canvasArrayDestroy(&gameState->canvasArray);
//
// 	LOG(GRID_LOGGING_FULL, __func__, __LINE__, "Destorying Game Object Array...\n");
// 	gameObjectArrayDestroy(&gameState->gameObjectArray);
// }
//
// enum GridEngineStates gridEngineUpdateGameState(struct GridEngine* const engine)
// {
// 	struct GameState* gameState = &engine->gameState;
//
// 	timeDataUpdate(&gameState->time);
// 	gameState->previousInput = gameState->input;
// 	glfwPollEvents(); // updates the input member in the gameState
//
// 	// used to capture any GRID_ENGINE_PAUSED return value, which will wait for the entire gameStateUpdate to be finished before returning the need for engine to pause
// 	bool shouldEnginePause = false;
//
// 	// PRE-UPDATE
// 	enum GridEngineStates updateFunctionReturnedState;
// 	updateFunctionReturnedState = gameState->preUpdate(engine);
//
// 	switch (updateFunctionReturnedState)
// 	{
// 		case GRID_ENGINE_RUNNING:
// 			break;
// 		case GRID_ENGINE_PAUSED:
// 			shouldEnginePause = true;
// 			break;
//
// 		case GRID_ENGINE_ERROR:
// 			LOG(GRID_LOGGING_ERROR, __func__, __LINE__, "pre-update error\n");
// 		case GRID_ENGINE_SUCCESS:
// 			return updateFunctionReturnedState;
//
// 		default:
// 			LOG(GRID_LOGGING_WARN, __func__,  __LINE__, "returned value must be of enum GridEngineStates\n");
// 	}
//
// 	// PERFORM ALL GAME OBJECT UPDATE AND DRAW FUNCTIONS
// 	struct GameObject* object;
// 	struct CanvasArray canvasesToDrawOn;
// 	struct CoordinateCanvas* canvas;
// 	for (int idx = 0; idx < gameState->gameObjectArray.num; idx++)
// 	{
// 		object = gameState->gameObjectArray.elements[idx];
//
// 		// object update function
// 		updateFunctionReturnedState = object->update(object, engine);
// 		switch (updateFunctionReturnedState)
// 		{
// 			case GRID_ENGINE_RUNNING:
// 				break;
// 			case GRID_ENGINE_PAUSED:
// 				shouldEnginePause = true;
// 				break;
//
// 			case GRID_ENGINE_ERROR:
// 				LOG(GRID_LOGGING_ERROR, __func__, __LINE__, "object (%s) update error\n", object->id);
// 			case GRID_ENGINE_SUCCESS:
// 				return updateFunctionReturnedState;
//
// 			default:
// 				LOG(GRID_LOGGING_WARN, __func__, __LINE__, "object (%s) update not of enum GridEngineStates\n", object->id);
// 		}
//
// 		// object draw (on canvases) function
// 		canvasesToDrawOn = canvasArrayGet(&gameState->canvasArray, object->canvasId);
// 		if (canvasesToDrawOn.num == 0)
// 			LOG(GRID_LOGGING_WARN, __func__, __LINE__, "object (%s) is not linked to a valid canvas (%s)\n", object->id, object->canvasId);
//
// 		for (int idx = 0; idx < canvasesToDrawOn.num; idx++)
// 		{
// 			canvas = canvasesToDrawOn.elements[idx];
// 			object->draw(object, canvas);
// 		}
// 	}
//
// 	// POST-UPDATE
// 	updateFunctionReturnedState = gameState->postUpdate(engine);
// 	switch (updateFunctionReturnedState)
// 	{
// 		case GRID_ENGINE_RUNNING:
// 			break;
// 		case GRID_ENGINE_PAUSED:
// 			shouldEnginePause = true;
// 			break;
//
// 		case GRID_ENGINE_ERROR:
// 			LOG(GRID_LOGGING_ERROR, __func__, __LINE__, "post-update error\n");
// 		case GRID_ENGINE_SUCCESS:
// 			return updateFunctionReturnedState;
//
// 		default:
// 			LOG(GRID_LOGGING_WARN, __func__, __LINE__, "post-update return value not of enum GridEngineStates\n");
// 	}
//
// 	// GAME STATE IS NOW FULLY UPDATED. 
// 	return shouldEnginePause ? GRID_ENGINE_PAUSED : GRID_ENGINE_RUNNING;
// }
//
// void gridEngineAttachPreUpdateFunction(struct GridEngine* engine, enum GridEngineStates (*preUpdateFunction)(struct GridEngine* const))
// 	{engine->gameState.preUpdate = preUpdateFunction;}
// void gridEngineAttachPostUpdateFunction(struct GridEngine* engine, enum GridEngineStates (*postUpdateFunction)(struct GridEngine* const))
// 	{engine->gameState.postUpdate = postUpdateFunction;}
