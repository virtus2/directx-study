#pragma once
#ifndef _TIMER_H_
#define _TIMER_H_
#include <windows.h>

class Timer
{
public:
	Timer();
	Timer(const Timer&);
	~Timer();

	bool Initialize();
	void Frame();

	float GetTime();

private:
	INT64 frequency;
	float ticksPerMs;
	INT64 startTime;
	float frameTime;
};

#endif