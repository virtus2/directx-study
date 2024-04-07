#pragma once
#include "pch.h"

#include <d3d11.h>

class Shader 
{
public:
	Shader();
	~Shader();

	void CreateShader(Graphics* graphics, const std::wstring& vertexShaderFilePath, const std::wstring& pixelShaderFilePath);

	ID3D11VertexShader* GetVertexShader() { return vertexShader.Get(); }
	ID3D11PixelShader* GetPixelShader() { return pixelShader.Get(); }
	ID3D11InputLayout* GetInputLayout() { return inputLayout.Get(); }

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader = nullptr;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader = nullptr;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout = nullptr; // 인풋 레이아웃을 다른 형태로 만들 일이 있을까?
};