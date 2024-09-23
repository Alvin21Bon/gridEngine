#include "../include/grid-engine.h"

struct InputData inputData()
{
	// zeroes out entire struct
	struct InputData inputData = { {0} };
	return inputData;
}

// NOTE: the engine GameState is always stored in the window user pointer

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	struct GameState* game = glfwGetWindowUserPointer(window);
	game->inputData.keyFlags[key] = action; // press is true, release is false, repeat doesnt matter
}
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	struct GameState* game = glfwGetWindowUserPointer(window);
	game->inputData.mouseFlags[button] = action;
}
void cursorPosCallback(GLFWwindow* window, double xPos, double yPos)
{
	struct GameState* game = glfwGetWindowUserPointer(window);
	game->inputData.cursorPos.x = xPos;

	// invert the y because by default, the coordinates are relative to the top left corner (so dumb)
	int height;
	glfwGetWindowSize(window, NULL, &height);
	game->inputData.cursorPos.y = height - yPos;
}
