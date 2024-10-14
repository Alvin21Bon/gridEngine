#pragma once

#include "../shared.h"

enum CursorStates {
	GRID_CURSOR_DEFAULT,
	GRID_CURSOR_MOVING,
	GRID_CURSOR_RESIZING_LEFT,
	GRID_CURSOR_RESIZING_RIGHT,
	GRID_CURSOR_RESIZING_BELOW,
	GRID_CURSOR_RESIZING_ABOVE
};

/*
 * Manages all functionality pertaining to: 
 * 	- Managing state of the cursor, including switching cursors 
 * 	- Storing different cursors
 * 	- Destorying the GLFW cursors
*/
struct CursorStateHandler {
	struct {
		typeof(GLFWcursor*) crosshair, hResize, vResize;
	} shapes;

	enum CursorStates state;
};

struct CursorStateHandler cursorStateHandler(GLFWwindow* const window);
void cursorSetState(struct CursorStateHandler* const cursor, GLFWwindow* const window, enum CursorStates stateToSet);
void cursorStateHandlerDestory(struct CursorStateHandler* cursor);
