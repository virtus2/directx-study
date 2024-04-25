#include "pch.h"
#include "Camera.h"

Camera::Camera()
{
	position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	lookAt = XMFLOAT3(0.0f, 0.0f, 0.0f);
	view = XMMATRIX();
	projection = XMMATRIX();
	cameraVertexConstantBufferData.view = {};
	cameraVertexConstantBufferData.projection = {};
}

void Camera::Update(float deltaTime)
{
	XMVECTOR eyePos = XMLoadFloat3(&position);
	XMVECTOR eyeDir = XMLoadFloat3(&lookDirection);
	XMVECTOR upDir = XMLoadFloat3(&upVector);
	XMVECTOR newRightDir = XMVector3Cross(upDir, eyeDir);
	XMStoreFloat3(&rightVector, newRightDir);

	view = DirectX::XMMatrixLookToLH(eyePos, eyeDir, upDir);
	projection = DirectX::XMMatrixPerspectiveFovLH(XMConvertToRadians(fieldOfView), aspectRatio, nearPlane, farPlane);

	cameraVertexConstantBufferData.view = DirectX::XMMatrixTranspose(view);
	cameraVertexConstantBufferData.projection = DirectX::XMMatrixTranspose(projection);
}

void Camera::SetPosition(XMFLOAT3 position)
{ 
	this->position = position;
}

void Camera::SetLookDirection(XMFLOAT3 direction)
{
	lookDirection = direction;
	SetViewParameters(position, lookDirection, upVector);
}

void Camera::SetViewParameters(XMFLOAT3 eyePosition, XMFLOAT3 eyeDirection, XMFLOAT3 up)
{
	position = eyePosition;
	lookDirection = eyeDirection;
}

void Camera::SetProjectionParameters(float fieldOfView, float aspectRatio, float nearPlane, float farPlane)
{
	this->fieldOfView = fieldOfView;
	this->aspectRatio = aspectRatio;
	this->nearPlane = nearPlane;
	this->farPlane = farPlane;
}
