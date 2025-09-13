#pragma once

enum GridEngineStates;
struct GridEngine;

void gridEngineRender(struct GridEngine* const engine);
enum GridEngineStates enterEngineLoop(struct GridEngine* const engine);

/*
 * user-defined function that runs immediately at the start of the program, even before engine initialization
 * NOTE: this is the best place to set engine options, so that they take effect before engine initialization
*/
void initProgram();

