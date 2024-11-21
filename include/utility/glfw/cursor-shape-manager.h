#pragma once

typedef struct GLFWcursor GLFWcursor;
typedef struct GLFWwindow GLFWwindow;

enum CursorShapes {
	GRID_CURSOR_ARROW, 
	GRID_CURSOR_IBEAM,
	GRID_CURSOR_CROSSHAIR,
	GRID_CURSOR_POINTING,
	GRID_CURSOR_RESIZE_HORIZONTAL,
	GRID_CURSOR_RESIZE_VERTICAL,
	GRID_CURSOR_RESIZE_TOPLEFT_BOTRIGHT,
	GRID_CURSOR_RESIZE_TOPRRIGHT_BOTLEFT,
	GRID_CURSOR_RESIZE_ALL,
	GRID_CURSOR_NOT_ALLOWED
};

struct CursorShapeManager {
	// default arrow is not saved since it is not necessary
	GLFWcursor *iBeam, *crosshair, *pointingHand, *hResize, *vResize,
		   *tlbrResize, *trblResize, *allResize, *notAllowed;

	enum CursorShapes activeShape;
};

struct CursorShapeManager cursorShapeManager(GLFWwindow* const window);
void cursorSetShape(struct CursorShapeManager* const cursor, GLFWwindow* const window, const enum CursorShapes shapeToSet);
void cursorShapeManagerDestroy(struct CursorShapeManager* cursor);
