#pragma once
#include "Vector.h"
namespace Math
{
	// TODO: 크로스플랫폼 지원
	using Matrix = DirectX::SimpleMath::Matrix;

	
	static Matrix MatrixFromLookAt(Vector3 eyePosition, Vector3 targetPosition, Vector3 up)
	{
		return XMMatrixLookAtLH(eyePosition, targetPosition, up);
	}

	static Matrix Persepective(float fovY, float aspectRatio, float nearZ, float farZ)
	{
		return XMMatrixPerspectiveFovLH(fovY, aspectRatio, nearZ, farZ);
	}
}