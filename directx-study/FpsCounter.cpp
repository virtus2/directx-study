#include "FpsCounter.h"

FpsCounter::FpsCounter()
{
}

FpsCounter::FpsCounter(const FpsCounter&)
{
}

FpsCounter::~FpsCounter()
{
}

void FpsCounter::Initialize()
{
	fps = 0;
	count = 0;
	startTime = timeGetTime();
}

void FpsCounter::Frame()
{
	count++;
	if(timeGetTime() >= (startTime + 1000))
	{
		fps = count;
		count = 0;
		startTime = timeGetTime();
	}
}

int FpsCounter::GetFps()
{
	return fps;
}
