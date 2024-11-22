#include "utility/glfw/window.h"

#include "utility/glfw/input-data.h"
#include "utility/glfw/time-data.h"
#include "engine/grid-engine-options.h"
#include "glfw.h"

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

	struct GridWindow gridWindow;
	gridWindow.windowPointer = window;
	gridWindow.time = timeData();
	gridWindow.input = inputData();

	return gridWindow;
}
void gridWindowUpdate(struct GridWindow* const window)
{
	// TODO: IMPLEMENT THIS. CHECK FOR SIZE/TITLE CHANGES. UPDATE TIMEDATA AND INPUTDATA
}
