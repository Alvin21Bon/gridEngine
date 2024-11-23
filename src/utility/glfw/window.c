#include "utility/glfw/window.h"

#include "utility/glfw/input-data.h"
#include "utility/glfw/time-data.h"
#include "engine/grid-engine-options.h"
#include "glfw.h"
#include <string.h>

#define GRID_GLFW_CONTEXT_VERSION_MAJOR 3
#define GRID_GLFW_CONTEXT_VERSION_MINOR 3
#define GRID_GLFW_OPENGL_PROFILE GLFW_OPENGL_CORE_PROFILE
struct GridWindow gridWindow()
{
	// making the window
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GRID_GLFW_CONTEXT_VERSION_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GRID_GLFW_CONTEXT_VERSION_MINOR);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GRID_GLFW_OPENGL_PROFILE);
	GLFWwindow* window = glfwCreateWindow(GRID_WINDOW_SIZE.width, GRID_WINDOW_SIZE.height, GRID_WINDOW_TITLE, NULL, NULL);
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
