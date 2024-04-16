#include "pch.h"

#include "Texture.h"
#include "Graphics.h"
#include <assimp/scene.h>

Texture::Texture()
{

}

Texture::~Texture()
{

}

void Texture::CreateTexture(Graphics* graphics, const std::wstring& filePath)
{
	graphics->CreateTexture(filePath, texture.GetAddressOf(), shaderResourceView.GetAddressOf(), samplerState.GetAddressOf());
}

void Texture::CreateTexture(Graphics* graphics, uint8_t* data, size_t width, size_t height)
{
	graphics->CreateTexture(data, width, height, this->texture.GetAddressOf(), shaderResourceView.GetAddressOf(), samplerState.GetAddressOf());
}
