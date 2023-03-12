#ifndef _FRUSTUM_H_
#define _FRUSTUM_H_
#include <DirectXMath.h>

using namespace DirectX;

class Frustum
{
public:
	Frustum();
	Frustum(const Frustum&);
	~Frustum();

	void ConstructFrustum(float, XMMATRIX, XMMATRIX);

	bool CheckPoint(float, float, float);
	bool CheckCube(float, float, float, float);
	bool CheckSphere(float, float, float, float);
	bool CheckRectangle(float, float, float, float, float, float);

private:
	XMVECTOR planes[6];
};

#endif