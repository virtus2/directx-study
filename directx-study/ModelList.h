#ifndef _MODELLIST_H_
#define _MODELLIST_H_

#include "DirectXMath.h"
#include <stdlib.h>
#include <time.h>

using namespace DirectX;

class ModelList
{
private:
	struct ModelInfoType
	{
		XMFLOAT4 color;
		float positionX, positionY, positionZ;
	};

public:
	ModelList();
	ModelList(const ModelList&);
	~ModelList();

	bool Initialize(int);
	void Shutdown();

	int GetModelCount();
	void GetData(int, float&, float&, float&, XMFLOAT4&);

private:
	int modelCount;
	ModelInfoType* modelInfoList;
};

#endif