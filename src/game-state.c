#include "../include/grid-engine.h"

struct GameState gameState(GLFWwindow* window)
{
	struct GameState gameState;
	gameState.numCanvases = 0;
	gameState.window = window;

	return gameState;
}

struct CoordinateCanvas* gameStateGetCanvas(const struct GameState* const game, const char* const id)
{
	for (int idx = 0; idx < game->numCanvases; idx++)
	{
		struct CoordinateCanvas* canvas = game->canvasRenderingArray[idx];
		if (strcmp(canvas->id, id) == 0)
		{
			return canvas;
		}
	}

	return NULL;
}
// this is the same as the usual function but also sets the parameter to the idx of the canvas in the array
struct CoordinateCanvas* gameStateGetCanvasAndIdx(const struct GameState* const game, const char* const id, int* idxOfCanvas)
{
	for (int idx = 0; idx < game->numCanvases; idx++)
	{
		struct CoordinateCanvas* canvas = game->canvasRenderingArray[idx];
		if (strcmp(canvas->id, id) == 0)
		{
			*idxOfCanvas = idx;
			return canvas;
		}
	}

	idxOfCanvas = NULL;
	return NULL;
}

// returns copy, NULL if canvas array full
struct GameState* gameStateAddCanvas(struct GameState* const game, const struct CoordinateCanvas* canvas)
{
	if (game->numCanvases == GRID_GAME_MAX_CANVAS_AMT) return NULL;
	
	game->canvasRenderingArray[game->numCanvases] = canvas;
	game->numCanvases++;

	return game;
}

// return canvas that was removed, NULL if "id" not found in array
struct CoordinateCanvas* gameStateRemoveCanvas(struct GameState* const game, const char* const id)
{
	int idxOfRemovedCanvas;
	struct CoordinateCanvas* removedCanvas;

	removedCanvas = gameStateGetCanvasAndIdx(game, id, &idxOfRemovedCanvas);
	if (removedCanvas == NULL) return NULL;
	
	for (int idx = idxOfRemovedCanvas; idx < game->numCanvases - 1; idx++)
	{
		// shift all canvases after removed canvas to the left
		game->canvasRenderingArray[idx] = game->canvasRenderingArray[idx + 1];
	}
	game->numCanvases--;

	return removedCanvas;
}

// called as the final drawing call in the game loop
void gameStateDraw(struct GameState* game)
{
	for (int idx = 0; idx < game->numCanvases; idx++)
	{
		canvasDraw(game->canvasRenderingArray[idx]);
	}
}
