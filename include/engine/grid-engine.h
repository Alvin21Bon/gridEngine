#pragma once

#include "../canvas/canvas-types.h"
#include "../canvas/canvas-border.h"
#include "../canvas/canvas-pixel.h"
#include "../canvas/coordinate-canvas.h"
#include "../utility/glfw/input-data.h"
#include "../utility/glfw/cursor-state-handler.h"
#include "grid-engine-states.h"
#include "../game/game-object.h"
#include "../game/game-state.h"
#include "../utility/shared.h"

// TODO: ADD CONFIG SUPPORT WITH TOML
// NOTE: This function automatically does a lot of needed setup for the window, such as 
// 	 setting the hints, context, and callbacks
GLFWwindow* gridEngineCreateAndSetupWindow();

ShaderProgram gridEngineGenerateCanvasProgram();
ShaderProgram gridEngineGenerateBorderProgram();

// destroys the GameState, terminates GLFW, and any other possible cleanup necessary
void gridEngineTerminate(struct GameState* game);

// called in the engine entrypoint, returns GRID_ENGINE_ERROR if an error occured
int gridEngineGameLoop(struct GameState* game);

