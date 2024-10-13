#include "../include/engine/grid-engine.h"

int main()
{
	if (!glfwInit())
	{
		printf("GRID_MAIN ERROR: GLFW init failed\n");
		return GRID_ENGINE_ERROR;
	}

	GLFWwindow* window = gridEngineCreateAndSetupWindow();
	if (window == NULL)
	{
		printf("GRID_MAIN ERROR: window creation failed\n");
		glfwTerminate();
		return GRID_ENGINE_ERROR;
	}

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("GRID_MAIN ERROR: GLAD failed to load openGL\n");
		glfwTerminate();
		return GRID_ENGINE_ERROR;
	}

	ShaderProgram canvasProgram = gridEngineGenerateCanvasProgram();
	ShaderProgram borderProgram = gridEngineGenerateBorderProgram();

	struct GameState game = gameState(window, canvasProgram, borderProgram);
	glfwSetWindowUserPointer(window, &game); // THIS REMAINS IN THE WINDOW USER POINTER FOREVER
	
	// TODO: ADD CONFIG SUPPORT WITH TOML
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	if (gridEngineGameLoop(&game) == GRID_ENGINE_ERROR)
	{
		printf("GRID_MAIN ERROR: game loop encountered an error\n");
		gridEngineTerminate(&game);
		return GRID_ENGINE_ERROR;
	}

	gridEngineTerminate(&game);
	return GRID_ENGINE_SUCCESS;
}
