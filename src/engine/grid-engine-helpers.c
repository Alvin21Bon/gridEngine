#include "engine/grid-engine-helpers.h"

#include "engine/grid-engine.h"
#include "canvas/coordinate-canvas.h"
#include "utility/math-util.h"
#include <lina/lina.h>

struct CoordinateCanvas* getCanvasUnderCursor(const struct GridEngine* const engine)
{
	Vec2 cursorPos = vec2(engine->window.input.mouse.cursorPos.x, engine->window.input.mouse.cursorPos.y);
	struct CoordinateCanvas* canvas;
	Vec2 canvasHitboxBottomLeft, canvasHitboxTopRight;

	// traverse backwards to return top-most interacted canvas first
	for (int idx = engine->canvasArray.num - 1; idx >= 0; idx--)
	{
		canvas = engine->canvasArray.elements[idx];

		canvasHitboxBottomLeft = canvas->origin;
		canvasHitboxTopRight = vec2Add(canvas->origin, canvas->size);

		if (IS_IN_BOXED_RANGE(cursorPos, canvasHitboxBottomLeft, canvasHitboxTopRight)) return canvas;
	}

	return NULL;
}

