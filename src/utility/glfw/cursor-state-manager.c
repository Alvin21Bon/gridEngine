#include "../../../include/engine/grid-engine.h"

struct CursorStateManager cursorStateManager(GLFWwindow* const window)
{
	struct CursorStateManager cursorStateManager;
	cursorStateManager.shapes.crosshair = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
	cursorStateManager.shapes.hResize = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
	cursorStateManager.shapes.vResize = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);

	cursorSetState(&cursorStateManager, window, GRID_CURSOR_DEFAULT);
	return cursorStateManager;
}
void cursorSetState(struct CursorStateManager* const cursor, GLFWwindow* const window, enum CursorStates stateToSet)
{
	if (cursor->state == stateToSet) return;

	cursor->state = stateToSet;
	switch (stateToSet)
	{
		case GRID_CURSOR_DEFAULT:
			glfwSetCursor(window, NULL);
			break;
		case GRID_CURSOR_MOVING:
			glfwSetCursor(window, cursor->shapes.crosshair);
			break;
		case GRID_CURSOR_RESIZING_LEFT:
		case GRID_CURSOR_RESIZING_RIGHT:
			glfwSetCursor(window, cursor->shapes.hResize);
			break;
		case GRID_CURSOR_RESIZING_BELOW:
		case GRID_CURSOR_RESIZING_ABOVE:
			glfwSetCursor(window, cursor->shapes.vResize);
			break;
		default:
			printf("GAME_STATE_SET_CURSOR_STATE ERROR: loose int values somehow entered this function. FIX.\n");
			break;
	}
}
void cursorStateManagerDestroy(struct CursorStateManager* cursor)
{
	printf("Destroying CursorStateManager...\n");
	glfwDestroyCursor(cursor->shapes.crosshair);
	glfwDestroyCursor(cursor->shapes.hResize);
	glfwDestroyCursor(cursor->shapes.vResize);
}
