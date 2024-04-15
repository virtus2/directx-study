#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

class Light
{
	struct LightConstantBuffer
	{
		XMFLOAT3 cameraPosition;
		float padding0;
		XMFLOAT3 lightPosition;
		float padding1;
		XMFLOAT3 lightDirection;
		float padding2;
	};
	static_assert((sizeof(LightConstantBuffer) % 16) == 0, "LightConstantBuffer size must be 16-byte aligned");

public:
	Light() {}
	~Light() {}

	void Update()
	{
		lightConstantBufferData.cameraPosition = cameraPosition;
		lightConstantBufferData.lightPosition = lightPosition;
		lightConstantBufferData.lightDirection = lightDirection;
	}

	void SetCameraPosition(XMFLOAT3 position) { cameraPosition = position; }
	void SetLightPosition(XMFLOAT3 position) { lightPosition = position; }
	void SetLightDirection(XMFLOAT3 direction) { lightDirection = direction; }

	XMFLOAT3 GetLightPosition() { return lightPosition; }
	XMFLOAT3 GetLightDirection() { return lightDirection; }

	LightConstantBuffer* GetLightConstantBufferData() { return &lightConstantBufferData; }
	ID3D11Buffer* GetLightConstantBuffer() { return lightConstantBuffer.Get(); }
	size_t GetLightConstantBufferSize() { return sizeof(LightConstantBuffer); }

private:
	XMFLOAT3 cameraPosition = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 lightPosition = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 lightDirection = XMFLOAT3(0.0f, 0.0f, 0.0f);

	LightConstantBuffer lightConstantBufferData;
	Microsoft::WRL::ComPtr<ID3D11Buffer> lightConstantBuffer = nullptr;
};