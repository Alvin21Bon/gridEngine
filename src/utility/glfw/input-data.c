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
	// WARN: delta cursor calculations happen here instead of in the cursorPosCallback because pollEvents processes a queue of 
	// 	 events, such as mouse movements, with each call of pollEvents. Thus, the delta cursor position since THE LAST FRAME
	// 	 would not be accurate if this calculation was done in the callback. Must sandwich the pollEvents call.
	inputData->mouse.prevCursorPos = inputData->mouse.cursorPos;
	glfwPollEvents(); 
	inputData->mouse.deltaCursorPos = dvec2Sub(inputData->mouse.cursorPos, inputData->mouse.prevCursorPos);
}

bool inputDataIsKeyDown(const struct InputData* const inputData, const int key)
{
	return inputData->key[key].isDown;
}
ulong inputDataGetKeyFramesPressed(const struct InputData* const inputData, const int key)
{
	return inputDataIsKeyDown(inputData, key) ? GRID_FRAME_COUNTER - inputData->key[key].frameFirstPressed + 1 : 0;
}
ulong inputDataGetKeyFramesReleased(const struct InputData* const inputData, const int key)
{
	return !inputDataIsKeyDown(inputData, key) ? GRID_FRAME_COUNTER - inputData->key[key].frameFirstReleased + 1 : 0;
}
bool inputDataIsKeyFirstFramePressed(const struct InputData* const inputData, const int key)
{
	return inputDataGetKeyFramesPressed(inputData, key) == 1;
}

bool inputDataIsMouseButtonDown(const struct InputData* const inputData, const int button)
{
	return inputData->mouse.button[button].isDown;
}
ulong inputDataGetMouseButtonFramesPressed(const struct InputData* const inputData, const int button)
{
	return inputDataIsMouseButtonDown(inputData, button) ? GRID_FRAME_COUNTER - inputData->mouse.button[button].frameFirstPressed + 1 : 0;
}
ulong inputDataGetMouseButtonFramesReleased(const struct InputData* const inputData, const int button)
{
	return !inputDataIsMouseButtonDown(inputData, button) ? GRID_FRAME_COUNTER - inputData->mouse.button[button].frameFirstReleased + 1 : 0;
}
bool inputDataIsMouseButtonFirstFramePressed(const struct InputData* const inputData, const int button)
{
	return inputDataGetMouseButtonFramesPressed(inputData, button) == 1;
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
			LOG(GRID_LOGGING_ERROR, __func__, __LINE__, "this action value (%d) should not be possible\n", action);
			break;
	}
}
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	struct InputData* inputData = (struct InputData*)glfwGetWindowUserPointer(window);
	inputData->mouse.button[button].isDown = action; // press is true, release is false, repeat doesnt matter
	
	switch (action)
	{
		case GLFW_PRESS:
			inputData->mouse.button[button].frameFirstPressed = GRID_FRAME_COUNTER;
			break;
		case GLFW_RELEASE:
			inputData->mouse.button[button].frameFirstReleased = GRID_FRAME_COUNTER;
			break;
		case GLFW_REPEAT:
			return; // dont mess up frameFirstPressed
		default:
			LOG(GRID_LOGGING_ERROR, __func__, __LINE__, "this action value (%d) should not be possible\n", action);
			break;
	}
}
void cursorPosCallback(GLFWwindow* window, double xPos, double yPos)
{
	struct InputData* inputData = (struct InputData*)glfwGetWindowUserPointer(window);

	// invert the y because by default, the coordinates are relative to the top left corner (so dumb)
	int height;
	glfwGetWindowSize(window, NULL, &height);
	yPos = height - yPos;
	inputData->mouse.cursorPos = dvec2(xPos, yPos);
}

