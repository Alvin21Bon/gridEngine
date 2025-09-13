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
	LOG(GRID_LOGGING_FULL, __func__, __LINE__, "initating GLFW...\n");
	if (!glfwInit())
	{
		LOG(GRID_LOGGING_ERROR, __func__, __LINE__, "GLFW failed to init\n");
		exit(EXIT_FAILURE);
	}

	// making the window
	LOG(GRID_LOGGING_FULL, __func__, __LINE__, "creating GLFW window...\n");
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
	LOG(GRID_LOGGING_FULL, __func__, __LINE__, "setting GLFW window callbacks...\n");
	glfwSetKeyCallback(window, keyCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetCursorPosCallback(window, cursorPosCallback);
	glfwSetWindowSizeCallback(window, windowSizeCallback);

	struct GridWindow gridWindow;
	gridWindow.windowPointer = window;
	gridWindow.time = timeData();
	gridWindow.input = inputData();
	gridWindow.cursorShapeManager = cursorShapeManager(window);

	return gridWindow;
}
void gridWindowUpdate(struct GridWindow* const window)
{
	timeDataUpdate(&window->time);
	inputDataUpdate(&window->input); // call this first, so poll events can run for the windowSizeCallback before the code below
	
	// engine window title option was manually changed via the set option function
	if (strcmp(glfwGetWindowTitle(window->windowPointer), GRID_WINDOW_TITLE) != 0)
	{
		LOG(GRID_LOGGING_FULL, __func__, __LINE__, "window title changed to %s\n", GRID_WINDOW_TITLE);
		glfwSetWindowTitle(window->windowPointer, GRID_WINDOW_TITLE);
	}

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
	LOG(GRID_LOGGING_FULL, __func__, __LINE__, "destroying window...\n");
	cursorShapeManagerDestroy(&window->cursorShapeManager);
	glfwTerminate();
}

