#ifndef GRID_GRID_ENGINE_H
#define GRID_GRID_ENGINE_H

#include "canvas-types.h"
#include "canvas-border.h"
#include "canvas-pixel.h"
#include "coordinate-canvas.h"
#include "input-data.h"
#include "game-state.h"
#include "grid-engine-states.h"
#include "shared.h"

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

#endif // GRID_GRID_ENGINE_H
