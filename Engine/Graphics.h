#pragma once
#include "pch.h"

#pragma comment(lib, "d3d11.lib")
#include <d3d11.h>
#include <d3dcompiler.h>
#include <unordered_map>
class Mesh;

class Graphics
{
public:
	Graphics();
	~Graphics();

	int Initialize(Display* display, HWND hWnd, int width, int height);

	void CreateRasterizerState();
	void CreateVertexShader(std::wstring& filePath);
	void CreatePixelShader(std::wstring& filePath);
	void SetRasterizerState(bool wireframe = false);


	void ClearColor(float r, float g, float b, float a);
	void Render();
	void DrawMesh(Mesh& mesh);

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
	
	std::unordered_map<std::wstring, Microsoft::WRL::ComPtr<ID3D11VertexShader>> vertexShaders; // 버텍스 쉐이더를 여러개 쓸 일이 있을까?
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader = nullptr;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader = nullptr;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout = nullptr; // 인풋 레이아웃을 다른 형태로 만들 일이 있을까?
};

