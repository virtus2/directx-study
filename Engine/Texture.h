#pragma once
#include <d3d11.h>
#include <string>

class Graphics;

class Texture
{
public:
	enum class TextureType
	{
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
	ID3D11ShaderResourceView* GetShaderResourceView() { return shaderResourceView.Get(); }

private:
	std::string filePath;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderResourceView = nullptr;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture = nullptr;

};