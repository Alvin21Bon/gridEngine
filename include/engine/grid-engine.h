#pragma once

#include "game/game-state.h"
#include "utility/opengl/shader-programs.h"
#include "utility/glfw/cursor-shape-manager.h"
#include "engine/grid-engine-states.h"

/*
 * A singleton instance of this struct is made on initialization of the engine. This struct basically embodies the entire
 * running session of the engine. APIs to the engine are provided to the user, since none of the GridEngine
 * struct definition is not visible to the user
 *
 * The user can edit engine configurations in the user defined engine init function.
*/
struct GridEngine {
	struct GameState gameState;
	struct ShaderProgramManager shaderProgramManager;
	struct CursorShapeManager cursorShapeManager;

	enum GridEngineStates state;
	enum GridEngineStates (*pausedStateFunction)(struct GridEngine* const);

	// WINDOW/GLFW
};

// user defined init function
void gridEngineInit(struct GridEngine* const engine);

// getters and setters
void gridEngineAttachPausedStateFunction(struct GridEngine* const engine, enum GridEngineStates (*pausedStateFunction)(struct GridEngine* const));
