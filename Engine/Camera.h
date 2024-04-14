#pragma once
#include "Matrix.h"
#include <d3d11.h>

class Camera
{
	struct CameraConstantBuffer
	{
		Math::Matrix view;
		Math::Matrix projection;
	};
	static_assert((sizeof(CameraConstantBuffer) % 16) == 0, "CameraConstantBuffer size must be 16-byte aligned");

public:
	Camera();

	void Update(float deltaTime);

	void SetPosition(Math::Vector3 position);
	void SetViewParameters(Math::Vector3 eyePosition, Math::Vector3 eyeDirection, Math::Vector3 up);
	void SetProjectionParameters(float fieldOfView, float aspectRatio, float nearPlane, float farPlane);

	Math::Vector3 GetPosition() { return position; }
	Math::Vector3 GetLookAt() { return lookAt; }

	// TODO: Graphics에서 Getter로 버퍼, 버퍼 데이터 얻어와서 갱신하는 것 vs Camera에서 직접 갱신하는 것 중 어떤 방법이 더 좋은지 고민
	// 메인카메라만 갱신할것이니 전자가 나을수도 있음
	CameraConstantBuffer* GetCameraConstantBufferData() { return &cameraConstantBufferData; }
	ID3D11Buffer* GetCameraConstantBuffer() { return cameraConstantBuffer.Get(); }
	size_t GetCameraConstantBufferSize() { return sizeof(CameraConstantBuffer); }

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

	CameraConstantBuffer cameraConstantBufferData;
	Microsoft::WRL::ComPtr<ID3D11Buffer> cameraConstantBuffer = nullptr;
};