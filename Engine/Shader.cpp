#include "pch.h"
#include "Shader.h"

#include "Graphics.h"


Shader::Shader()
{
}

Shader::~Shader()
{
}

void Shader::CreateShader(Graphics* graphics, const std::wstring& vertexShaderFilePath, const std::wstring& pixelShaderFilePath)
{
	graphics->CreateVertexShader(vertexShaderFilePath, vertexShader.GetAddressOf(), inputLayout.GetAddressOf());
	graphics->CreatePixelShader(pixelShaderFilePath, pixelShader.GetAddressOf());
}
