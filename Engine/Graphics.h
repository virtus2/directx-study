﻿#pragma once
#pragma comment(lib, "d3d11.lib")
#include <d3d11.h>
#include <d3dcompiler.h>
#include <unordered_map>

#include "Matrix.h"

namespace Engine
{
	class Game;
}
class Mesh;
class Model;
class Shader;
class Vertex;
class Material;
class Camera;
struct aiTexture;

class Graphics
{
public:
	Graphics();
	~Graphics();

	int Initialize(Display* display, HWND hWnd, int width, int height);

	void CreateRasterizerState();

	void SetRasterizerState(bool wireframe = false);

public:
	// Display
	void CreateSwapChain(HWND hWnd, int width, int height, IDXGISwapChain** outSwapChain);
	void CreateRenderTargetView(IDXGISwapChain* swapChain, ID3D11RenderTargetView** outRenderTargetView);
	void CreateDepthStencilView(int width, int height, ID3D11DepthStencilView** outDepthStencilView);
	void CreateDepthStencilState(ID3D11DepthStencilState** outDepthStencilState);

	void SetViewport(int width, int height);

	void CreateVertexBuffer(const std::vector<Vertex>& vertices, ID3D11Buffer** outVertexBuffer);
	void CreateIndexBuffer(const std::vector<uint32_t>& indices, ID3D11Buffer** outIndexBuffer);
	void CreateVertexShader(const std::wstring& filePath, ID3D11VertexShader** outVertexShader, ID3D11InputLayout** outInputLayout);
	void CreatePixelShader(const std::wstring& filePath, ID3D11PixelShader** outPixelShader);
	void CreateConstantBuffer(void* data, size_t size, ID3D11Buffer** outBuffer);
	void CreateTexture(const std::wstring& filePath, ID3D11Texture2D** outTexture, ID3D11ShaderResourceView** outShaderResourceView, ID3D11SamplerState** outSamplerState);
	void CreateTexture(uint8_t* data, size_t width, size_t height, ID3D11Texture2D** outTexture, ID3D11ShaderResourceView** outShaderResourceView, ID3D11SamplerState** outSamplerState);

	void UseMaterial(Material* material);
	void UpdateConstantBuffer(void* data, size_t size, ID3D11Buffer* buffer);

	void ClearColor(float r, float g, float b, float a);
	void Render(Engine::Game* game);
	void DrawMesh(Mesh* mesh);
	void DrawModel(Model* model);

	void CheckMultisampleQualityLevels(UINT sampleCount, UINT& numQualityLevels);

	ID3D11Device* GetDevice() { return d3dDevice.Get(); }
	IDXGIDevice* GetDXGIDevice() { return dxgiDevice.Get(); }
	IDXGIAdapter* GetDXGIAdapter() { return dxgiAdapter.Get(); }
	IDXGIFactory* GetDXGIFactory() { return dxgiFactory.Get(); }
	ID3D11DeviceContext* GetContext() { return context.Get(); }

private:
	HWND hWnd = nullptr;
	Display* display = nullptr;

	Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice = nullptr;
	Microsoft::WRL::ComPtr<IDXGIDevice> dxgiDevice = nullptr;
	Microsoft::WRL::ComPtr<IDXGIAdapter> dxgiAdapter = nullptr;
	Microsoft::WRL::ComPtr<IDXGIFactory> dxgiFactory = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context = nullptr;

	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> wireframeRasterizerState = nullptr;
};

