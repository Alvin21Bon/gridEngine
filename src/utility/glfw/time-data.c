#include "../../../include/engine/grid-engine.h"

struct TimeData timeData()
{
	struct TimeData timeData;
	timeData.currentTime = glfwGetTime();
	timeData.previousTime = 0;
	timeData.dt = timeData.currentTime - timeData.previousTime;
	timeData.FPS = GRID_MAX_FPS;

	return timeData;
}

void timeDataUpdate(struct TimeData* const timeData)
{
	timeData->previousTime = timeData->currentTime;
	timeData->currentTime = glfwGetTime();
	timeData->dt = timeData->currentTime - timeData->previousTime;
	timeData->FPS = 1.0 / timeData->dt;
}
