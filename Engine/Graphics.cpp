#include "pch.h"
#include "Graphics.h"
#include "Display.h"
#pragma comment(lib, "d3d11.lib")

Graphics::Graphics()
{
}

Graphics::~Graphics()
{
}

int Graphics::Initialize(Display* display, HWND hWnd, int width, int height)
{
	this->display = display;

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_9_3,
	};

	UINT numDriverTypes = ARRAYSIZE(driverTypes);
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	HRESULT result = 0;
	D3D_FEATURE_LEVEL featureLevel;
	for(UINT index = 0; index < numDriverTypes; index++)
	{
		D3D_DRIVER_TYPE driverType = driverTypes[index];
		result = D3D11CreateDevice(NULL, driverType, NULL, 0, featureLevels, numFeatureLevels, D3D11_SDK_VERSION, &d3dDevice, &featureLevel, &context);
		if(SUCCEEDED(result))
		{
			break;
		}
	}
	ASSERT_SUCCEEDED(result);

	d3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);
	dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);
	dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);

	CreateRasterizerState();

	return 0;
}

void Graphics::CreateRasterizerState()
{
	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.FrontCounterClockwise = false; // 시계방향이 앞면
	rasterizerDesc.DepthClipEnable = TRUE;

	d3dDevice->CreateRasterizerState(&rasterizerDesc, &rasterizerState);

	// 와이어프레임 렌더링용 래스터라이저 상태
	rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
	rasterizerDesc.CullMode = D3D11_CULL_NONE;
	d3dDevice->CreateRasterizerState(&rasterizerDesc, &wireframeRasterizerState);
}

void Graphics::SetRasterizerState(bool wireframe)
{
	context->RSSetState(wireframe ? wireframeRasterizerState.Get() : rasterizerState.Get());
}

void Graphics::ClearColor(float r, float g, float b, float a)
{
	float color[4] = { r, g, b, a };
	auto renderTargetView = display->GetRenderTargetView();
	auto depthStencilView = display->GetDepthStencilView();
	context->ClearRenderTargetView(renderTargetView, color);
	context->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	context->OMSetRenderTargets(1, &renderTargetView, depthStencilView);
}

void Graphics::DrawMesh(Mesh& mesh)
{
}

void Graphics::CheckMultisampleQualityLevels(UINT sampleCount, UINT& numQualityLevels)
{
	d3dDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, sampleCount, &numQualityLevels);
}