#pragma once

#include "game/game-object.h"
#include "engine/grid-engine-states.h"

struct GridEngine;
struct CoordinateCanvas;

enum CursorCanvasInteractionTypes {
	IDLE,
	MOVING,
	L_RESIZE,
	R_RESIZE,
	U_RESIZE,
	D_RESIZE
};

struct InteractedCanvas {
	struct CoordinateCanvas* target;
	enum CursorCanvasInteractionTypes type;
};

struct BuiltInCursorCanvasInteractions {
	struct GameObject parent;
	struct InteractedCanvas interaction;
};

struct BuiltInCursorCanvasInteractions builtInCursorCanvasInteractions();

struct InteractedCanvas getInteractableCanvasUnderCursor(const struct GridEngine* const engine);
void updateCursorShape(struct GridEngine* const engine, const struct InteractedCanvas* const interaction);
void performCursorCanvasInteraction(const struct GridEngine* const engine, const struct InteractedCanvas* const interaction);

/*
 * Gives cursors the functionality of dragging a canvas to translate or resize it
*/
enum GridEngineStates cursorCanvasInteractionsUpdate(struct GameObject* const object, struct GridEngine* const engine);

/*
 * NOTE: only to revert cursor shape back to default if it was previously changed
*/
void cursorCanvasInteractionsDestroy(struct GameObject* object, struct GridEngine* const engine);

