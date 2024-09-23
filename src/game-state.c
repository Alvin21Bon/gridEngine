#include "../include/grid-engine.h"

int defaultFunctionForPreAndPostUpdate(struct GameState* game) {return GRID_ENGINE_SUCCESS;}

struct GameState gameState(GLFWwindow* window, const ShaderProgram canvas, const ShaderProgram border)
{
	struct GameState gameState;
	gameState.gameInfo.numCanvases = 0;
	gameState.gameInfo.numObjects = 0;
	gameState.gameInfo.window = window;
	gameState.gameInfo.programs.canvas = canvas;
	gameState.gameInfo.programs.border = border;

	gameState.gameInfo.uniforms.canvasGridUnitCnt = glGetUniformLocation(canvas, "gridUnitCnt");
	gameState.gameInfo.uniforms.borderColor = glGetUniformLocation(border, "borderColor");
	gameState.gameInfo.uniforms.canvasBottomLeftCoordsInNDC = glGetUniformLocation(border, "aCanvasBottomLeftCoordsInNDC");

	gameState.gameInfo.cursors.crosshair = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
	gameState.gameInfo.cursors.hResize = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
	gameState.gameInfo.cursors.vResize = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);

	gameState.timeData.currentTime = glfwGetTime();
	gameState.timeData.previousTime = gameState.timeData.currentTime;
	gameState.timeData.deltaTime = 0;
	gameState.timeData.FPS = GRID_GAME_MAX_FPS;

	gameState.inputData = inputData();
	gameState.previousInputData = inputData();
	gameState.targetCanvasForMouseInteractions = NULL;

	gameState.preUpdate = defaultFunctionForPreAndPostUpdate;
	gameState.postUpdate = defaultFunctionForPreAndPostUpdate;

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

void gameStateAttachPreUpdateFunction(struct GameState* game, int (*preUpdateFunction)(struct GameState*))
{
	game->preUpdate = preUpdateFunction;
}
void gameStateAttachPostUpdateFunction(struct GameState* game, int (*postUpdateFunction)(struct GameState*))
{
	game->postUpdate = postUpdateFunction;
}

// BINARY SEARCH FUNCTIONS FOR THE ARRAYS ====================================
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
static int gameStateBinarySearchObjectArray(const struct GameState* const game, const uint id, int* sliceIdxFinalValue)
{
	int sliceIdx = 0;
	uint testID;

	int startIdx = 0;
	int endIdx = game->gameInfo.numObjects - 1;
	while (startIdx <= endIdx)
	{
		sliceIdx = (startIdx + endIdx) / 2;
		testID = game->objectArray[sliceIdx]->id;

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

// =========================================================================================
// ARRAY ADDING, REMOVING, AND QUERYING FUNCTIONS

struct CoordinateCanvas* gameStateGetCanvas(const struct GameState* const game, const uint id)
{
	int idx = gameStateBinarySearchCanvasArray(game, id, NULL);
	if (idx == -1) return NULL;

	return game->canvasRenderingArray[idx];
}
struct GameObject* gameStateGetObject(const struct GameState* const game, const uint id)
{
	int idx = gameStateBinarySearchObjectArray(game, id, NULL);
	if (idx == -1) return NULL;

	// it is up to the user to know what child game object this is
	return game->objectArray[idx];
}

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
struct GameObject* gameStateAddCopyOfObject(struct GameState* const game, const struct GameObject* const object, const size_t sizeOfGameObject)
{
	if (game->gameInfo.numObjects == GRID_GAME_MAX_GAME_OBJECTS_AMT) return NULL;

	// this will copy over all the data of whatever derived game object the user has to the heap
	struct GameObject* objectOnHeap = malloc(sizeOfGameObject);
	memcpy(objectOnHeap, object, sizeOfGameObject);

	// add object such that array stays sorted on ID
	if (game->gameInfo.numObjects != 0)
	{
		int idxOfClosestIdFound;
		int idxToAddObjectTo;

		gameStateBinarySearchObjectArray(game, objectOnHeap->id, &idxOfClosestIdFound);
		if (game->objectArray[idxOfClosestIdFound]->id <= objectOnHeap->id) 
			idxToAddObjectTo = idxOfClosestIdFound + 1;
		else
			idxToAddObjectTo = idxOfClosestIdFound; 

		// all elements get shifted right so object can be inserted where it needs to be
		for (int idx = game->gameInfo.numObjects - 1; idx >= idxToAddObjectTo; idx--)
		{
			game->objectArray[idx + 1]  = game->objectArray[idx];
		}

		game->objectArray[idxToAddObjectTo] = objectOnHeap;
	}
	else game->objectArray[0] = objectOnHeap;
	
	game->gameInfo.numObjects++;
	return objectOnHeap;
}

int gameStateRemoveCanvas(struct GameState* const game, const uint id)
{
	int idxOfCanvasToRemove = gameStateBinarySearchCanvasArray(game, id, NULL);
	if (idxOfCanvasToRemove == -1) return 0;

	struct CoordinateCanvas* canvasToRemove = game->canvasRenderingArray[idxOfCanvasToRemove];
	canvasDestroy(canvasToRemove);
	free(canvasToRemove);

	// avoid dangling pointer
	if (canvasToRemove == game->targetCanvasForMouseInteractions) game->targetCanvasForMouseInteractions = NULL;
	
	for (int idx = idxOfCanvasToRemove; idx < game->gameInfo.numCanvases - 1; idx++)
	{
		// shift all canvases after removed canvas to the left
		game->canvasRenderingArray[idx] = game->canvasRenderingArray[idx + 1];
	}
	game->gameInfo.numCanvases--;

	return 1;
}
int gameStateRemoveObject(struct GameState* const game, const uint id)
{
	int idxOfObjectToRemove = gameStateBinarySearchObjectArray(game, id, NULL);
	if (idxOfObjectToRemove == -1) return 0;

	struct GameObject* objectToRemove = game->objectArray[idxOfObjectToRemove];
	objectToRemove->destroy(objectToRemove);  
	free(objectToRemove);

	for (int idx = idxOfObjectToRemove; idx < game->gameInfo.numObjects; idx++)
	{
		// shift all objects after removed canvas to the left
		game->objectArray[idx] = game->objectArray[idx + 1];
	}
	game->gameInfo.numObjects--;

	return 1;
}

// ARRAY ADDING, REMOVING, AND QUERYING FUNCTIONS
// =========================================================================================

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
void gameStateSetCursor(struct GameState* const game, GLFWcursor* cursor)
{
	glfwSetCursor(game->gameInfo.window, cursor);
	game->gameInfo.cursors.currentlyActive = cursor;
}

void gameStateDestroy(struct GameState* game)
{
	printf("Destroying Game State...\n");
	while (game->gameInfo.numCanvases > 0)
	{
		printf("Removed canvas with ID of %u, NumCanvases: %d\n", game->canvasRenderingArray[0]->id, game->gameInfo.numCanvases - 1);
		gameStateRemoveCanvas(game, game->canvasRenderingArray[0]->id);
	}
	while (game->gameInfo.numObjects > 0)
	{
		printf("Removed game object with ID of %u, NumObjects: %d\n", game->objectArray[0]->id, game->gameInfo.numObjects - 1);
		gameStateRemoveObject(game, game->objectArray[0]->id);
	}

	glfwDestroyCursor(game->gameInfo.cursors.crosshair);
	glfwDestroyCursor(game->gameInfo.cursors.hResize);
	glfwDestroyCursor(game->gameInfo.cursors.vResize);
	game->gameInfo.cursors.currentlyActive = NULL;

	glDeleteProgram(game->gameInfo.programs.canvas);
	glDeleteProgram(game->gameInfo.programs.border);
	game->gameInfo.programs.currentlyActive = 0;
}
