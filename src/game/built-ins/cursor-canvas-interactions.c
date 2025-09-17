#include "game/built-ins/cursor-canvas-interactions.h"

#include "canvas/coordinate-canvas.h"
#include "game/game-object.h"
#include "engine/grid-engine.h"
#include "engine/grid-engine-states.h"
#include "utility/glfw/input-data.h"
#include "utility/glfw/cursor-shape-manager.h"
#include "utility/math-util.h"
#include "utility/logging.h"
#include "glfw.h"
#include <sys/types.h>
#include <lina/lina.h>

struct BuiltInCursorCanvasInteractions builtInCursorCanvasInteractions()
{
	struct BuiltInCursorCanvasInteractions cursorCanvasInteractions;
	cursorCanvasInteractions.parent = gameObject("built-in-cursor-canvas-interactions", "");
	cursorCanvasInteractions.interaction.target = NULL;
	cursorCanvasInteractions.interaction.type = IDLE;

	gameObjectAttachUpdateFunction(&cursorCanvasInteractions.parent, cursorCanvasInteractionsUpdate);
	gameObjectAttachDestroyFunction(&cursorCanvasInteractions.parent, cursorCanvasInteractionsDestroy);

	return cursorCanvasInteractions;
}

enum GridEngineStates cursorCanvasInteractionsUpdate(struct GameObject* const object, struct GridEngine* const engine)
{
	// PLAN: THREE STATES: button not down, button down for the first frame, button down for more than first frame
	// PLAN: button not down: cursor is IDLE. simply update the cursor shapes based on position over canvases
	// PLAN: button down for first frame: it is time to check for STATE CHANGES. use same code from above to update LOCKED ON canvas and cursor states
	// PLAN: button down for more than first frame: if cursor was IDLE, keep it IDLE!! otherwise, continue locked on interaction
	// PLAN: while button is down, do not update cursor shapes

	// 1) Get struct InteractedCanvas
	// 2) get num frames mb_left hold down
	// 3) if not held down, just update the cursor shapes. Also set the targeted canvas to NULL and change to IDLE state
	// 4) if held down, check if this is first frame. If it is, it is time for a STATE CHANGE. update the interaction member 
	// 5) if not first frame, do not update cursor shape or state at all, just continue performing possible canvas interaction
	
	struct BuiltInCursorCanvasInteractions* interactionsObject = (struct BuiltInCursorCanvasInteractions*)object;

	// returns possible interactable canvas under cursor along with possible type of interaction
	// if interaction exists, interaction is only "locked onto" if the left mouse button is pressed
	struct InteractedCanvas possibleInteraction = getInteractableCanvasUnderCursor(engine);
	ulong numFramesPressedMButtonLeft = inputDataGetMouseButtonFramesPressed(&engine->window.input, GLFW_MOUSE_BUTTON_LEFT);

	// not pressed, cursor shape (mostly) only changes when not pressed
	if (!numFramesPressedMButtonLeft)
	{
		interactionsObject->interaction.target = NULL;
		interactionsObject->interaction.type = IDLE;
		updateCursorShape(engine, &possibleInteraction);
		return GRID_ENGINE_RUNNING;
	}

	// first frame pressed, time to lock onto a possible canvas interaction
	if (numFramesPressedMButtonLeft == 1) 
	{
		interactionsObject->interaction.target = possibleInteraction.target;
		interactionsObject->interaction.type = possibleInteraction.type;
		updateCursorShape(engine, &possibleInteraction); // NOTE: for the RARE chance left button pressed and cursor shape hasn't changed (frame-perfect)
	}

	// WARN: do not update interaction state or cursor shape while mouse button is held down
	
	performCursorCanvasInteraction(engine, &interactionsObject->interaction);
	return GRID_ENGINE_RUNNING;
}

void cursorCanvasInteractionsDestroy(struct GameObject* object, struct GridEngine* const engine)
{
	cursorSetShape(&engine->window.cursorShapeManager, engine->window.windowPointer, GRID_CURSOR_ARROW);
}

struct InteractedCanvas getInteractableCanvasUnderCursor(const struct GridEngine* const engine)
{
	struct InteractedCanvas possibleInteraction;
	Vec2 cursorPos = vec2(engine->window.input.mouse.cursorPos.x, engine->window.input.mouse.cursorPos.y);

	// traverse backwards to return top-most interacted canvas first
	struct CoordinateCanvas* canvas;
	Vec2 movingHitboxBottomLeft, movingHitboxTopRight;

	const float RESIZE_HITBOX_EXTENSION_DISTANCE = 10;
	Vec2 canvasBorderBottomLeft, canvasBorderTopRight;
	Vec2 resizeHitboxBottomLeft, resizeHitboxTopRight;
	for (int idx = engine->canvasArray.num - 1; idx >= 0; idx--)
	{
		canvas = engine->canvasArray.elements[idx];

		movingHitboxBottomLeft = canvas->origin;
		movingHitboxTopRight = vec2Add(canvas->origin, canvas->size);

		if (IS_IN_BOXED_RANGE(cursorPos, movingHitboxBottomLeft, movingHitboxTopRight))
		{
			possibleInteraction.target = canvas;
			possibleInteraction.type = MOVING;
			return possibleInteraction;
		}

		// we now know that cursor is outside of moving hitbox range
		canvasBorderBottomLeft = canvasGetBorderOrigin(canvas);
		canvasBorderTopRight = vec2Add(canvasBorderBottomLeft, canvasGetBorderSize(canvas));

		resizeHitboxBottomLeft = vec2Sub(canvasBorderBottomLeft, vec2Fill(RESIZE_HITBOX_EXTENSION_DISTANCE));
		resizeHitboxTopRight = vec2Add(canvasBorderTopRight, vec2Fill(RESIZE_HITBOX_EXTENSION_DISTANCE));

		if (IS_IN_BOXED_RANGE(cursorPos, resizeHitboxBottomLeft, resizeHitboxTopRight))
		{
			possibleInteraction.target = canvas;

			// process what type of resizing we are doing, prioritize horizontal resizing
			if (cursorPos.x < movingHitboxBottomLeft.x) possibleInteraction.type = L_RESIZE;
			else if (cursorPos.x > movingHitboxTopRight.x) possibleInteraction.type = R_RESIZE;
			else if (cursorPos.y > movingHitboxTopRight.y) possibleInteraction.type = U_RESIZE;
			else possibleInteraction.type = D_RESIZE;

			return possibleInteraction;
		}
	}

	possibleInteraction.target = NULL;
	possibleInteraction.type = IDLE;
	return possibleInteraction;
}

void updateCursorShape(struct GridEngine* const engine, const struct InteractedCanvas* const interaction)
{
	struct GridWindow* window = &engine->window;
	enum CursorShapes shapeToSet;

	switch (interaction->type)
	{
		case IDLE:
			shapeToSet = GRID_CURSOR_ARROW;
			break;
		case MOVING:
			shapeToSet = GRID_CURSOR_CROSSHAIR;
			break;

		case L_RESIZE:
		case R_RESIZE:
			shapeToSet = GRID_CURSOR_RESIZE_HORIZONTAL;
			break;

		case U_RESIZE:
		case D_RESIZE:
			shapeToSet = GRID_CURSOR_RESIZE_VERTICAL;
			break;

		default:
			LOG(GRID_LOGGING_ERROR, __func__, __LINE__, "interaction type is not of enum CursorCanvasInteractionTypes\n");
			break;
	}

	cursorSetShape(&window->cursorShapeManager, window->windowPointer, shapeToSet);
}

void performCursorCanvasInteraction(const struct GridEngine* const engine, const struct InteractedCanvas* const interaction)
{
	Vec2 deltaCursorPos = vec2(engine->window.input.mouse.deltaCursorPos.x, engine->window.input.mouse.deltaCursorPos.y);
	struct CoordinateCanvas* canvas = interaction->target;
	if (!canvas) return;

	switch (interaction->type)
	{
		case IDLE:
			return;
		case MOVING:
			canvasTranslate(canvas, deltaCursorPos);
			break;
		case L_RESIZE:
			canvas->origin.x += deltaCursorPos.x;
			canvasSetSize(canvas, vec2(canvas->size.width - deltaCursorPos.x, canvas->size.height));
			break;
		case R_RESIZE:
			canvasSetSize(canvas, vec2(canvas->size.width + deltaCursorPos.x, canvas->size.height));
			break;
		case U_RESIZE:
			canvasSetSize(canvas, vec2(canvas->size.width, canvas->size.height + deltaCursorPos.y));
			break;
		case D_RESIZE:
			canvas->origin.y += deltaCursorPos.y;
			canvasSetSize(canvas, vec2(canvas->size.width, canvas->size.height - deltaCursorPos.y));
			break;
		default:
			LOG(GRID_LOGGING_ERROR, __func__, __LINE__, "interaction type is not of enum CursorCanvasInteractionTypes\n");
			break;
	}
}

