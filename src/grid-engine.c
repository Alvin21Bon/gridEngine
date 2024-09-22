#include "../include/grid-engine.h"

GLFWwindow* gridEngineCreateAndSetupWindow()
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(1000, 1000, "hyprland:float", NULL, NULL);
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, keyCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetCursorPosCallback(window, cursorPosCallback);

	return window;
}

ShaderProgram gridEngineGenerateCanvasProgram()
{
	const char* vertexSource = 
		"#version 330 core\n"
		"layout (location = 0) in vec3 aPixelColor;\n"
		"layout (location = 1) in uint isPixelVisible;\n"
		"layout (location = 2) in vec2 vertexPosition;\n"
		"uniform uvec2 gridUnitCnt;\n"
		"out vec3 pixelColor;\n"
		"void main()\n"
		"{\n"
			"uint columnPos = uint(gl_InstanceID) / gridUnitCnt.y;\n"
			"uint rowPos = uint(gl_InstanceID) % gridUnitCnt.y;\n"
			"vec2 viewportBottomLeftCoords = vec2(-1, -1);\n"
			"float gridUnitWidthNDC = 2.0 / gridUnitCnt.x;\n"
			"float gridUnitHeightNDC = 2.0 / gridUnitCnt.y;\n"
			"vec2 gridOffsetAmount = vec2(columnPos * gridUnitWidthNDC, rowPos * gridUnitHeightNDC);\n"
			"vec2 translatedPosition = viewportBottomLeftCoords + gridOffsetAmount + vertexPosition;\n"
			"bool boolIsPixelVisible = bool(isPixelVisible);\n"
			"translatedPosition = translatedPosition + (int(!boolIsPixelVisible) * 100);\n"
			"gl_Position = vec4(translatedPosition, 0.0, 1.0);\n"
			"pixelColor = aPixelColor;\n"
		"}\0"
		;

	const char* fragmentSource = 
		"#version 330 core\n"
		"in vec3 pixelColor;\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
			"FragColor = vec4(pixelColor, 1.0);\n"
		"}\0"
		;

	return constructShaderProgramFromString(vertexSource, fragmentSource);
}
ShaderProgram gridEngineGenerateBorderProgram()
{
	const char* vertexSource = 
		"#version 330 core\n"
		"layout (location = 2) in vec2 vertexPosition;\n"
		"uniform vec3 borderColor;\n"
		"uniform vec2 aCanvasBottomLeftCoordsInNDC;\n" 
		"out vec3 pixelColor;\n"
		"out vec2 canvasBottomLeftCoordsInNDC;\n"
		"out vec2 canvasTopRightCoordsInNDC;\n"
		"out vec2 pixelCoordsInNDC;\n" 
		"void main()\n"
		"{\n"
			"vec2 translatedPosition = vertexPosition * 1000000;\n"
			"vec2 viewportBottomLeftCoords = vec2(-1, -1);\n"
			"translatedPosition = translatedPosition + viewportBottomLeftCoords;\n"
			"gl_Position = vec4(translatedPosition, 0.0, 1.0);\n"
			"pixelCoordsInNDC = gl_Position.xy;\n"
			"canvasBottomLeftCoordsInNDC = aCanvasBottomLeftCoordsInNDC;\n"
			"canvasTopRightCoordsInNDC = canvasBottomLeftCoordsInNDC * -1;\n"
			"pixelColor = borderColor;\n"
		"}\0"
		;

	const char* fragmentSource = 
		"#version 330 core\n"
		"in vec3 pixelColor;\n"
		"in vec2 canvasBottomLeftCoordsInNDC;\n"
		"in vec2 canvasTopRightCoordsInNDC;\n"
		"in vec2 pixelCoordsInNDC;\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
			"if (all(greaterThan(pixelCoordsInNDC, canvasBottomLeftCoordsInNDC)) && all(lessThan(pixelCoordsInNDC, canvasTopRightCoordsInNDC))) discard;\n"
			"FragColor = vec4(pixelColor, 1.0);\n"
		"}\0"
		;

	return constructShaderProgramFromString(vertexSource, fragmentSource);
}

void gridEngineTerminate(struct GameState* game)
{
	gameStateDestroy(game);
	glfwTerminate();
}

// used in the main game loop
static void gridEngineRender(struct GameState* game)
{
	glClear(GL_COLOR_BUFFER_BIT);
	gameStateDraw(game);
	glfwSwapBuffers(game->gameInfo.window);
}
static int gridEngineUpdateGameState(struct GameState* game)
{
	game->previousInputData = game->inputData;
	glfwPollEvents();

	// PRE UPDATE
	if (game->preUpdate(game) == GRID_ENGINE_ERROR) return GRID_ENGINE_ERROR;

	// PERFORM ALL GAME OBJECT UPDATE AND DRAW FUNCTIONS
	struct GameObject* object;
	for (int idx = 0; idx < game->gameInfo.numObjects; idx++)
	{
		object = game->objectArray[idx];

		// object function function
		if (object->update(object, game) == GRID_ENGINE_ERROR) return GRID_ENGINE_ERROR;

		// object draw (on canvas) function
		if (gameStateGetCanvas(game, object->canvasId) != NULL) 
			object->draw(object);
		// ^^^ this error checking is done here since EVERY draw call would need to have this anyway. So in the draw calls, it is guaranteed the canvas is in the canvas array
	}

	// POST UPDATE
	if (game->postUpdate(game) == GRID_ENGINE_ERROR) return GRID_ENGINE_ERROR;
	
	return GRID_ENGINE_SUCCESS;
}
static void waitOnMaxFPS(struct GameState* game)
{
	double timeAtStartOfGameLoop = game->timeData.currentTime;
	double currentTime = glfwGetTime();
	double currentDeltaTime = currentTime - timeAtStartOfGameLoop;
	double currentFPS = 1.0 / currentDeltaTime;

	while (currentFPS > GRID_GAME_MAX_FPS)
	{
		currentTime = glfwGetTime();
		currentDeltaTime = currentTime - timeAtStartOfGameLoop;
		currentFPS = 1.0 / currentDeltaTime;
	}
}

int gridEngineGameLoop(struct GameState* game)
{
	while (!glfwWindowShouldClose(game->gameInfo.window))
	{
		gameStateUpdateTime(game);

		// ========================================
		// CORE OF GAME LOOP
		if (gridEngineUpdateGameState(game) == GRID_ENGINE_ERROR)
		{
			printf("GRID_GAME_LOOP ERROR: GameState update returned an error\n");
			return GRID_ENGINE_ERROR;
		}
		gridEngineRender(game);
		// ========================================

		// puts the loop in a spin cycle until FPS is not above defined max FPS
		waitOnMaxFPS(game);
	}

	return GRID_ENGINE_SUCCESS;
}
