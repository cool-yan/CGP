#include "FrameTimer.h"

void FrameTimer::Init(int framesPerSecond) {
	QueryPerformanceFrequency(&timer_freq);
	QueryPerformanceCounter(&timer_now);
	QueryPerformanceCounter(&timer_previous);

	requested_FPS = framesPerSecond;
	intervals_per_frames = (float)timer_freq.QuadPart / framesPerSecond;
}

int FrameTimer::FramesToUpdate() {
	int frames_to_update;
	QueryPerformanceCounter(&timer_now);

	intervalsSinceLastUpdate = timer_now.QuadPart - timer_previous.QuadPart;
	frames_to_update = (int)(intervalsSinceLastUpdate / intervals_per_frames);
	if (frames_to_update != 0)
		QueryPerformanceCounter(&timer_previous);
	
	return frames_to_update;
}