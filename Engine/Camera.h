#pragma once
#include "Matrix.h"
#include <d3d11.h>

struct CameraData
{
	Math::Matrix viewMatrix;
	// Math::Matrix prevViewMatrix;
	Math::Matrix projMatrix;
	// Math::Matrix viewProjMatrix;
	// Math::Matrix invViewProjMatrix;

	Math::Vector3 worldPosition = Math::Vector3(0.0f, 0.0f, 0.0f);
	float focalLength;
	Math::Vector3 targetPosition = Math::Vector3(0.0f, 0.0f, 0.0f);
	float _padding1;
	Math::Vector3 up = Math::Vector3(0.0f, 1.0f, 0.0f);
	float _padding2;
};
static_assert(sizeof(CameraData) % (sizeof(Math::Vector4)) == 0, "CameraData size should be a multiple of 16");

class Camera
{
	// 1인칭 시점 카메라
	// TODO: 추후에 3인칭 시점이 필요할 경우 Camera를 상속받아서 FirstPersonCamera, ThirdPersonCamera로 분리하던지 다른 방법을 사용한다.
	// 구조는 언리얼 엔진 참고하는게 좋을듯.

	// TODO: DirectX 벡터, 행렬 구조체 대신 래핑한 구조체 사용 (추후 다른 그래픽 API 대비...?)
	struct CameraVertexConstantBuffer
	{
		Math::Matrix view;
		Math::Matrix projection;
	};
	static_assert((sizeof(CameraVertexConstantBuffer) % 16) == 0, "CameraVertexConstantBuffer size must be 16-byte aligned");

public:
	Camera();
	
	void BeginFrame(bool firstFrame);
	void Update(float deltaTime);

	void SetPosition(Math::Vector3& position) { cameraData.worldPosition = position; }
	void SetTargetPosition(Math::Vector3& position) { cameraData.targetPosition = position;	}
	void SetProjectionParameters(float fieldOfView, float aspectRatio, float nearPlane, float farPlane);

	const Math::Vector3& GetPosition() { return cameraData.worldPosition; }
	const Math::Vector3& GetTargetPosition() { return cameraData.targetPosition; }
	const Math::Vector3& GetUpVector() { return cameraData.up; }

	// TODO: Graphics에서 Getter로 버퍼, 버퍼 데이터 얻어와서 갱신하는 것 vs Camera에서 직접 갱신하는 것 중 어떤 방법이 더 좋은지 고민
	// 메인카메라만 갱신할것이니 전자가 나을수도 있음
	const CameraData& GetCameraData() { CalculateCameraParameters(); return cameraData; }
	size_t GetCameraDataSize() { return sizeof(cameraData); }
	ID3D11Buffer* GetCameraDataBuffer() { return cameraDataBuffer.Get(); }

private:
	void CalculateCameraParameters();

	CameraData cameraData;
	CameraData prevCameraData;

	bool isDirty = false;
	float fieldOfView;
	float aspectRatio;
	float nearPlane;
	float farPlane;
	float roll = 0.0f;
	float yaw = 0.0f;

	Microsoft::WRL::ComPtr<ID3D11Buffer> cameraDataBuffer = nullptr;
};