#pragma once
#include <Windows.h>


class FrameTimer
{
public:
	void Init(int framesPerSecond);
	int FramesToUpdate();
private:
	LARGE_INTEGER timer_freq;
	LARGE_INTEGER timer_now;
	LARGE_INTEGER timer_previous;
	int requested_FPS;
	float intervals_per_frames;
	float intervalsSinceLastUpdate;
};

