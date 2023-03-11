#ifndef _FPSCOUNTER_H_
#define _FPSCOUNTER_H_

#pragma comment(lib, "winmm.lib")

#include <Windows.h>
#include <mmsystem.h>

class FpsCounter
{
public:
	FpsCounter();
	FpsCounter(const FpsCounter&);
	~FpsCounter();

	void Initialize();
	void Frame();
	int GetFps();

private:
	int fps, count;
	unsigned long startTime;
};

#endif