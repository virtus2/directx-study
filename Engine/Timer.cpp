#include "pch.h"
#include "Timer.h"

Timer::Timer()
{
}

Timer::~Timer()
{
}

int Timer::Initialize()
{
	// 이 시스템이 고성능 타이머를 지원하는지 확인합니다.
	QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
	if (frequency == 0)
	{
		return -1;
	}

	// 주파수 카운터가 밀리초마다 얼마나 많이 틱하는지 알아냅니다.
	ticksPerMs = (float)(frequency / 1000);

	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);

	return 0;
}

void Timer::Tick()
{
	INT64 currentTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

	float timeDifference = (float)(currentTime - startTime);

	deltaTime = timeDifference / ticksPerMs;
	startTime = currentTime;
}

