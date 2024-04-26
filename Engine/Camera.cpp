#include "pch.h"
#include "Camera.h"

Camera::Camera()
{
	
}

void Camera::BeginFrame(bool firstFrame)
{
	CalculateCameraParameters();

	if (firstFrame) prevCameraData = cameraData;
}

void Camera::Update(float deltaTime)
{
	CalculateCameraParameters();
}

void Camera::SetProjectionParameters(float fieldOfView, float aspectRatio, float nearPlane, float farPlane)
{
	this->fieldOfView = fieldOfView;
	this->aspectRatio = aspectRatio;
	this->nearPlane = nearPlane;
	this->farPlane = farPlane;
}

void Camera::CalculateCameraParameters()
{
	if (prevCameraData.worldPosition != cameraData.worldPosition) isDirty = true;
	if (prevCameraData.targetPosition != cameraData.targetPosition) isDirty = true;
	if (!isDirty) return;

	cameraData.viewMatrix = XMMatrixLookAtLH(cameraData.worldPosition, cameraData.targetPosition, cameraData.up);
	cameraData.projMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(fieldOfView), aspectRatio, nearPlane, farPlane);

	cameraData.viewMatrix = cameraData.viewMatrix.Transpose();
	cameraData.projMatrix = cameraData.projMatrix.Transpose();
	
	prevCameraData = cameraData;
	isDirty = false;
}
