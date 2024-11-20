#include "game/game-state.h"

#include "canvas/canvas-array.h"
#include "game/game-object-array.h"
#include "engine/grid-engine-states.h"
#include "utility/glfw/time-data.h"
#include "utility/glfw/input-data.h"
#include "glfw.h"

static enum GridEngineStates defaultPreAndPostUpdateFunction(struct GameState* const gameState) {return GRID_ENGINE_RUNNING;}

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

enum GridEngineStates gameStateUpdate(struct GameState* const gameState)
{
	timeDataUpdate(&gameState->time);

	gameState->previousInput = gameState->input;
	glfwPollEvents(); // updates the input member in the gameState
	
	// PRE UPDATE
}

void gameStateDestroy(struct GameState* const gameState)
{
	printf("Destroying Game State...\n");
	printf("Destroying CANVAS ARRAY\n");
	canvasArrayDestroy(&gameState->canvasArray);

	printf("Destroying GAME OBJECT ARRAY\n");
	gameObjectArrayDestroy(&gameState->gameObjectArray);
}

void gameStateAttachPreUpdateFunction(struct GameState* gameState, enum GridEngineStates (*preUpdateFunction)(struct GameState* const))
	{gameState->preUpdate = preUpdateFunction;}
void gameStateAttachPostUpdateFunction(struct GameState* gameState, enum GridEngineStates (*postUpdateFunction)(struct GameState* const))
	{gameState->postUpdate = postUpdateFunction;}

