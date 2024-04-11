#pragma once
#include <d3d11.h>

class Graphics;
class Shader;

class Material
{
public:
	Material();
	~Material();

	void SetConstantBufferData(Graphics* graphics, void* data, size_t size);
	void SetShader(std::shared_ptr<Shader> shader) { this->shader = shader; }
	std::shared_ptr<Shader> GetShader() { return shader; }
	ID3D11Buffer* GetConstantBuffer() { return constantBuffer.Get(); }

private:
	void CreateConstantBuffer(Graphics* graphics, void* data, size_t size);
	void UpdateConstantBuffer(Graphics* graphics, void* data, size_t size);

	std::shared_ptr<Shader> shader = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer = nullptr;
};