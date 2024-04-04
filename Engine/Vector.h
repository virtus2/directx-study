#pragma once

#define INLINE __forceinline

#include <DirectXMath.h>

using namespace DirectX;

namespace Math
{
	class Vector4;

	class Vector3
	{
	public:
		INLINE Vector3() {}
		INLINE Vector3(FXMVECTOR v) { vec = v; }
		INLINE Vector3(float x, float y, float z) { vec = XMVectorSet(x, y, z, 0.0f); }
		INLINE Vector3(const Vector3& v) { vec = v; }

		INLINE operator XMVECTOR() const { return vec; }

		// Comparision operators
		INLINE bool operator == (const Vector3& v) const { return XMVector3Equal(vec, v.vec); }
		INLINE bool operator != (const Vector3& v) const { return XMVector3NotEqual(vec, v.vec); }

		// Assignment operators
		INLINE Vector3& operator= (const Vector3& v) { vec = v; return *this; }
		INLINE Vector3& operator+= (const Vector3& v) { vec = XMVectorAdd(vec, v); return *this; }
		INLINE Vector3& operator-= (const Vector3& v) { vec = XMVectorSubtract(vec, v); return *this; }
		INLINE Vector3& operator*= (const Vector3& v) { vec = XMVectorMultiply(vec, v); return *this; }
		INLINE Vector3& operator*= (float s) { vec = XMVectorScale(vec, s); return *this; }
		INLINE Vector3& operator/= (const Vector3& v) { vec = XMVectorDivide(vec, v); return *this; }
		INLINE Vector3& operator/= (float s) { vec = XMVectorScale(vec, 1.0f / s); return *this; }

		// Unary operators
		INLINE Vector3 operator+ () const { return *this; }
		INLINE Vector3 operator- () const { return Vector3(XMVectorNegate(vec)); }

		// Vector operations
		INLINE bool InBounds(const Vector3& Bounds) const { return XMVector3InBounds(vec, Bounds); }
		INLINE float Length() const { return XMVectorGetX(XMVector3Length(vec)); }
		INLINE float LengthSquared() const { return XMVectorGetX(XMVector3LengthSq(vec)); }
		INLINE Vector3 Normalize() const { return XMVector3Normalize(vec); }
		INLINE Vector3 Clamp(const Vector3& vmin, const Vector3& vmax) const { return XMVectorClamp(vec, vmin, vmax); }

	protected:
		XMVECTOR vec;
	};


	class Vector4
	{
	public:
		INLINE Vector4() {}
		INLINE Vector4(FXMVECTOR v) { vec = v; }
		INLINE Vector4(float x, float y, float z, float w) { vec = XMVectorSet(x, y, z, w); }
		INLINE Vector4(const Vector4& v) { vec = v; }
		INLINE Vector4(const Vector3& v, float w) { vec = XMVectorSetW(v, w); }

		INLINE operator XMVECTOR() const { return vec; }

		// Comparision operators
		INLINE bool operator == (const Vector4& v) const { return XMVector4Equal(vec, v.vec); }
		INLINE bool operator != (const Vector4& v) const { return XMVector4NotEqual(vec, v.vec); }

		// Assignment operators
		INLINE Vector4& operator= (const Vector4& v) { vec = v; return *this; }
		INLINE Vector4& operator+= (const Vector4& v) { vec = XMVectorAdd(vec, v); return *this; }
		INLINE Vector4& operator-= (const Vector4& v) { vec = XMVectorSubtract(vec, v); return *this; }
		INLINE Vector4& operator*= (const Vector4& v) { vec = XMVectorMultiply(vec, v); return *this; }
		INLINE Vector4& operator*= (float s) { vec = XMVectorScale(vec, s); return *this; }
		INLINE Vector4& operator/= (const Vector4& v) { vec = XMVectorDivide(vec, v); return *this; }
		INLINE Vector4& operator/= (float s) { vec = XMVectorScale(vec, 1.0f / s); return *this; }

		// Unary operators
		INLINE Vector4 operator+ () const { return *this; }
		INLINE Vector4 operator- () const { return Vector4(XMVectorNegate(vec)); }

		// Vector operations
		INLINE bool InBounds(const Vector4& Bounds) const { return XMVector4InBounds(vec, Bounds); }
		INLINE float Length() const { return XMVectorGetX(XMVector4Length(vec)); }
		INLINE float LengthSquared() const { return XMVectorGetX(XMVector4LengthSq(vec)); }
		INLINE Vector4 Normalize() const { return XMVector4Normalize(vec); }
		INLINE Vector4 Clamp(const Vector4& vmin, const Vector4& vmax) const { return XMVectorClamp(vec, vmin, vmax); }

	protected:
		XMVECTOR vec;
	};

	class Vector2
	{
	public:
INLINE Vector2() {}
		INLINE Vector2(FXMVECTOR v) { vec = v; }
		INLINE Vector2(float x, float y) { vec = XMVectorSet(x, y, 0.0f, 0.0f); }
		INLINE Vector2(const Vector2& v) { vec = v; }

		INLINE operator XMVECTOR() const { return vec; }

		// Comparision operators
		INLINE bool operator == (const Vector2& v) const { return XMVector2Equal(vec, v.vec); }
		INLINE bool operator != (const Vector2& v) const { return XMVector2NotEqual(vec, v.vec); }

		// Assignment operators
		INLINE Vector2& operator= (const Vector2& v) { vec = v; return *this; }
		INLINE Vector2& operator+= (const Vector2& v) { vec = XMVectorAdd(vec, v); return *this; }
		INLINE Vector2& operator-= (const Vector2& v) { vec = XMVectorSubtract(vec, v); return *this; }
		INLINE Vector2& operator*= (const Vector2& v) { vec = XMVectorMultiply(vec, v); return *this; }
		INLINE Vector2& operator*= (float s) { vec = XMVectorScale(vec, s); return *this; }
		INLINE Vector2& operator/= (const Vector2& v) { vec = XMVectorDivide(vec, v); return *this; }
		INLINE Vector2& operator/= (float s) { vec = XMVectorScale(vec, 1.0f / s); return *this; }

		// Unary operators
		INLINE Vector2 operator+ () const { return *this; }
		INLINE Vector2 operator- () const { return Vector2(XMVectorNegate(vec)); }

		// Vector operations
		INLINE bool InBounds(const Vector2& Bounds) const { return XMVector2InBounds(vec, Bounds); }
		INLINE float Length() const { return XMVectorGetX(XMVector2Length(vec)); }
		INLINE float LengthSquared() const { return XMVectorGetX(XMVector2LengthSq(vec)); }
		INLINE Vector2 Normalize() const { return XMVector2Normalize(vec); }
		INLINE Vector2 Clamp(const Vector2& vmin, const Vector2& vmax) const { return XMVectorClamp(vec, vmin, vmax); }

	protected:
		XMVECTOR vec;
	};
}