#pragma once

class Timer
{
public:
	Timer();
	~Timer();

	int Initialize();
	void Tick();
	float GetDeltaTime() { return deltaTime; }

private:
	INT64 frequency;
	float ticksPerMs;
	INT64 startTime;
	float deltaTime;
};