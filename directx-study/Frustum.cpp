#include "Frustum.h"

Frustum::Frustum()
{
}

Frustum::Frustum(const Frustum&)
{
}

Frustum::~Frustum()
{
}

void Frustum::ConstructFrustum(float screenDepth, XMMATRIX projectionMatrix, XMMATRIX viewMatrix)
{
	XMFLOAT4X4 pMatrix;
	XMStoreFloat4x4(&pMatrix, projectionMatrix);
	// Calculate the minimum Z distance in the frustum.
	float zMinimum = -pMatrix._43 / pMatrix._33;
	float r = screenDepth / (screenDepth - zMinimum);

	pMatrix._33 = r;
	pMatrix._43 = -r * zMinimum;
	projectionMatrix = XMLoadFloat4x4(&pMatrix);
	
	XMMATRIX finalMatrix = XMMatrixMultiply(viewMatrix, projectionMatrix);
	XMFLOAT4X4 matrix;
	XMStoreFloat4x4(&matrix, finalMatrix);

	// Calculate near plane of frustum.
	float x = matrix._14 + matrix._13;
	float y = matrix._24 + matrix._23;
	float z = matrix._34 + matrix._33;
	float w = matrix._44 + matrix._43;
	planes[0] = XMVectorSet(x, y, z, w);
	planes[0] = XMPlaneNormalize(planes[0]);

	// Calculate far plane of frustum.
	x = matrix._14 + matrix._13;
	y = matrix._24 + matrix._23;
	z = matrix._34 + matrix._33;
	w = matrix._44 + matrix._43;
	planes[1] = XMVectorSet(x, y, z, w);
	planes[1] = XMPlaneNormalize(planes[1]);

	// Calculate left plane of frustum.
	x = matrix._14 + matrix._11;
	y = matrix._24 + matrix._21;
	z = matrix._34 + matrix._31;
	w = matrix._44 + matrix._41;
	planes[2] = XMVectorSet(x, y, z, w);
	planes[2] = XMPlaneNormalize(planes[2]);

	// Calculate left plane of frustum.
	x = matrix._14 + matrix._11;
	y = matrix._24 + matrix._21;
	z = matrix._34 + matrix._31;
	w = matrix._44 + matrix._41;
	planes[2] = XMVectorSet(x, y, z, w);
	planes[2] = XMPlaneNormalize(planes[2]);

	// Calculate right plane of frustum.
	x = matrix._14 + matrix._11;
	y = matrix._24 + matrix._21;
	z = matrix._34 + matrix._31;
	w = matrix._44 + matrix._41;
	planes[3] = XMVectorSet(x, y, z, w);
	planes[3] = XMPlaneNormalize(planes[3]);

	// Calculate top plane of frustum.
	x = matrix._14 + matrix._12;
	y = matrix._24 + matrix._22;
	z = matrix._34 + matrix._32;
	w = matrix._44 + matrix._42;
	planes[4] = XMVectorSet(x, y, z, w);
	planes[4] = XMPlaneNormalize(planes[4]);

	// Calculate bottom plane of frustum.
	x = matrix._14 + matrix._12;
	y = matrix._24 + matrix._22;
	z = matrix._34 + matrix._32;
	w = matrix._44 + matrix._42;
	planes[5] = XMVectorSet(x, y, z, w);
	planes[5] = XMPlaneNormalize(planes[5]);
}

bool Frustum::CheckPoint(float x, float y, float z)
{
	// Check if the point is inside all six planes of the view frustum.
	for (int i = 0; i < 6; i++)
	{
		if (XMVectorGetX(XMPlaneDotCoord(planes[i], XMVectorSet(x, y, z, 1.0f))) < 0.0f)
		{
			return false;
		}
		return true;
	}
}

bool Frustum::CheckCube(float xCenter, float yCenter, float zCenter, float radius)
{
	// Check if any one point of the cube is in the view frustum.
	for (int i = 0; i < 6; i++)
	{
		if (XMVectorGetX(XMPlaneDotCoord(planes[i], XMVectorSet((xCenter - radius), (yCenter - radius), (zCenter - radius), 1.0f))) >= 0.0f)
		{
			continue;
		}
		if (XMVectorGetX(XMPlaneDotCoord(planes[i], XMVectorSet((xCenter + radius), (yCenter - radius), (zCenter - radius), 1.0f))) >= 0.0f)
		{
			continue;
		}
		if (XMVectorGetX(XMPlaneDotCoord(planes[i], XMVectorSet((xCenter - radius), (yCenter + radius), (zCenter - radius), 1.0f))) >= 0.0f)
		{
			continue;
		}
		if (XMVectorGetX(XMPlaneDotCoord(planes[i], XMVectorSet((xCenter + radius), (yCenter + radius), (zCenter - radius), 1.0f))) >= 0.0f)
		{
			continue;
		}
		if (XMVectorGetX(XMPlaneDotCoord(planes[i], XMVectorSet((xCenter - radius), (yCenter - radius), (zCenter + radius), 1.0f))) >= 0.0f)
		{
			continue;
		}
		if (XMVectorGetX(XMPlaneDotCoord(planes[i], XMVectorSet((xCenter + radius), (yCenter - radius), (zCenter + radius), 1.0f))) >= 0.0f)
		{
			continue;
		}
		if (XMVectorGetX(XMPlaneDotCoord(planes[i], XMVectorSet((xCenter - radius), (yCenter + radius), (zCenter + radius), 1.0f))) >= 0.0f)
		{
			continue;
		}
		if (XMVectorGetX(XMPlaneDotCoord(planes[i], XMVectorSet((xCenter + radius), (yCenter + radius), (zCenter + radius), 1.0f))) >= 0.0f)
		{
			continue;
		}
		return false;
	}
	return true;
}

bool Frustum::CheckSphere(float xCenter, float yCenter, float zCenter, float radius)
{
	// Check if the radius of the sphere is inside the view frustum.
	for (int i = 0; i < 6; i++)
	{
		if(XMVectorGetX(XMPlaneDotCoord(planes[i], XMVectorSet(xCenter, yCenter, zCenter, 1.0f))) < -radius)
		{
			return false;
		}
	}
	return true;
}

bool Frustum::CheckRectangle(float xCenter, float yCenter, float zCenter, float xSize, float ySize, float zSize)
{
	// Check if any of the 6 planes of the rectangle are inside the view frustum.
	for (int i = 0; i < 6; i++)
	{
		if (XMVectorGetX(XMPlaneDotCoord(planes[i], XMVectorSet(xCenter - xSize, yCenter - ySize, zCenter - zSize, 1.0f))) >= 0.0f)
		{
			continue;
		}

		if (XMVectorGetX(XMPlaneDotCoord(planes[i], XMVectorSet(xCenter + xSize, yCenter - ySize, zCenter - zSize, 1.0f))) >= 0.0f)
		{
			continue;
		}

		if (XMVectorGetX(XMPlaneDotCoord(planes[i], XMVectorSet(xCenter - xSize, yCenter + ySize, zCenter - zSize, 1.0f))) >= 0.0f)
		{
			continue;
		}

		if (XMVectorGetX(XMPlaneDotCoord(planes[i], XMVectorSet(xCenter - xSize, yCenter - ySize, zCenter + zSize, 1.0f))) >= 0.0f)
		{
			continue;
		}

		if (XMVectorGetX(XMPlaneDotCoord(planes[i], XMVectorSet(xCenter + xSize, yCenter + ySize, zCenter - zSize, 1.0f))) >= 0.0f)
		{
			continue;
		}

		if (XMVectorGetX(XMPlaneDotCoord(planes[i], XMVectorSet(xCenter + xSize, yCenter - ySize, zCenter + zSize, 1.0f))) >= 0.0f)
		{
			continue;
		}

		if (XMVectorGetX(XMPlaneDotCoord(planes[i], XMVectorSet(xCenter - xSize, yCenter + ySize, zCenter + zSize, 1.0f))) >= 0.0f)
		{
			continue;
		}

		if (XMVectorGetX(XMPlaneDotCoord(planes[i], XMVectorSet(xCenter + xSize, yCenter + ySize, zCenter + zSize, 1.0f))) >= 0.0f)
		{
			continue;
		}
		return false;
	}
	return true;
}
