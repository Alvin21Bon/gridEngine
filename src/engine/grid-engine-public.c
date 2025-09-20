#include "engine/grid-engine-public.h"

#include "engine/grid-engine.h"
#include "utility/glfw/cursor-shape-manager.h"
#include "utility/glfw/input-data.h"
#include <sys/types.h>
#include <stddef.h>
#include <lina/lina.h>

// INPUT FETCHING
bool isKeyDown(const struct GridEngine* const engine, const int key) { return inputDataIsKeyDown(&engine->window.input, key); }
ulong getNumFramesKeyPressed(const struct GridEngine* const engine, const int key) { return inputDataGetKeyFramesPressed(&engine->window.input, key); }
ulong getNumFramesKeyReleased(const struct GridEngine* const engine, const int key) { return inputDataGetKeyFramesReleased(&engine->window.input, key); }
bool isKeyFirstFramePressed(const struct GridEngine* const engine, const int key) { return inputDataIsKeyFirstFramePressed(&engine->window.input, key); }
bool isMouseButtonDown(const struct GridEngine* const engine, const int button) { return inputDataIsMouseButtonDown(&engine->window.input, button); }
ulong getNumFramesMouseButtonPressed(const struct GridEngine* const engine, const int button) { return inputDataGetMouseButtonFramesPressed(&engine->window.input, button); }
ulong getNumFramesMouseButtonReleased(const struct GridEngine* const engine, const int button) { return inputDataGetMouseButtonFramesReleased(&engine->window.input, button); }
bool isMouseButtonFirstFramePressed(const struct GridEngine* const engine, const int button) { return inputDataIsMouseButtonFirstFramePressed(&engine->window.input, button); }
dVec2 getCursorPos(const struct GridEngine* const engine) { return engine->window.input.mouse.cursorPos; }
dVec2 getPrevCursorPos(const struct GridEngine* const engine) { return engine->window.input.mouse.prevCursorPos; }
dVec2 getDeltaCursorPos(const struct GridEngine* const engine) { return engine->window.input.mouse.deltaCursorPos; }
bool isCursorMoving(const struct GridEngine* const engine) { return !dvec2Equals(engine->window.input.mouse.deltaCursorPos, dvec2Fill(0)); }

// TIME FETCHING
double getCurrentTime(const struct GridEngine* const engine) { return engine->window.time.currentTime; }
double getPrevTime(const struct GridEngine* const engine) { return engine->window.time.prevTime; }
double getDeltaTime(const struct GridEngine* const engine) { return engine->window.time.dt; }
double dt(const struct GridEngine* const engine) { return engine->window.time.dt; }
double getFPS(const struct GridEngine* const engine) { return engine->window.time.FPS; }

// CURSOR SHAPE MANAGING
enum CursorShapes getCursorShape(const struct GridEngine* const engine) { return engine->window.cursorShapeManager.activeShape; }
void setCursorShape(struct GridEngine* const engine, const enum CursorShapes shape) { cursorSetShape(&engine->window.cursorShapeManager, engine->window.windowPointer, shape); }

// MANAGING ENGINE CANVAS ARRAY
struct CanvasArray getAllCanvases(const struct GridEngine* const engine) { return engine->canvasArray; }
bool attachCanvas(struct GridEngine* const engine, const struct CoordinateCanvas* canvas) { return canvasArrayAddHeapCopy(&engine->canvasArray, canvas); }
bool removeCanvas(struct GridEngine* const engine, const char* id) { return canvasArrayRemove(&engine->canvasArray, id); }
struct CanvasArray getCanvas(const struct GridEngine* const engine, const char* id) { return canvasArrayGet(&engine->canvasArray, id); }

// MANAGING ENGINE GAME OBJECT ARRAY
struct GameObjectArray getAllGameObjects(const struct GridEngine* const engine) { return engine->gameObjectArray; }
bool attachGameObject(struct GridEngine* const engine, const struct GameObject* object, const size_t sizeOfObject) { return gameObjectArrayAddHeapCopy(&engine->gameObjectArray, object, sizeOfObject); }
bool removeGameObject(struct GridEngine* const engine, const char* id) { return gameObjectArrayRemove(&engine->gameObjectArray, id, engine); }
struct GameObjectArray getGameObject(const struct GridEngine* const engine, const char* id) { return gameObjectArrayGet(&engine->gameObjectArray, id); }

