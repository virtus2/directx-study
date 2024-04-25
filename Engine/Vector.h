#pragma once

#define INLINE __forceinline

#include <DirectXMath.h>
#include <directxtk/SimpleMath.h>
using namespace DirectX;

namespace Math
{
	// TODO: XMFLOAT를 래핑하던지 아예 그냥 삭제할 예정.
	class Vector4;

	class Vector3
	{
	public:


	protected:
		DirectX::SimpleMath::Vector3 vec;
	};


	class Vector4
	{
	public:


	protected:
		DirectX::SimpleMath::Vector4 vec;
	};

	class Vector2
	{
	public:


	protected:
		DirectX::SimpleMath::Vector2 vec;
	};
}