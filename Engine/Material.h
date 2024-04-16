#pragma once
#include <d3d11.h>
#include <unordered_map>
#include "Texture.h"

class Graphics;
class Shader;

class Material
{
	friend class ModelLoader;
public:
	Material();
	~Material();

	void LoadTexture(Graphics* graphics);

	void SetConstantBufferData(Graphics* graphics, void* data, size_t size);
	void SetShader(std::shared_ptr<Shader> shader) { this->shader = shader; }

	std::shared_ptr<Texture> GetTexture(Texture::TextureType type) { return textures[type]; }
	std::shared_ptr<Shader> GetShader() { return shader; }
	ID3D11Buffer* GetConstantBuffer() { return constantBuffer.Get(); }

private:
	DirectX::XMFLOAT4 diffuse{ 0.0f, 0.0f, 0.0f, 1.0f };
	DirectX::XMFLOAT4 specular{ 0.0f, 0.0f, 0.0f, 1.0f };
	DirectX::XMFLOAT4 ambient{ 0.0f, 0.0f, 0.0f, 1.0f };

	void CreateConstantBuffer(Graphics* graphics, void* data, size_t size);
	void UpdateConstantBuffer(Graphics* graphics, void* data, size_t size);

	std::unordered_map<Texture::TextureType, std::string> textureFilePaths;
	std::unordered_map<Texture::TextureType, std::shared_ptr<Texture>> textures;
	std::shared_ptr<Shader> shader = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer = nullptr;
};