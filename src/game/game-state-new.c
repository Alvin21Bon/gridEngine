#include "../../include/engine/grid-engine.h"
#include "../../include/game/game-state-new.h"

struct GameState1 gameState1()
{
	struct GameState1 gameState;
	gameState.canvasArray = canvasArray();
	gameState.gameObjectArray = gameObjectArray();
	gameState.time = timeData();
	gameState.input = inputData();

	return gameState;
}

void gameState1Destroy(struct GameState1* const gameState)
{
	printf("Destroying Game State...\n");
	printf("Destroying CANVAS ARRAY\n");
	canvasArrayDestroy(&gameState->canvasArray);

	printf("Destroying GAME OBJECT ARRAY\n");
	gameObjectArrayDestroy(&gameState->gameObjectArray);
}
