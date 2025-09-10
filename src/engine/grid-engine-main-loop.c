#include "engine/grid-engine-main-loop.h"

#include "canvas/canvas-array.h"
#include "engine/grid-engine.h"
#include "engine/grid-engine-options.h"
#include "engine/grid-engine-states.h"
#include "engine/grid-engine-globals.h"
#include "utility/opengl/glad.h"
#include "utility/color.h"
#include "utility/logging.h"
#include "glfw.h"
#include <sys/types.h>

ulong GRID_FRAME_COUNTER = 0;
static void waitOnMaxFPS(double startingTime);

int main()
{
	struct GridEngine engine = gridEngine();
	const enum GridEngineStates endingEngineState = enterEngineLoop(&engine);

	if (endingEngineState == GRID_ENGINE_ERROR)
		LOG(GRID_LOGGING_ERROR, __func__, __LINE__, "engine error\n");

	gridEngineDestroy(&engine);
	return endingEngineState;
}

void gridEngineRender(struct GridEngine* const engine)
{
	Color clearColor = GRID_CLEAR_COLOR;

	glClearColor(clearColor.r, clearColor.g, clearColor.b, (GLfloat)1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	canvasArrayDraw(&engine->canvasArray);

	glfwSwapBuffers(engine->window.windowPointer);
}
enum GridEngineStates enterEngineLoop(struct GridEngine* const engine)
{
	while (!glfwWindowShouldClose(engine->window.windowPointer))
	{
		GRID_FRAME_COUNTER++;

		engine->state = engineTick(engine);
		if (engine->state == GRID_ENGINE_ERROR || engine->state == GRID_ENGINE_SUCCESS) return engine->state;

		gridEngineRender(engine);

		// enter spin cycle until FPS is synced with max FPS
		waitOnMaxFPS(engine->window.time.currentTime);
	}

	return GRID_ENGINE_SUCCESS;
}

static void waitOnMaxFPS(double startingTime)
{
	double currentTime, currentDeltaTime, currentFPS;
	do
	{
		currentTime = glfwGetTime();
		currentDeltaTime = currentTime - startingTime;
		currentFPS = 1.0 / currentDeltaTime;
	} while (currentFPS > GRID_MAX_FPS);
}

