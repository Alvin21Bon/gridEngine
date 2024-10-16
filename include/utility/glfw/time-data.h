#pragma once

#include "../shared.h"

struct TimeData {
	double currentTime;
	double previousTime;
	double dt;
	double FPS;
};

struct TimeData timeData();

void timeDataUpdate(struct TimeData* const timeData);
