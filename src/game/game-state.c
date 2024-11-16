#include "game/game-state.h"

#include "engine/grid-engine-states.h"

static bool defaultPreAndPostUpdateFunction(struct GameState* gameState) {return GRID_ENGINE_SUCCESS;}

struct GameState gameState()
{
	struct GameState gameState;
	gameState.canvasArray = canvasArray();
	gameState.gameObjectArray = gameObjectArray();
	gameState.time = timeData();
	gameState.input = inputData();

	gameState.preUpdate = defaultPreAndPostUpdateFunction;
	gameState.postUpdate = defaultPreAndPostUpdateFunction;

	return gameState;
}

void gameStateAttachPreUpdateFunction(struct GameState* gameState, bool (*preUpdateFunction)(struct GameState*))
	{gameState->preUpdate = preUpdateFunction;}
void gameStateAttachPostUpdateFunction(struct GameState* gameState, bool (*postUpdateFunction)(struct GameState*))
	{gameState->postUpdate = postUpdateFunction;}

void gameStateDestroy(struct GameState* const gameState)
{
	printf("Destroying Game State...\n");
	printf("Destroying CANVAS ARRAY\n");
	canvasArrayDestroy(&gameState->canvasArray);

	printf("Destroying GAME OBJECT ARRAY\n");
	gameObjectArrayDestroy(&gameState->gameObjectArray);
}
