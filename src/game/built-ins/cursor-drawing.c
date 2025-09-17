#include "game/built-ins/cursor-drawing.h"

#include "game/game-object.h"
#include "canvas/coordinate-canvas.h"
#include "canvas/canvas-drawing.h"
#include "engine/grid-engine.h"
#include "engine/grid-engine-states.h"
#include "engine/grid-engine-helpers.h"
#include "utility/color.h"
#include "utility/math-util.h"
#include "utility/glfw/input-data.h"
#include "glfw.h"
#include <lina/lina.h>
#include <math.h>

struct BuiltInCursorDrawing builtInCursorDrawing()
{
	struct BuiltInCursorDrawing cursorDrawing;

	cursorDrawing.parent = gameObject("built-in-cursor-drawing", NULL);
	gameObjectAttachUpdateFunction(&cursorDrawing.parent, cursorDrawingUpdate);
	gameObjectAttachDrawFunction(&cursorDrawing.parent, cursorDrawOnCanvas);

	cursorDrawing.penColor = color(0, 0, 0);

	return cursorDrawing;
}
void cursorDrawingChangePenColor(struct BuiltInCursorDrawing* const cursorDrawing, const Color color)
{
	cursorDrawing->penColor = color;
}

enum GridEngineStates cursorDrawingUpdate(struct GameObject* const object, struct GridEngine* const engine)
{
	// NOTE: state of canvas being drawn on is tracked through the parent GameObject canvasId member. If it is NULL, then we are not in valid drawing conditions
	struct BuiltInCursorDrawing* cursorDrawingObject = (struct BuiltInCursorDrawing*)object;
	struct InputData* input = &engine->window.input;

	// drawing only occurs after left mouse button has been held down
	if (inputDataGetMouseButtonFramesPressed(input, GLFW_MOUSE_BUTTON_LEFT) < 2)
	{
		cursorDrawingObject->parent.canvasId = NULL;
		return GRID_ENGINE_RUNNING;
	}

	struct CoordinateCanvas* canvasUnderCursor = getCanvasUnderCursor(engine);
	if (!canvasUnderCursor)
	{
		cursorDrawingObject->parent.canvasId = NULL;
		return GRID_ENGINE_RUNNING;
	}

	// mouse is being held down AND there is a canvas under the cursor
	cursorDrawingObject->parent.canvasId = (char*)canvasUnderCursor->id;

	// map the cursor position to that of the canvas grid
	// TODO: yucky yucky casting. Get your types in order (also, update Lina to support vector type casting functions)
	Vec2 castedCursorPos = vec2(input->mouse.cursorPos.x, input->mouse.cursorPos.y);
	Vec2 castedPrevCursorPos = vec2(input->mouse.prevCursorPos.x, input->mouse.prevCursorPos.y);
	Vec2 canvasWindowBottomLeft = canvasUnderCursor->origin;
	Vec2 canvasWindowTopRight = vec2Add(canvasUnderCursor->origin, canvasUnderCursor->size);
	Vec2 canvasCoordsBottomLeft = vec2(0, 0);
	Vec2 canvasCoordsTopRight = vec2(canvasUnderCursor->gridUnitCnt.x, canvasUnderCursor->gridUnitCnt.y);

	Vec2 mappedCursorPos = MAP_BOXED_RANGE(castedCursorPos, canvasWindowBottomLeft, canvasWindowTopRight, canvasCoordsBottomLeft, canvasCoordsTopRight);
	Vec2 mappedPrevCursorPos = MAP_BOXED_RANGE(castedPrevCursorPos, canvasWindowBottomLeft, canvasWindowTopRight, canvasCoordsBottomLeft, canvasCoordsTopRight);

	cursorDrawingObject->cursorPosMappedToCanvas = uvec2(mappedCursorPos.x, mappedCursorPos.y);
	cursorDrawingObject->prevCursorPosMappedToCanvas = uvec2(mappedPrevCursorPos.x, mappedPrevCursorPos.y);

	return GRID_ENGINE_RUNNING;
}
void cursorDrawOnCanvas(const struct GameObject* const object, struct CoordinateCanvas* const canvas)
{
	struct BuiltInCursorDrawing* cursorDrawingObject = (struct BuiltInCursorDrawing*)object;
	canvasDrawLine(canvas, cursorDrawingObject->penColor, cursorDrawingObject->prevCursorPosMappedToCanvas, cursorDrawingObject->cursorPosMappedToCanvas);
}

