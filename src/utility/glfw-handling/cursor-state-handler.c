#pragma once

#include "../../../include/engine/grid-engine.h"

struct CursorStateHandler cursorStateHandler(GLFWwindow* const window)
{
	struct CursorStateHandler cursorStateHandler;
	cursorStateHandler.shapes.crosshair = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
	cursorStateHandler.shapes.hResize = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
	cursorStateHandler.shapes.vResize = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);

	cursorSetState(&cursorStateHandler, window, GRID_CURSOR_DEFAULT);
	return cursorStateHandler;
}
void cursorSetState(struct CursorStateHandler* const cursor, GLFWwindow* const window, enum CursorStates stateToSet)
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
void cursorStateHandlerDestory(struct CursorStateHandler* cursor)
{
	printf("Destroying CursorStatehandler...\n");
	glfwDestroyCursor(cursor->shapes.crosshair);
	glfwDestroyCursor(cursor->shapes.hResize);
	glfwDestroyCursor(cursor->shapes.vResize);
}
