#pragma once

struct TimeData {
	double currentTime;
	double prevTime;
	double dt;
	double FPS;
};

struct TimeData timeData();

void timeDataUpdate(struct TimeData* const timeData);
