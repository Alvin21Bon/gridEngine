#include "utility/glfw/input-data.h"

#include "glfw.h"
#include "engine/grid-engine-globals.h"
#include "utility/logging.h"
#include <lina/lina.h>

struct InputData inputData()
{
	// zeroes out entire struct
	struct InputData inputData = {}; // c23 zero initialization
	return inputData;
}
void inputDataUpdate(struct InputData* const inputData)
{
	// this is just a nice wrapper
	glfwPollEvents(); 
}

bool inputDataIsKeyDown(const struct InputData* inputData, const int key)
{
	return inputData->key[key].isDown;
}
ulong inputDataGetKeyFramesPressed(const struct InputData* inputData, const int key)
{
	return inputDataIsKeyDown(inputData, key) ? GRID_FRAME_COUNTER - inputData->key[key].frameFirstPressed + 1 : 0;
}
ulong inputDataGetKeyFramesReleased(const struct InputData* inputData, const int key)
{
	return !inputDataIsKeyDown(inputData, key) ? GRID_FRAME_COUNTER - inputData->key[key].frameFirstReleased + 1 : 0;
}

bool inputDataIsMouseButtonDown(const struct InputData* inputData, const int button)
{
	return inputData->mouseButton[button].isDown;
}
ulong inputDataGetMouseButtonFramesPressed(const struct InputData* inputData, const int button)
{
	return inputDataIsMouseButtonDown(inputData, button) ? GRID_FRAME_COUNTER - inputData->mouseButton[button].frameFirstPressed + 1 : 0;
}
ulong inputDataGetMouseButtonFramesReleased(const struct InputData* inputData, const int button)
{
	return !inputDataIsMouseButtonDown(inputData, button) ? GRID_FRAME_COUNTER - inputData->mouseButton[button].frameFirstReleased + 1 : 0;
}

// NOTE: the engine InputData struct is always stored in the window user pointer

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	struct InputData* inputData = (struct InputData*)glfwGetWindowUserPointer(window);
	inputData->key[key].isDown = action; // press is true, release is false, repeat doesnt matter
	
	switch (action)
	{
		case GLFW_PRESS:
			inputData->key[key].frameFirstPressed = GRID_FRAME_COUNTER;
			break;
		case GLFW_RELEASE:
			inputData->key[key].frameFirstReleased = GRID_FRAME_COUNTER;
			break;
		case GLFW_REPEAT:
			return; // dont mess up frameFirstPressed
		default:
			LOG(GRID_LOGGING_FULL, "action value of %d should not be possible here in keyCallback\n", action);
			break;
	}
}
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	struct InputData* inputData = (struct InputData*)glfwGetWindowUserPointer(window);
	inputData->mouseButton[button].isDown = action; // press is true, release is false, repeat doesnt matter
	
	switch (action)
	{
		case GLFW_PRESS:
			inputData->mouseButton[button].frameFirstPressed = GRID_FRAME_COUNTER;
			break;
		case GLFW_RELEASE:
			inputData->mouseButton[button].frameFirstReleased = GRID_FRAME_COUNTER;
			break;
		case GLFW_REPEAT:
			return; // dont mess up frameFirstPressed
		default:
			LOG(GRID_LOGGING_FULL, "action value of %d should not be possible here in mouseButtonCallback\n", action);
			break;
	}
}
void cursorPosCallback(GLFWwindow* window, double xPos, double yPos)
{
	// invert the y because by default, the coordinates are relative to the top left corner (so dumb)
	int height;
	glfwGetWindowSize(window, NULL, &height);
	yPos = height - yPos;

	struct InputData* inputData = (struct InputData*)glfwGetWindowUserPointer(window);
	inputData->previousCursorPos = inputData->cursorPos;
	inputData->cursorPos = dvec2(xPos, yPos);
	inputData->cursorPosDelta = dvec2Sub(inputData->cursorPos, inputData->previousCursorPos);
}
