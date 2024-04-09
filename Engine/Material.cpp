#include "pch.h"
#include "Material.h"

#include "Graphics.h"

Material::Material()
{
}

Material::~Material()
{
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
