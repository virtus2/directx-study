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

	void SetConstantBufferData(Graphics* graphics, void* data, size_t size);
	void SetShader(std::shared_ptr<Shader> shader) { this->shader = shader; }
	std::shared_ptr<Shader> GetShader() { return shader; }
	ID3D11Buffer* GetConstantBuffer() { return constantBuffer.Get(); }

private:
	void CreateConstantBuffer(Graphics* graphics, void* data, size_t size);
	void UpdateConstantBuffer(Graphics* graphics, void* data, size_t size);

	std::unordered_map<Texture::TextureType, std::string> textureFilePaths;
	std::unordered_map<Texture::TextureType, std::shared_ptr<Texture>> textures;
	std::shared_ptr<Shader> shader = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer = nullptr;
};