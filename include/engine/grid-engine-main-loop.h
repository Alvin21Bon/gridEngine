#pragma once

enum GridEngineStates;
struct GridEngine;

void gridEngineRender(struct GridEngine* const engine);
enum GridEngineStates enterEngineLoop(struct GridEngine* const engine);

int main();

