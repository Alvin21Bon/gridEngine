#include "../include/grid-engine.h"

struct GameState gameState(GLFWwindow* window, const ShaderProgram canvas, const ShaderProgram border)
{
	struct GameState gameState;
	gameState.gameInfo.numCanvases = 0;
	gameState.gameInfo.window = window;
	gameState.gameInfo.programs.canvas = canvas;
	gameState.gameInfo.programs.border = border;

	gameState.gameInfo.uniforms.canvasGridUnitCnt = glGetUniformLocation(canvas, "gridUnitCnt");
	gameState.gameInfo.uniforms.borderColor = glGetUniformLocation(border, "borderColor");
	gameState.gameInfo.uniforms.canvasBottomLeftCoordsInNDC = glGetUniformLocation(border, "aCanvasBottomLeftCoordsInNDC");

	gameState.timeData.currentTime = glfwGetTime();
	gameState.timeData.previousTime = gameState.timeData.currentTime;
	gameState.timeData.deltaTime = 0;
	gameState.timeData.FPS = GRID_GAME_MAX_FPS;

	gameState.inputData = inputData();

	// this function is defined by the user
	initGame(&gameState);

	return gameState;
}

void gameStateUpdateTime(struct GameState* const game)
{
	game->timeData.previousTime = game->timeData.currentTime;
	game->timeData.currentTime = glfwGetTime();
	game->timeData.deltaTime = game->timeData.currentTime - game->timeData.previousTime;
	game->timeData.FPS = 1.0 / game->timeData.deltaTime;
}

// return -1 if not found
static int gameStateBinarySearchCanvasArray(const struct GameState* const game, const uint id, int* sliceIdxFinalValue)
{
	int sliceIdx = 0;
	uint testID;

	int startIdx = 0;
	int endIdx = game->gameInfo.numCanvases - 1;
	while (startIdx <= endIdx)
	{
		sliceIdx = (startIdx + endIdx) / 2;
		testID = game->canvasRenderingArray[sliceIdx]->id;

		if (testID < id) startIdx = sliceIdx + 1;
		else if (testID > id) endIdx = sliceIdx - 1;
		else 
		{
			// FOUND
			if (sliceIdxFinalValue != NULL) *sliceIdxFinalValue = sliceIdx;
			return sliceIdx;
		}
	}

	if (sliceIdxFinalValue != NULL) *sliceIdxFinalValue = sliceIdx;
	return -1;
}

struct CoordinateCanvas* gameStateGetCanvas(const struct GameState* const game, const uint id)
{
	int idx = gameStateBinarySearchCanvasArray(game, id, NULL);
	if (idx == -1) return NULL;

	return game->canvasRenderingArray[idx];
}

// this function will allocate the canvas in the heap and store it in the array
// returns pointer to canvas in the array, NULL if canvas array full
struct CoordinateCanvas* gameStateAddCopyOfCanvas(struct GameState* const game, const struct CoordinateCanvas canvas)
{
	if (game->gameInfo.numCanvases == GRID_GAME_MAX_CANVAS_AMT) return NULL;

	struct CoordinateCanvas* canvasOnHeap = malloc(sizeof(struct CoordinateCanvas));
	*canvasOnHeap = canvas;

	// add canvas such that array stays sorted on ID
	if (game->gameInfo.numCanvases != 0)
	{
		int idxOfClosestIdFound;
		int idxToAddCanvasTo;

		gameStateBinarySearchCanvasArray(game, canvasOnHeap->id, &idxOfClosestIdFound);
		if (game->canvasRenderingArray[idxOfClosestIdFound]->id <= canvasOnHeap->id) 
			idxToAddCanvasTo = idxOfClosestIdFound + 1;
		else
			idxToAddCanvasTo = idxOfClosestIdFound; 

		// all elements get shifted right so canvas can be inserted where it needs to be
		for (int idx = game->gameInfo.numCanvases - 1; idx >= idxToAddCanvasTo; idx--)
		{
			game->canvasRenderingArray[idx + 1]  = game->canvasRenderingArray[idx];
		}

		game->canvasRenderingArray[idxToAddCanvasTo] = canvasOnHeap;
	}
	else game->canvasRenderingArray[0] = canvasOnHeap;
	
	game->gameInfo.numCanvases++;
	return canvasOnHeap;
}

// this function will destroy the canvas, free the allocated memory for the canvas, and remove the pointer from the array
// return 1 on success, 0 if canvas not found 
int gameStateRemoveCanvas(struct GameState* const game, const uint id)
{
	int idxOfCanvasToRemove = gameStateBinarySearchCanvasArray(game, id, NULL);
	if (idxOfCanvasToRemove == -1) return 0;

	struct CoordinateCanvas* canvasToRemove = game->canvasRenderingArray[idxOfCanvasToRemove];
	canvasDestroy(canvasToRemove);
	free(canvasToRemove);
	
	for (int idx = idxOfCanvasToRemove; idx < game->gameInfo.numCanvases - 1; idx++)
	{
		// shift all canvases after removed canvas to the left
		game->canvasRenderingArray[idx] = game->canvasRenderingArray[idx + 1];
	}
	game->gameInfo.numCanvases--;

	return 1;
}

// called as the final drawing call in the game loop
void gameStateDraw(struct GameState* const game)
{
	for (int idx = 0; idx < game->gameInfo.numCanvases; idx++)
	{
		canvasDraw(game->canvasRenderingArray[idx], game);
	}
}

void gameStateSetGridUnitCntUniform(struct GameState* const game, const unsigned int xCnt, const unsigned int yCnt)
{
	ShaderProgram originalProgram = game->gameInfo.programs.currentlyActive;

	gameStateUseProgram(game, game->gameInfo.programs.canvas);
		glUniform2ui(game->gameInfo.uniforms.canvasGridUnitCnt, xCnt, yCnt);
	gameStateUseProgram(game, originalProgram);
}
void gameStateSetBorderColorUniform(struct GameState* const game, const Vec3 color)
{
	ShaderProgram originalProgram = game->gameInfo.programs.currentlyActive;

	gameStateUseProgram(game, game->gameInfo.programs.border);
		glUniform3fv(game->gameInfo.uniforms.borderColor, 1, color.elements);
	gameStateUseProgram(game, originalProgram);
}
void gameStateSetCanvasBottomLeftCoordsUniform(struct GameState* const game, const Vec2 coordsInNDC)
{
	ShaderProgram originalProgram = game->gameInfo.programs.currentlyActive;

	gameStateUseProgram(game, game->gameInfo.programs.border);
		glUniform2fv(game->gameInfo.uniforms.canvasBottomLeftCoordsInNDC, 1, coordsInNDC.elements);
	gameStateUseProgram(game, originalProgram);
}

void gameStateUseProgram(struct GameState* const game, ShaderProgram program)
{
	glUseProgram(program);
	game->gameInfo.programs.currentlyActive = program;
}

void gameStateDestory(struct GameState* game)
{
	printf("Destorying Game State...\n");
	while (game->gameInfo.numCanvases > 0)
	{
		printf("Removed canvas with ID of %u, NumCanvases: %d\n", game->canvasRenderingArray[0]->id, game->gameInfo.numCanvases - 1);
		gameStateRemoveCanvas(game, game->canvasRenderingArray[0]->id);
	}

	glDeleteProgram(game->gameInfo.programs.canvas);
	glDeleteProgram(game->gameInfo.programs.border);
	game->gameInfo.programs.currentlyActive = 0;
}
