﻿#pragma once

#define INLINE __forceinline

#include <DirectXMath.h>
#include <directxtk/SimpleMath.h>
using namespace DirectX;

namespace Math
{
	// TODO: 크로스플랫폼 지원
	using Vector2 = DirectX::SimpleMath::Vector2;
	using Vector3 = DirectX::SimpleMath::Vector3;
	using Vector4 = DirectX::SimpleMath::Vector4;
}