#pragma once
#include "pch.h"

#include <d3d11.h>

class Graphics
{
public:
	Graphics();
	~Graphics();

	int Initialize(Display* display, HWND hWnd, int width, int height);
	void ClearColor(float r, float g, float b, float a);

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
};

