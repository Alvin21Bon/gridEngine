#pragma once

#include <lina/lina.h>
#include <sys/types.h>
#include "glfw.h"

struct InputEntry {
	bool isDown;
	// these extra members are used to calculate how many frames a key/button has been in either pressed/released state.
	ulong frameFirstPressed; 
	ulong frameFirstReleased;
};
struct InputData {
	struct InputEntry key[GLFW_KEY_LAST + 1];
	struct InputEntry mouseButton[GLFW_MOUSE_BUTTON_LAST + 1];

	dVec2 cursorPos;
	dVec2 previousCursorPos;
	dVec2 cursorPosDelta; // this is calculated here since this is commonly needed when working with cursor coordinates
};

struct InputData inputData();
void inputDataUpdate(struct InputData* const inputData);

bool inputDataIsKeyDown(const struct InputData* inputData, const int key);
ulong inputDataGetKeyFramesPressed(const struct InputData* inputData, const int key);
ulong inputDataGetKeyFramesReleased(const struct InputData* inputData, const int key);

bool inputDataIsMouseButtonDown(const struct InputData* inputData, const int button);
ulong inputDataGetMouseButtonFramesPressed(const struct InputData* inputData, const int button);
ulong inputDataGetMouseButtonFramesReleased(const struct InputData* inputData, const int button);

/*
 * 	THESE CALLBACKS ARE SET FOR GLFW DURING THE CREATION OF THE GRIDWINDOW
*/

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void cursorPosCallback(GLFWwindow* window, double xPos, double yPos);

