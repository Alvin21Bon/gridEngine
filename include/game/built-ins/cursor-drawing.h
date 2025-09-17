#pragma once

#include "game/game-object.h"
#include "engine/grid-engine-states.h"
#include "utility/color.h"
#include <lina/lina.h>

struct GridEngine;
struct CoordinateCanvas;

struct BuiltInCursorDrawing {
	struct GameObject parent;
	Color penColor;
	uVec2 cursorPosMappedToCanvas;
	uVec2 prevCursorPosMappedToCanvas;
};

struct BuiltInCursorDrawing builtInCursorDrawing();
void cursorDrawingChangePenColor(struct BuiltInCursorDrawing* const cursorDrawing, const Color color);

enum GridEngineStates cursorDrawingUpdate(struct GameObject* const object, struct GridEngine* const engine);
void cursorDrawOnCanvas(const struct GameObject* const object, struct CoordinateCanvas* const canvas);

