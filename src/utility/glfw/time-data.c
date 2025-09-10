#include "utility/glfw/time-data.h"

#include "glfw.h"
#include "engine/grid-engine-options.h"

struct TimeData timeData()
{
	struct TimeData timeData;
	timeData.currentTime = glfwGetTime();
	timeData.prevTime = 0;
	timeData.dt = timeData.currentTime - timeData.prevTime;
	timeData.FPS = GRID_MAX_FPS;

	return timeData;
}

void timeDataUpdate(struct TimeData* const timeData)
{
	timeData->prevTime = timeData->currentTime;
	timeData->currentTime = glfwGetTime();
	timeData->dt = timeData->currentTime - timeData->prevTime;
	timeData->FPS = 1.0 / timeData->dt;
}
