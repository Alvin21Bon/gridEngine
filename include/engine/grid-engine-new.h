#pragma once

#include "game/game-state-new.h"
#include "utility/opengl/shader-programs.h"
#include "utility/glfw/cursor-state-manager.h"

/*
 * A singleton instance of this struct is made on initialization of the engine. This struct basically embodies the entire
 * running session of the engine. APIs to the engine are provided to the user, but generally the user will not 
 * interact with this struct much.
 *
 * The user can work with the gamestate and edit engine configurations
 * in the user defined engine init function.
*/
struct GridEngine {
	struct GameState1 gameState;
	struct ShaderProgramManager shaderProgramManager;
	struct CursorStateManager cursorStateManager;
	// ENGINE OPTIONS
	// WINDOW/GLFW
};

void gridEngineInit(struct GridEngine* const engine);
