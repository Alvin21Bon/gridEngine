#pragma once

#include "canvas/canvas-array.h"
#include "game/game-object-array.h"
#include <sys/types.h>
#include <stddef.h>
#include <lina/lina.h>

struct GridEngine;
enum CursorShapes;
struct CoordinateCanvas;
struct GameObject;

// INPUT FETCHING
bool isKeyDown(const struct GridEngine* const engine, const int key);
ulong getNumFramesKeyPressed(const struct GridEngine* const engine, const int key);
ulong getNumFramesKeyReleased(const struct GridEngine* const engine, const int key);
bool isKeyFirstFramePressed(const struct GridEngine* const engine, const int key);
bool isMouseButtonDown(const struct GridEngine* const engine, const int button);
ulong getNumFramesMouseButtonPressed(const struct GridEngine* const engine, const int button);
ulong getNumFramesMouseButtonReleased(const struct GridEngine* const engine, const int button);
bool isMouseButtonFirstFramePressed(const struct GridEngine* const engine, const int button);
dVec2 getCursorPos(const struct GridEngine* const engine);
dVec2 getPrevCursorPos(const struct GridEngine* const engine);
dVec2 getDeltaCursorPos(const struct GridEngine* const engine);
bool isCursorMoving(const struct GridEngine* const engine);

// TIME FETCHING
double getCurrentTime(const struct GridEngine* const engine);
double getPrevTime(const struct GridEngine* const engine);
double getDeltaTime(const struct GridEngine* const engine);
double dt(const struct GridEngine* const engine); // short-hand for getDeltaTime
double getFPS(const struct GridEngine* const engine);

// CURSOR SHAPE MANAGING
enum CursorShapes getCursorShape(const struct GridEngine* const engine);
void setCursorShape(struct GridEngine* const engine, const enum CursorShapes shape);

// MANAGING ENGINE CANVAS ARRAY
struct CanvasArray getAllCanvases(const struct GridEngine* const engine);

// Allocates canvas to the heap and stores it in engine canvas array
// @param {canvas} gets modified to point to new canvas on heap
// @returns {bool} false if engine canvas array full
bool attachCanvas(struct GridEngine* const engine, const struct CoordinateCanvas* canvas);

// @returns {bool} false if id not found
// NOTE: this destroys the removed canvases
bool removeCanvas(struct GridEngine* const engine, const char* id);

// @returns array of canvases that match the ID
// NOTE: up to the user to check if array has a num of 0
// WARN: modifications to the returned canvases also modify the engine's canvas array
struct CanvasArray getCanvas(const struct GridEngine* const engine, const char* id);


// MANAGING ENGINE GAME OBJECT ARRAY
struct GameObjectArray getAllGameObjects(const struct GridEngine* const engine);

// Allocates object to the heap and stores it in the engine object array
// @param {sizeOfGameObject} since GameObjects extend the base GameObject struct, this is to know how much space to allocate
// @param {gameObject} pointer gets changed to point to new game object on heap
// @returns {bool} false if engine game object array full
bool attachGameObject(struct GridEngine* const engine, const struct GameObject* object, const size_t sizeOfObject);

// @returns {bool} false if id not found
// NOTE: this destroys the removed game objects
bool removeGameObject(struct GridEngine* const engine, const char* id);

// @returns array of game objects that match the ID
// NOTE: up to the user to check if gameObjectArrat has a num of 0 if id not found
// WARN: any altering of the GameObjects in the returned array will impact the original engine GameObjectArray
struct GameObjectArray getGameObject(const struct GridEngine* const engine, const char* id);

