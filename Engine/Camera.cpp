#include "pch.h"
#include "Camera.h"

Camera::Camera()
{
	position = Math::Vector3(0.0f, 0.0f, 0.0f);
	lookAt = Math::Vector3(0.0f, 0.0f, 0.0f);
	view = Math::Matrix::Identity();
	projection = Math::Matrix::Identity();
	cameraConstantBufferData.view = {};
	cameraConstantBufferData.projection = {};
}

void Camera::Update(float deltaTime)
{
	cameraConstantBufferData.view = view.Transpose();
	cameraConstantBufferData.projection = projection.Transpose();
}

void Camera::SetPosition(Math::Vector3 position)
{ 
	this->position = position; 
	SetViewParameters(position, lookAt, upVector);
}

void Camera::SetViewParameters(Math::Vector3 eyePosition, Math::Vector3 eyeDirection, Math::Vector3 up)
{
	position = eyePosition;
	lookDirection = eyeDirection;
	view = Math::Matrix::LookToLH(eyePosition, eyeDirection, up);
}

void Camera::SetProjectionParameters(float fieldOfView, float aspectRatio, float nearPlane, float farPlane)
{
	this->fieldOfView = fieldOfView;
	this->aspectRatio = aspectRatio;
	this->nearPlane = nearPlane;
	this->farPlane = farPlane;
	projection = Math::Matrix::PerspectiveFovLH(XMConvertToRadians(fieldOfView), aspectRatio, nearPlane, farPlane);
}
