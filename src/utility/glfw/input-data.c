#include "utility/glfw/input-data.h"

struct InputData inputData()
{
	// zeroes out entire struct
	struct InputData inputData = {}; // c23 zero initialization
	return inputData;
}

// NOTE: the engine InputData struct is always stored in the window user pointer

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	struct InputData* inputData = glfwGetWindowUserPointer(window);
	inputData->keyFlags[key] = action; // press is true, release is false, repeat doesnt matter
}
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	struct InputData* inputData = glfwGetWindowUserPointer(window);
	inputData->mouseFlags[button] = action;
}
void cursorPosCallback(GLFWwindow* window, double xPos, double yPos)
{
	struct InputData* inputData = glfwGetWindowUserPointer(window);
	inputData->cursorPos.x = xPos;

	// invert the y because by default, the coordinates are relative to the top left corner (so dumb)
	int height;
	glfwGetWindowSize(window, NULL, &height);
	inputData->cursorPos.y = height - yPos;
}
