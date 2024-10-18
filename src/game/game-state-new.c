#include "game/game-state-new.h"

#include "engine/grid-engine-states.h"

static bool defaultPreAndPostUpdateFunction(struct GameState1* gameState) {return GRID_ENGINE_SUCCESS;}

struct GameState1 gameState1()
{
	struct GameState1 gameState;
	gameState.canvasArray = canvasArray();
	gameState.gameObjectArray = gameObjectArray();
	gameState.time = timeData();
	gameState.input = inputData();

	gameState.preUpdate = defaultPreAndPostUpdateFunction;
	gameState.postUpdate = defaultPreAndPostUpdateFunction;

	return gameState;
}

void gameState1AttachPreUpdateFunction(struct GameState1* gameState, bool (*preUpdateFunction)(struct GameState1*))
	{gameState->preUpdate = preUpdateFunction;}
void gameState1AttachPostUpdateFunction(struct GameState1* gameState, bool (*postUpdateFunction)(struct GameState1*))
	{gameState->postUpdate = postUpdateFunction;}

void gameState1Destroy(struct GameState1* const gameState)
{
	printf("Destroying Game State...\n");
	printf("Destroying CANVAS ARRAY\n");
	canvasArrayDestroy(&gameState->canvasArray);

	printf("Destroying GAME OBJECT ARRAY\n");
	gameObjectArrayDestroy(&gameState->gameObjectArray);
}
