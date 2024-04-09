#pragma once
#include "Vector.h"
namespace Math
{
	class Matrix
	{
	public:
		Matrix() {}
		Matrix(FXMMATRIX m) { mat = m; }
		Matrix(const Matrix& m) { mat = m.mat; }
		Matrix(float m11, float m12, float m13, float m14,
			float m21, float m22, float m23, float m24,
			float m31, float m32, float m33, float m34,
			float m41, float m42, float m43, float m44)
		{
			mat = XMMatrixSet(m11, m12, m13, m14,
								m21, m22, m23, m24,
								m31, m32, m33, m34,
								m41, m42, m43, m44);
		}

		operator XMMATRIX() const { return mat; }

		// Assignment operators
		INLINE Matrix& operator= (const Matrix& m) { mat = m.mat; return *this; }
		INLINE Matrix& operator+= (const Matrix& m) { mat += m; return *this; }
		INLINE Matrix& operator-= (const Matrix& m) { mat -= m; return *this; }
		INLINE Matrix& operator*= (const Matrix& m) { mat = XMMatrixMultiply(mat, m); return *this; }
		INLINE Matrix& operator*= (float s) { mat *= s; return *this; }
		INLINE Matrix& operator/= (const Matrix& m) { mat = XMMatrixMultiply(mat, XMMatrixInverse(nullptr, m)); return *this; }
		INLINE Matrix& operator/= (float s) { mat /= s; return *this; }

		// Unary operators
		INLINE Matrix operator+ () const { return *this; }
		INLINE Matrix operator- () const { return  -mat; }
		
		// Matrix operations
		INLINE Matrix Transpose() const { return XMMatrixTranspose(mat); }
		INLINE Matrix Inverse() const { return XMMatrixInverse(nullptr, mat); }

		// Static functions
		INLINE static Matrix Identity() { return XMMatrixIdentity(); }
		INLINE static Matrix Translation(float x, float y, float z) { return XMMatrixTranslation(x, y, z); }
		INLINE static Matrix Translation(const Vector3& v) { return XMMatrixTranslationFromVector(v); }
		INLINE static Matrix Scaling(float x, float y, float z) { return XMMatrixScaling(x, y, z); }
		INLINE static Matrix Scaling(const Vector3& v) { return XMMatrixScalingFromVector(v); }
		INLINE static Matrix RotationX(float angle) { return XMMatrixRotationX(angle); }
		INLINE static Matrix RotationY(float angle) { return XMMatrixRotationY(angle); }
		INLINE static Matrix RotationZ(float angle) { return XMMatrixRotationZ(angle); }
		INLINE static Matrix RotationRollPitchYaw(float pitch, float yaw, float roll) { return XMMatrixRotationRollPitchYaw(pitch, yaw, roll); }
		INLINE static Matrix RotationQuaternion(const Vector4& q) { return XMMatrixRotationQuaternion(q); }
		INLINE static Matrix RotationAxis(const Vector3& axis, float angle) { return XMMatrixRotationAxis(axis, angle); }
		INLINE static Matrix PerspectiveFovLH(float fov, float aspect, float nearZ, float farZ) { return XMMatrixPerspectiveFovLH(fov, aspect, nearZ, farZ); }
		INLINE static Matrix PerspectiveFovRH(float fov, float aspect, float nearZ, float farZ) { return XMMatrixPerspectiveFovRH(fov, aspect, nearZ, farZ); }
		INLINE static Matrix PerspectiveLH(float width, float height, float nearZ, float farZ) { return XMMatrixPerspectiveLH(width, height, nearZ, farZ); }
		INLINE static Matrix PerspectiveRH(float width, float height, float nearZ, float farZ) { return XMMatrixPerspectiveRH(width, height, nearZ, farZ); }
		INLINE static Matrix OrthographicLH(float width, float height, float nearZ, float farZ) { return XMMatrixOrthographicLH(width, height, nearZ, farZ); }
		INLINE static Matrix OrthographicRH(float width, float height, float nearZ, float farZ) { return XMMatrixOrthographicRH(width, height, nearZ, farZ); }
		INLINE static Matrix LookAtLH(const Vector3& eye, const Vector3& at, const Vector3& up) { return XMMatrixLookAtLH(eye, at, up); }
		INLINE static Matrix LookAtRH(const Vector3& eye, const Vector3& at, const Vector3& up) { return XMMatrixLookAtRH(eye, at, up); }

	protected:
		XMMATRIX mat;
	};
}