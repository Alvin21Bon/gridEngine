#pragma once

#include <sys/types.h>
#include "utility/glfw/input-data.h"
#include "utility/glfw/time-data.h"
#include "utility/glfw/cursor-shape-manager.h"

struct GLFWwindow;

// single instance of this will be made which will make a window based off engine options
struct GridWindow {
	GLFWwindow* windowPointer;
	struct InputData input;
	struct TimeData time;
	struct CursorShapeManager cursorShapeManager;
};

struct GridWindow gridWindow();

// called every frame to update window, input, and time data
void gridWindowUpdate(struct GridWindow* const window);

// for updating the engine window size option variable
void windowSizeCallback(GLFWwindow* window, int width, int height);

void gridWindowDestroy(struct GridWindow* window);
