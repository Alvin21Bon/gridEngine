#pragma once

#include "GLFW/glfw3.h"

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
 * 	- Destroying the GLFW cursors
*/
struct CursorStateManager {
	struct {
		typeof(GLFWcursor*) crosshair, hResize, vResize;
	} shapes;

	enum CursorStates state;
};

struct CursorStateManager cursorStateManager(GLFWwindow* const window);
void cursorSetState(struct CursorStateManager* const cursor, GLFWwindow* const window, enum CursorStates stateToSet);
void cursorStateManagerDestroy(struct CursorStateManager* cursor);
