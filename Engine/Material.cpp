#include "pch.h"
#include "Material.h"

#include "Graphics.h"

Material::Material()
{
}

Material::~Material()
{
}

void Material::LoadTexture(Graphics* graphics)
{
	for (auto& filePath : textureFilePaths)
	{
		auto texture = std::make_shared<Texture>();
		texture->CreateTexture(graphics, Utility::UTF8ToWideString(filePath.second));
		textures.insert({ filePath.first, texture });
	}
}

void Material::SetConstantBufferData(Graphics* graphics, void* data, size_t size)
{
	if (constantBuffer == nullptr)
	{
		CreateConstantBuffer(graphics, data, size);
	}
	else
	{
		UpdateConstantBuffer(graphics, data, size);
	}
}

void Material::CreateConstantBuffer(Graphics* graphics, void* data, size_t size)
{
	graphics->CreateConstantBuffer(data, size, constantBuffer.GetAddressOf());
}

void Material::UpdateConstantBuffer(Graphics* graphics, void* data, size_t size)
{
	graphics->UpdateConstantBuffer(data, constantBuffer.Get());
}
