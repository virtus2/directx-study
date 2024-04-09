#include "pch.h"
#include "Display.h"
#include "Game.h"
#include "d3d11.h"
#include "dxgi1_6.h"

Display::Display()
{
}

Display::~Display()
{
}

int Display::Initialize(Graphics* graphics, HWND hWnd, int width, int height)
{
	this->graphics = graphics;

	CreateSwapChain(hWnd, width, height);
	CreateRenderTargetView();
	SetViewport(width, height);
	CreateDepthStencilView(width, height);
	CreateDepthStencilState();
	return 0;
}

void Display::CreateSwapChain(HWND hWnd, int width, int height)
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferCount = 2; // 더블 버퍼
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hWnd;
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	// TODO: 멀티샘플링 지원
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	auto device = graphics->GetDevice();
	auto context = graphics->GetContext();
	auto dxgiFactory = graphics->GetDXGIFactory();

	HRESULT result;
	Microsoft::WRL::ComPtr<IDXGISwapChain> dxgiSwapChain;
	result = dxgiFactory->CreateSwapChain(device, &swapChainDesc, &dxgiSwapChain);
	ASSERT_SUCCEEDED(result);
	dxgiSwapChain.As(&swapChain);
}

void Display::CreateRenderTargetView()
{
	HRESULT result;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
	result = swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
	ASSERT_SUCCEEDED(result);

	auto device = graphics->GetDevice();
	result = device->CreateRenderTargetView(backBuffer.Get(), nullptr, &renderTargetView);	
	backBuffer->Release();
	ASSERT_SUCCEEDED(result);
}

void Display::SetViewport(int width, int height)
{
	// 래스터라이저에 뷰포트를 설정합니다.
	D3D11_VIEWPORT viewport;
	viewport.Width = static_cast<float>(width);
	viewport.Height = static_cast<float>(height);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	auto context = graphics->GetContext();
	context->RSSetViewports(1, &viewport);
}

void Display::CreateDepthStencilView(int width, int height)
{
	D3D11_TEXTURE2D_DESC depthStencilBufferDesc;
	depthStencilBufferDesc.Width = width;
	depthStencilBufferDesc.Height = height;
	depthStencilBufferDesc.MipLevels = 1;
	depthStencilBufferDesc.ArraySize = 1;
	depthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	// TODO: 멀티샘플링 지원
	depthStencilBufferDesc.SampleDesc.Count = 1;
	depthStencilBufferDesc.SampleDesc.Quality = 0;
	depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilBufferDesc.CPUAccessFlags = 0;
	depthStencilBufferDesc.MiscFlags = 0;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilBuffer;
	auto device = graphics->GetDevice();

	HRESULT result;
	result = device->CreateTexture2D(&depthStencilBufferDesc, 0, depthStencilBuffer.GetAddressOf());
	// TODO: ASSERT 쓸지 그냥 if문 쓸지 결정
	ASSERT_SUCCEEDED(result);

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	depthStencilViewDesc.Format = depthStencilBufferDesc.Format;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;
	result = device->CreateDepthStencilView(depthStencilBuffer.Get(), &depthStencilViewDesc, depthStencilView.GetAddressOf());
	ASSERT_SUCCEEDED(result);
}

void Display::CreateDepthStencilState()
{
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

	HRESULT result;
	auto device = graphics->GetDevice();
	result = device->CreateDepthStencilState(&depthStencilDesc, depthStencilState.GetAddressOf());
	ASSERT_SUCCEEDED(result);
}
