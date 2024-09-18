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
	initGameState(&gameState);

	return gameState;
}

void gameStateUpdateTime(struct GameState* const game, bool shouldPreviousTimeBeUpdated)
{
	if (shouldPreviousTimeBeUpdated) game->timeData.previousTime = game->timeData.currentTime;
	game->timeData.currentTime = glfwGetTime();
	game->timeData.deltaTime = game->timeData.currentTime - game->timeData.previousTime;
	game->timeData.FPS = 1.0 / game->timeData.deltaTime;
}

struct CoordinateCanvas* gameStateGetCanvas(const struct GameState* const game, const char* const id)
{
	for (int idx = 0; idx < game->gameInfo.numCanvases; idx++)
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
	for (int idx = 0; idx < game->gameInfo.numCanvases; idx++)
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
	if (game->gameInfo.numCanvases == GRID_GAME_MAX_CANVAS_AMT) return NULL;
	
	game->canvasRenderingArray[game->gameInfo.numCanvases] = canvas;
	game->gameInfo.numCanvases++;

	return game;
}

// return canvas that was removed, NULL if "id" not found in array
struct CoordinateCanvas* gameStateRemoveCanvas(struct GameState* const game, const char* const id)
{
	int idxOfRemovedCanvas;
	struct CoordinateCanvas* removedCanvas;

	removedCanvas = gameStateGetCanvasAndIdx(game, id, &idxOfRemovedCanvas);
	if (removedCanvas == NULL) return NULL;
	
	for (int idx = idxOfRemovedCanvas; idx < game->gameInfo.numCanvases - 1; idx++)
	{
		// shift all canvases after removed canvas to the left
		game->canvasRenderingArray[idx] = game->canvasRenderingArray[idx + 1];
	}
	game->gameInfo.numCanvases--;

	return removedCanvas;
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
	for (int idx = 0; idx < game->gameInfo.numCanvases; idx++)
	{
		canvasDestroy(game->canvasRenderingArray[idx]);
	}

	glDeleteProgram(game->gameInfo.programs.canvas);
	glDeleteProgram(game->gameInfo.programs.border);
	game->gameInfo.programs.currentlyActive = 0;
}
