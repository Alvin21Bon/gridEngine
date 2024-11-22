#include "utility/glfw/input-data.h"

#include "glfw.h"
#include <lina/lina.h>
#include <stdio.h>

struct InputData inputData()
{
	// zeroes out entire struct
	struct InputData inputData = {}; // c23 zero initialization
	return inputData;
}
void inputDataUpdate(struct InputData* const inputData)
{
	// increment this first since a call of this function signals another frame
	inputData->numTotalUpdates++;  
	glfwPollEvents(); 
}

// NOTE: the engine InputData struct is always stored in the window user pointer

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	struct InputData* inputData = (struct InputData*)glfwGetWindowUserPointer(window);
	inputData->keyFlags[key].isDown = action; // press is true, release is false, repeat doesnt matter
	
	switch (action)
	{
		case GLFW_PRESS:
			inputData->keyFlags[key].frameFirstPressed = inputData->numTotalUpdates;
			break;
		case GLFW_RELEASE:
			inputData->keyFlags[key].frameFirstReleased = inputData->numTotalUpdates;
			break;
		case GLFW_REPEAT:
			return; // dont mess up frameFirstPressed
		default:
			printf("???\n");
			break;
	}
}
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	struct InputData* inputData = (struct InputData*)glfwGetWindowUserPointer(window);
	inputData->mouseFlags[button].isDown = action; // press is true, release is false, repeat doesnt matter
	
	switch (action)
	{
		case GLFW_PRESS:
			inputData->mouseFlags[button].frameFirstPressed = inputData->numTotalUpdates;
			break;
		case GLFW_RELEASE:
			inputData->mouseFlags[button].frameFirstReleased = inputData->numTotalUpdates;
			break;
		case GLFW_REPEAT:
			return; // dont mess up frameFirstPressed
		default:
			printf("???\n");
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
