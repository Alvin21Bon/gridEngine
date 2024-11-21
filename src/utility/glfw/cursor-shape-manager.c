#include "utility/glfw/cursor-shape-manager.h"

#include "glfw.h"
#include <stdio.h>

struct CursorShapeManager cursorShapeManager(GLFWwindow* const window)
{
	struct CursorShapeManager cursorShapeManager;
	cursorShapeManager.iBeam = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
	cursorShapeManager.crosshair = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
	cursorShapeManager.pointingHand = glfwCreateStandardCursor(GLFW_POINTING_HAND_CURSOR);
	cursorShapeManager.hResize = glfwCreateStandardCursor(GLFW_RESIZE_EW_CURSOR);
	cursorShapeManager.vResize = glfwCreateStandardCursor(GLFW_RESIZE_NS_CURSOR);
	cursorShapeManager.tlbrResize = glfwCreateStandardCursor(GLFW_RESIZE_NWSE_CURSOR);
	cursorShapeManager.trblResize = glfwCreateStandardCursor(GLFW_RESIZE_NESW_CURSOR);
	cursorShapeManager.allResize = glfwCreateStandardCursor(GLFW_RESIZE_ALL_CURSOR);
	cursorShapeManager.notAllowed = glfwCreateStandardCursor(GLFW_NOT_ALLOWED_CURSOR);

	cursorSetShape(&cursorShapeManager, window, GRID_CURSOR_ARROW);
	return cursorShapeManager;
}
void cursorSetShape(struct CursorShapeManager* const cursor, GLFWwindow* const window, const enum CursorShapes shapeToSet)
{
	if (cursor->activeShape == shapeToSet) return;

	cursor->activeShape = shapeToSet; 
	switch (shapeToSet)
	{
		case GRID_CURSOR_ARROW:
			glfwSetCursor(window, NULL);
			break;
		case GRID_CURSOR_IBEAM:
			glfwSetCursor(window, cursor->iBeam);
			break;
		case GRID_CURSOR_CROSSHAIR:
			glfwSetCursor(window, cursor->crosshair);
			break;
		case GRID_CURSOR_POINTING:
			glfwSetCursor(window, cursor->pointingHand);
			break;
		case GRID_CURSOR_RESIZE_HORIZONTAL:
			glfwSetCursor(window, cursor->hResize);
			break;
		case GRID_CURSOR_RESIZE_VERTICAL:
			glfwSetCursor(window, cursor->vResize);
			break;
		case GRID_CURSOR_RESIZE_TOPLEFT_BOTRIGHT:
			glfwSetCursor(window, cursor->tlbrResize);
			break;
		case GRID_CURSOR_RESIZE_TOPRRIGHT_BOTLEFT:
			glfwSetCursor(window, cursor->trblResize);
			break;
		case GRID_CURSOR_RESIZE_ALL:
			glfwSetCursor(window, cursor->allResize);
			break;
		case GRID_CURSOR_NOT_ALLOWED:
			glfwSetCursor(window, cursor->notAllowed);
			break;
		default:
			printf("CURSOR_SET_SHAPE WARN: shape must be of enum CursorShapes\n");
			cursor->activeShape = GRID_CURSOR_ARROW;
			glfwSetCursor(window, NULL);
			break;
	}
}
void cursorShapeManagerDestroy(struct CursorShapeManager* cursor)
{
	printf("Destroying CursorShapeManager...\n");
	glfwDestroyCursor(cursor->iBeam);
	glfwDestroyCursor(cursor->crosshair);
	glfwDestroyCursor(cursor->pointingHand);
	glfwDestroyCursor(cursor->hResize);
	glfwDestroyCursor(cursor->vResize);
	glfwDestroyCursor(cursor->tlbrResize);
	glfwDestroyCursor(cursor->trblResize);
	glfwDestroyCursor(cursor->allResize);
	glfwDestroyCursor(cursor->notAllowed);
	cursor->activeShape = 0;
}
