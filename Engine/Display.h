#pragma once
#include <d3d11.h>

class Display
{
public:
	Display();
	~Display();

	int Initialize(Graphics* graphics, HWND hWnd, int width, int height);

	IDXGISwapChain* GetSwapChain() { return swapChain.Get(); }
	ID3D11RenderTargetView* GetRenderTargetView() { return renderTargetView.Get(); }
	ID3D11DepthStencilView* GetDepthStencilView() { return depthStencilView.Get(); }

private:
	HWND hWnd = nullptr;
	Graphics* graphics = nullptr;

	Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState = nullptr;
};

