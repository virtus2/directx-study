#include "pch.h"

#include "Texture.h"
#include "Graphics.h"

void Texture::CreateTexture(Graphics* graphics, const std::wstring& filePath)
{
	graphics->CreateTexture(filePath, texture.GetAddressOf(), shaderResourceView.GetAddressOf(), samplerState.GetAddressOf());
}