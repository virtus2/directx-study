#pragma once
#include "Matrix.h"
#include <d3d11.h>

class Camera
{
	// 1인칭 시점 카메라
	// TODO: 추후에 3인칭 시점이 필요할 경우 Camera를 상속받아서 FirstPersonCamera, ThirdPersonCamera로 분리하던지 다른 방법을 사용한다.
	// 구조는 언리얼 엔진 참고하는게 좋을듯.

	// TODO: DirectX 벡터, 행렬 구조체 대신 래핑한 구조체 사용 (추후 다른 그래픽 API 대비...?)
	struct CameraVertexConstantBuffer
	{
		XMMATRIX view;
		XMMATRIX projection;
	};
	static_assert((sizeof(CameraVertexConstantBuffer) % 16) == 0, "CameraVertexConstantBuffer size must be 16-byte aligned");

public:
	Camera();

	void Update(float deltaTime);

	void SetPosition(XMFLOAT3 position);
	void SetLookDirection(XMFLOAT3 direction);
	void SetViewParameters(XMFLOAT3 eyePosition, XMFLOAT3 eyeDirection, XMFLOAT3 up);
	void SetProjectionParameters(float fieldOfView, float aspectRatio, float nearPlane, float farPlane);

	XMFLOAT3 GetPosition() { return position; }
	XMFLOAT3 GetLookAt() { return lookAt; }
	XMFLOAT3 GetLookDirection() { return lookDirection; }
	XMFLOAT3 GetUpVector() { return upVector; }
	XMFLOAT3 GetRightVector() { return rightVector; }

	// TODO: Graphics에서 Getter로 버퍼, 버퍼 데이터 얻어와서 갱신하는 것 vs Camera에서 직접 갱신하는 것 중 어떤 방법이 더 좋은지 고민
	// 메인카메라만 갱신할것이니 전자가 나을수도 있음
	CameraVertexConstantBuffer* GetCameraVertexConstantBufferData() { return &cameraVertexConstantBufferData; }
	ID3D11Buffer* GetCameraVertexConstantBuffer() { return cameraVertexConstantBuffer.Get(); }
	size_t GetCameraVertexConstantBufferSize() { return sizeof(CameraVertexConstantBuffer); }

private:
	XMFLOAT3 upVector = XMFLOAT3(0.0f, 1.0f, 0.0f);
	XMFLOAT3 rightVector = XMFLOAT3(1.0f, 0.0f, 0.0f);

	XMFLOAT3 position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 lookAt = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 lookDirection = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMMATRIX view = XMMATRIX();
	XMMATRIX projection = XMMATRIX();

	float fieldOfView;
	float aspectRatio;
	float nearPlane;
	float farPlane;
	float roll = 0.0f;
	float yaw = 0.0f;

	CameraVertexConstantBuffer cameraVertexConstantBufferData;
	Microsoft::WRL::ComPtr<ID3D11Buffer> cameraVertexConstantBuffer = nullptr;
};