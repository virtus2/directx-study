#pragma once
#include "Matrix.h"

class Camera
{
public:
	Camera();

	void Update(float deltaTime);

	void SetPosition(Math::Vector3 position);
	void SetViewParameters(Math::Vector3 eyePosition, Math::Vector3 eyeDirection, Math::Vector3 up);
	void SetProjectionParameters(float fieldOfView, float aspectRatio, float nearPlane, float farPlane);

	Math::Vector3 GetPosition() { return position; }
	Math::Vector3 GetLookAt() { return lookAt; }
	Math::Matrix GetView() { return view; }
	Math::Matrix GetProjection() { return projection; }

private:
	const Math::Vector3 upVector = Math::Vector3(0.0f, 1.0f, 0.0f);

	Math::Vector3 position;
	Math::Vector3 lookAt;
	Math::Vector3 lookDirection;
	Math::Matrix view;
	Math::Matrix projection;

	float fieldOfView;
	float aspectRatio;
	float nearPlane;
	float farPlane;
};