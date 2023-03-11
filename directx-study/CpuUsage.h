#ifndef _CPUUSAGE_H_
#define _CPUUSAGE_H_

#pragma comment(lib, "pdh.lib")

#include <Pdh.h>

class CpuUsage
{
public:
	CpuUsage();
	CpuUsage(const CpuUsage&);
	~CpuUsage();

	void Initialize();
	void Shutdown();
	void Frame();
	int GetCpuPercentage();

private:
	bool canReadCpu;
	HQUERY queryHandle;
	HCOUNTER counterHandle;
	unsigned long lastSampleTime;
	long cpuUsage;
};
#endif