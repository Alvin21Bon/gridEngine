#pragma once

// These values are returned from the GameState pre/post update functions, and any of the
// user defined GameObject update functions. 
// GRID_ENGINE_SUCCESS and GRID_ENGINE_ERROR signal to cease the game loop on a success or failure condition
// GRID_ENGINE_RUNNING dicates normal game behavior, and signals to progress with the game loop as normal
// GRID_ENGINE_PAUSED signals a pause, which will hault game updates until the state is changed
enum GridEngineStates {
	GRID_ENGINE_SUCCESS, 
	GRID_ENGINE_ERROR, 

	GRID_ENGINE_RUNNING,
	GRID_ENGINE_PAUSED
};

