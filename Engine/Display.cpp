#include "pch.h"
#include "Display.h"
#include "Game.h"

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
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	auto device = graphics->GetDevice();
	auto context = graphics->GetContext();

	HRESULT result = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&swapChain,
		&device,
		nullptr,
		&context
	);

	ASSERT_SUCCEEDED(result);
}

void Display::CreateRenderTargetView()
{
	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
	swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
	ASSERT(backBuffer != nullptr, "backBuffer is nullptr");
	auto device = graphics->GetDevice();
	device->CreateRenderTargetView(backBuffer.Get(), nullptr, &renderTargetView);	
}

void Display::SetViewport(int width, int height)
{
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

void Display::CreateRasterizerState()
{
	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_BACK; 
	rasterizerDesc.FrontCounterClockwise = false; // 시계방향이 앞면
	rasterizerDesc.DepthClipEnable = TRUE;

	// 일반 래스터라이저 상태
	auto device = graphics->GetDevice();
	device->CreateRasterizerState(&rasterizerDesc, &rasterizerState);

	// 와이어프레임 렌더링용 래스터라이저 상태
	rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
	rasterizerDesc.CullMode = D3D11_CULL_NONE;
	device->CreateRasterizerState(&rasterizerDesc, &wireframeRasterizerState);
}

void Display::CreateDepthStencilView(int width, int height)
{
	D3D11_TEXTURE2D_DESC depthStencilBufferDesc;
	depthStencilBufferDesc.Width = width;
	depthStencilBufferDesc.Height = height;
	depthStencilBufferDesc.MipLevels = 1;
	depthStencilBufferDesc.ArraySize = 1;
	depthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilBufferDesc.SampleDesc.Count = 1; // how many multisamples
	depthStencilBufferDesc.SampleDesc.Quality = 0;
	depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilBufferDesc.CPUAccessFlags = 0;
	depthStencilBufferDesc.MiscFlags = 0;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilBuffer;
	auto device = graphics->GetDevice();

	HRESULT result;
	result = device->CreateTexture2D(&depthStencilBufferDesc, 0, depthStencilBuffer.GetAddressOf());
	ASSERT_SUCCEEDED(result);
	result = device->CreateDepthStencilView(depthStencilBuffer.Get(), 0, depthStencilView.GetAddressOf());
	ASSERT_SUCCEEDED(result);

	// Create depth stencil state
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	depthStencilDesc.DepthEnable = true; // false
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;
	result = device->CreateDepthStencilState(&depthStencilDesc, depthStencilState.GetAddressOf());
	ASSERT_SUCCEEDED(result);
}
