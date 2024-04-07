#pragma once
#include "pch.h"

#pragma comment(lib, "d3d11.lib")
#include <d3d11.h>
#include <d3dcompiler.h>
#include <unordered_map>

class Mesh;
class Model;
class Shader;
class Vertex;

class Graphics
{
public:
	Graphics();
	~Graphics();

	int Initialize(Display* display, HWND hWnd, int width, int height);

	void CreateRasterizerState();
	void CreateVertexBuffer(const std::vector<Vertex>& vertices, ID3D11Buffer** outVertexBuffer);
	void CreateIndexBuffer(const std::vector<uint32_t>& indices, ID3D11Buffer** outIndexBuffer);
	void CreateVertexShader(const std::wstring& filePath, ID3D11VertexShader** outVertexShader, ID3D11InputLayout** outInputLayout);
	void CreatePixelShader(const std::wstring& filePath, ID3D11PixelShader** outPixelShader);

	void SetRasterizerState(bool wireframe = false);
	void SetShader(Shader* shader);

	void ClearColor(float r, float g, float b, float a);
	void Render();
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

