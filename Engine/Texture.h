#pragma once
#include <d3d11.h>
#include <string>

class Graphics;
class aiTexture;

class Texture
{
public:
	enum class TextureType
	{
		None,

		Diffuse,
		Specular,
		Ambient,
		Emissive,
		Height,
		Normals,
		Shininess,
		Opacity,
		Displacement,
		Lightmap,
		Reflection,
		BaseColor,
		NormalCamera,
		EmissionColor,
		Metalness,
		DiffuseRoughness,
		AmbientOcclusion,

		Count
	};
	Texture();
	~Texture();

	void CreateTexture(Graphics* graphics, const std::wstring& filePath);
	void CreateTexture(Graphics* graphics, uint8_t* data, size_t width, size_t height);
	
	ID3D11ShaderResourceView* GetShaderResourceView() { return shaderResourceView.Get(); }
	ID3D11SamplerState* GetSamplerState() { return samplerState.Get(); }
	ID3D11Texture2D* GetTexture() { return texture.Get(); }


private:
	std::string filePath;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderResourceView = nullptr;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture = nullptr;

};