#include "game/built-ins/built-ins.h"

#include "engine/grid-engine.h"
#include "engine/grid-engine-options.h"
#include "game/built-ins/cursor-canvas-interactions.h"
#include "game/game-object-array.h"

void injectBuiltIns(struct GridEngine* const engine)
{
	if (GRID_BUILT_IN_ENABLE_CURSOR_CANVAS_INTERACTIONS)
	{
		struct CursorCanvasInteractions interactions = cursorCanvasInteractions();
		gameObjectArrayAddHeapCopy(&engine->gameObjectArray, (struct GameObject*)&interactions, sizeof(struct CursorCanvasInteractions));
	}
}

