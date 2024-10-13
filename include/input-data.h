#pragma once

#include "shared.h"

struct InputData {
	// NOTE: keeps arrays first field so constructor initializes all fields to zero correctly
	bool keyFlags[GLFW_KEY_LAST + 1];
	bool mouseFlags[GLFW_MOUSE_BUTTON_LAST + 1];
	struct {
		double x, y;
	} cursorPos;
};

struct InputData inputData();

/*
 * 	THESE CALLBACKS ARE SET IN THE INITIATION OF THE ENGINE
*/

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void cursorPosCallback(GLFWwindow* window, double xPos, double yPos);

