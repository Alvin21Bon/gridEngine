#include "utility/glfw/window.h"

#include "utility/glfw/input-data.h"
#include "utility/glfw/time-data.h"
#include "utility/glfw/cursor-shape-manager.h"
#include "utility/logging.h"
#include "engine/grid-engine-options.h"
#include "glfw.h"
#include <string.h>
#include <stdlib.h>

#define GRID_GLFW_CONTEXT_VERSION_MAJOR 3
#define GRID_GLFW_CONTEXT_VERSION_MINOR 3
#define GRID_GLFW_OPENGL_PROFILE GLFW_OPENGL_CORE_PROFILE
struct GridWindow gridWindow()
{
	if (!glfwInit())
	{
		LOG(GRID_LOGGING_ERROR, __func__, __LINE__, "GLFW failed to init\n");
		exit(EXIT_FAILURE);
	}

	// making the window
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GRID_GLFW_CONTEXT_VERSION_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GRID_GLFW_CONTEXT_VERSION_MINOR);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GRID_GLFW_OPENGL_PROFILE);
	GLFWwindow* window = glfwCreateWindow(GRID_WINDOW_SIZE.width, GRID_WINDOW_SIZE.height, GRID_WINDOW_TITLE, NULL, NULL);
	if (!window)
	{
		LOG(GRID_LOGGING_ERROR, __func__, __LINE__, "GLFW window creation failed\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);

	// setting the callbacks
	glfwSetKeyCallback(window, keyCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetCursorPosCallback(window, cursorPosCallback);
	glfwSetWindowSizeCallback(window, windowSizeCallback);

	struct GridWindow gridWindow;
	gridWindow.windowPointer = window;
	gridWindow.time = timeData();
	gridWindow.input = inputData();
	gridWindow.cursorShapeManager = cursorShapeManager(window);

	glfwSetWindowUserPointer(window, &gridWindow.input); // remains in window user pointer for input callbacks

	return gridWindow;
}
void gridWindowUpdate(struct GridWindow* const window)
{
	timeDataUpdate(&window->time);
	inputDataUpdate(&window->input); // call this first, so poll events can run for the windowSizeCallback before the code below
	
	// engine window title option was manually changed via the set option function
	if (strcmp(glfwGetWindowTitle(window->windowPointer), GRID_WINDOW_TITLE) != 0)
		glfwSetWindowTitle(window->windowPointer, GRID_WINDOW_TITLE);

	// because of the window size callback, this will only run if the engine window size option was manually changed with the option setting function
	int width, height;
	glfwGetWindowSize(window->windowPointer, &width, &height);
	uVec2 currentWindowSize = uvec2(width, height);
	if (!uvec2Equals(currentWindowSize, GRID_WINDOW_SIZE))
		glfwSetWindowSize(window->windowPointer, GRID_WINDOW_SIZE.width, GRID_WINDOW_SIZE.height);
}

void windowSizeCallback(GLFWwindow* window, int width, int height)
{
	GRID_WINDOW_SIZE = uvec2(width, height);
}

void gridWindowDestroy(struct GridWindow* window)
{
	cursorShapeManagerDestroy(&window->cursorShapeManager);
	glfwTerminate();
}
