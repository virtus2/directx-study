#include "pch.h"
#include <assimp/scene.h>
#include "DirectXTex.h"
#include "dxgi1_6.h"

#include "Graphics.h"
#include "Game.h"
#include "Display.h"
#include "Model.h"
#include "Mesh.h"
#include "Camera.h"
#include "Entity.h"
#include "Light.h"

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

void Graphics::CreateSwapChain(HWND hWnd, int width, int height, IDXGISwapChain** outSwapChain)
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

	Microsoft::WRL::ComPtr<IDXGISwapChain> dxgiSwapChain;
	HRESULT result = dxgiFactory->CreateSwapChain(d3dDevice.Get(), &swapChainDesc, outSwapChain);
	if (FAILED(result))
	{
		MessageBox(nullptr, L"Failed to create swap chain", L"Error", MB_OK);
	}
}

void Graphics::CreateRenderTargetView(IDXGISwapChain* swapChain, ID3D11RenderTargetView** outRenderTargetView)
{
	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
	HRESULT result = swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
	if (FAILED(result))
	{
		MessageBox(nullptr, L"Failed to get back buffer", L"Error", MB_OK);
		return;
	}

	result = d3dDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, outRenderTargetView);
	if (FAILED(result))
	{
		MessageBox(nullptr, L"Failed to create render target view", L"Error", MB_OK);
		return;
	}
	backBuffer->Release();
}

void Graphics::CreateDepthStencilView(int width, int height, ID3D11DepthStencilView** outDepthStencilView)
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
	HRESULT result = d3dDevice->CreateTexture2D(&depthStencilBufferDesc, 0, depthStencilBuffer.GetAddressOf());
	if (FAILED(result))
	{
		MessageBox(nullptr, L"Failed to create depth stencil buffer", L"Error", MB_OK);
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	depthStencilViewDesc.Format = depthStencilBufferDesc.Format;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;
	result = d3dDevice->CreateDepthStencilView(depthStencilBuffer.Get(), &depthStencilViewDesc, outDepthStencilView);
	if (FAILED(result))
	{
		MessageBox(nullptr, L"Failed to create depth stencil view", L"Error", MB_OK);
	}
}

void Graphics::CreateDepthStencilState(ID3D11DepthStencilState** outDepthStencilState)
{
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

	HRESULT result = d3dDevice->CreateDepthStencilState(&depthStencilDesc, outDepthStencilState);
	if (FAILED(result))
	{
		MessageBox(nullptr, L"Failed to create depth stencil state", L"Error", MB_OK);
	}
}

void Graphics::SetViewport(int width, int height)
{
	// 래스터라이저에 뷰포트를 설정합니다.
	D3D11_VIEWPORT viewport;
	viewport.Width = static_cast<float>(width);
	viewport.Height = static_cast<float>(height);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	context->RSSetViewports(1, &viewport);
}

void Graphics::CreateVertexBuffer(const std::vector<Vertex>& vertices, ID3D11Buffer** outVertexBuffer)
{
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.ByteWidth = sizeof(Vertex) * vertices.size();
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData = {};
	vertexBufferData.pSysMem = vertices.data();
	vertexBufferData.SysMemPitch = 0;
	vertexBufferData.SysMemSlicePitch = 0;

	const HRESULT result = d3dDevice->CreateBuffer(&bufferDesc, &vertexBufferData, outVertexBuffer);
	if (FAILED(result))
	{
		Utility::Print("Failed to create vertex buffer");
	}
}

void Graphics::CreateIndexBuffer(const std::vector<uint32_t>& indices, ID3D11Buffer** outIndexBuffer)
{
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.ByteWidth = sizeof(uint32_t) * indices.size();
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA indexBufferData = {};
	indexBufferData.pSysMem = indices.data();
	indexBufferData.SysMemPitch = 0;
	indexBufferData.SysMemSlicePitch = 0;

	const HRESULT result = d3dDevice->CreateBuffer(&bufferDesc, &indexBufferData, outIndexBuffer);
	if (FAILED(result))
	{
		Utility::Print("Failed to create index buffer");
	}
}

void Graphics::CreateVertexShader(const std::wstring& filePath, ID3D11VertexShader** outVertexShader, ID3D11InputLayout** outInputLayout)
{
	Microsoft::WRL::ComPtr<ID3DBlob> vertexShaderBlob;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;

	UINT compileFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	HRESULT result = D3DCompileFromFile(
		filePath.c_str(), // 파일 경로(파일명)
		nullptr, // 컴파일 시 사용할 매크로 정의
		nullptr, // 컴파일 시 사용할 include 정의
		"main", // 쉐이더에서 사용할 진입점 함수의 이름
		"vs_5_0", // 컴파일할 쉐이더의 대상 프로파일
		compileFlags, // 컴파일 옵션
		0, // 추가 컴파일 옵션
		&vertexShaderBlob, // 컴파일된 쉐이더 코드
		&errorBlob // 컴파일 에러 메시지
	);

	if (FAILED(result))
	{
		if ((result & D3D11_ERROR_FILE_NOT_FOUND) != 0)
		{
			MessageBox(nullptr, L"File not found", L"Error", MB_OK);
		}
		else
		{
			if (errorBlob)
			{
				MessageBox(nullptr, (LPCWSTR)errorBlob->GetBufferPointer(), L"Error", MB_OK);
			}
			else
			{
				MessageBox(nullptr, L"Unknown error", L"Error", MB_OK);
			}
		}
	}

	result = d3dDevice->CreateVertexShader(vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), nullptr, outVertexShader);
	if (FAILED(result))
	{
		MessageBox(nullptr, L"Failed to create vertex shader", L"Error", MB_OK);
	}

	D3D11_INPUT_ELEMENT_DESC basicInputElements[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
		 D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3,
		 D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 3 + 4 * 3,
		 D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	result = d3dDevice->CreateInputLayout(
		basicInputElements, // 입력 레이아웃 정의
		ARRAYSIZE(basicInputElements), // 입력 레이아웃 정의의 개수
		vertexShaderBlob->GetBufferPointer(), // 컴파일된 쉐이더 코드, 유효성 검사용
		vertexShaderBlob->GetBufferSize(), // 컴파일된 쉐이더 코드의 크기
		outInputLayout // 입력 레이아웃
	);
	if (FAILED(result))
	{
		MessageBox(nullptr, L"Failed to create input layout", L"Error", MB_OK);
	}
}

void Graphics::CreatePixelShader(const std::wstring& filePath, ID3D11PixelShader** outPixelShader)
{
	Microsoft::WRL::ComPtr<ID3DBlob> pixelShaderBlob;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;

	UINT compileFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	HRESULT result = D3DCompileFromFile(
		filePath.c_str(), // 파일 경로(파일명)
		nullptr, // 컴파일 시 사용할 매크로 정의
		nullptr, // 컴파일 시 사용할 include 정의
		"main", // 쉐이더에서 사용할 진입점 함수의 이름
		"ps_5_0", // 컴파일할 쉐이더의 대상 프로파일
		compileFlags, // 컴파일 옵션
		0, // 추가 컴파일 옵션
		&pixelShaderBlob, // 컴파일된 쉐이더 코드
		&errorBlob // 컴파일 에러 메시지
	);
	if (FAILED(result))
	{
		if ((result & D3D11_ERROR_FILE_NOT_FOUND) != 0)
		{
			MessageBox(nullptr, L"File not found", L"Error", MB_OK);
		}
		else
		{
			if (errorBlob)
			{
				MessageBox(nullptr, (LPCWSTR)errorBlob->GetBufferPointer(), L"Error", MB_OK);
			}
			else
			{
				MessageBox(nullptr, L"Unknown error", L"Error", MB_OK);
			}
		}
	}
	result = d3dDevice->CreatePixelShader(pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize(), nullptr, outPixelShader);
	if (FAILED(result))
	{
		MessageBox(nullptr, L"Failed to create pixel shader", L"Error", MB_OK);
	}
}

void Graphics::CreateConstantBuffer(void* data, size_t size, ID3D11Buffer** outBuffer)
{
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = size;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA bufferData = {};
	bufferData.pSysMem = data;
	bufferData.SysMemPitch = 0;
	bufferData.SysMemSlicePitch = 0;

	HRESULT result = d3dDevice->CreateBuffer(&bufferDesc, &bufferData, outBuffer);
	if (FAILED(result))
	{
		MessageBox(nullptr, L"Failed to create constant buffer", L"Error", MB_OK);
	}
}

void Graphics::CreateTexture(const std::wstring& filePath, ID3D11Texture2D** outTexture, ID3D11ShaderResourceView** outShaderResourceView, ID3D11SamplerState** outSamplerState)
{
	// TODO: 파일 형식 핸들링
	DirectX::ScratchImage image;
	HRESULT result = DirectX::LoadFromWICFile(filePath.c_str(), DirectX::WIC_FLAGS_NONE, nullptr, image);
	if (FAILED(result))
	{
		// TODO: 에러핸들링을 좀 더 깔쌈하게 하는 법은 없을까
		// bool 반환하는 방법도 있을듯. 한번 고치기 시작하면 다른데도 다 통일하자...
		MessageBox(nullptr, L"Failed to load texture", L"Error", MB_OK);
		return;
	}
	
	DirectX::TexMetadata metadata = image.GetMetadata();
	DirectX::ScratchImage mipChain;
	result = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_DEFAULT, 0, mipChain);
	if (FAILED(result))
	{
		MessageBox(nullptr, L"Failed to generate mipmaps", L"Error", MB_OK);
		return;
	}

	result = DirectX::CreateTexture(d3dDevice.Get(), mipChain.GetImages(), mipChain.GetImageCount(), mipChain.GetMetadata(), (ID3D11Resource**)outTexture);
	if (FAILED(result))
	{
		MessageBox(nullptr, L"Failed to create texture", L"Error", MB_OK);
		return;	
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};
	shaderResourceViewDesc.Format = metadata.format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = (UINT)mipChain.GetMetadata().mipLevels;
	result = d3dDevice->CreateShaderResourceView(*outTexture, &shaderResourceViewDesc, outShaderResourceView);
	if (FAILED(result))
	{
		MessageBox(nullptr, L"Failed to create shader resource view", L"Error", MB_OK);
		return;
	}

	// TODO: 샘플러 스테이트를 다른곳에서 생성하고 다른곳에서 참조
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MinLOD = 0.0f;
	samplerDesc.MaxLOD = (FLOAT)mipChain.GetMetadata().mipLevels;
	result = d3dDevice->CreateSamplerState(&samplerDesc, outSamplerState);
	if (FAILED(result))
	{
		MessageBox(nullptr, L"Failed to create sampler state", L"Error", MB_OK);
		return;
	}
}

void Graphics::CreateTexture(uint8_t* data, size_t width, size_t height, ID3D11Texture2D** outTexture, ID3D11ShaderResourceView** outShaderResourceView, ID3D11SamplerState** outSamplerState)
{
	DirectX::Image rawImage;
	rawImage.pixels = data;
	rawImage.width = width;
	rawImage.height = height;
	rawImage.format = DXGI_FORMAT_R8G8B8A8_UNORM; // TODO: UINT는 0~255지만 다른 형식도 존재할 수 있음

	size_t rowPitch, slicePitch;
	ComputePitch(rawImage.format, width, height, rowPitch, slicePitch);
	rawImage.rowPitch = rowPitch;
	rawImage.slicePitch = slicePitch;

	DirectX::ScratchImage image;
	image.InitializeFromImage(rawImage);

	DirectX::TexMetadata metadata = image.GetMetadata();
	DirectX::ScratchImage mipChain;
	HRESULT result = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_DEFAULT, 0, mipChain);
	if (FAILED(result))
	{
		MessageBox(nullptr, L"Failed to generate mipmaps", L"Error", MB_OK);
		return;
	}

	result = DirectX::CreateTexture(d3dDevice.Get(), mipChain.GetImages(), mipChain.GetImageCount(), mipChain.GetMetadata(), (ID3D11Resource**)outTexture);
	if (FAILED(result))
	{
		MessageBox(nullptr, L"Failed to create texture", L"Error", MB_OK);
		return;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};
	shaderResourceViewDesc.Format = metadata.format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = (UINT)mipChain.GetMetadata().mipLevels;
	result = d3dDevice->CreateShaderResourceView(*outTexture, &shaderResourceViewDesc, outShaderResourceView);
	if (FAILED(result))
	{
		MessageBox(nullptr, L"Failed to create shader resource view", L"Error", MB_OK);
		return;
	}

	// TODO: 샘플러 스테이트를 다른곳에서 생성하고 다른곳에서 참조
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MinLOD = 0.0f;
	samplerDesc.MaxLOD = (FLOAT)mipChain.GetMetadata().mipLevels;
	result = d3dDevice->CreateSamplerState(&samplerDesc, outSamplerState);
	if (FAILED(result))
	{
		MessageBox(nullptr, L"Failed to create sampler state", L"Error", MB_OK);
		return;
	}
}

void Graphics::UseMaterial(Material* material)
{
	auto shader = material->GetShader();
	if (shader)
	{
		context->IASetInputLayout(shader->GetInputLayout());
		context->VSSetShader(shader->GetVertexShader(), nullptr, 0);
		context->PSSetShader(shader->GetPixelShader(), nullptr, 0);
	}

	auto diffuse = material->GetTexture(Texture::TextureType::Diffuse);
	if (diffuse)
	{
		auto texture = diffuse->GetTexture();
		auto shaderResourceView = diffuse->GetShaderResourceView();
		auto samplerState = diffuse->GetSamplerState();

		context->PSSetSamplers(0, 1, &samplerState);
		context->PSSetShaderResources(0, 1, &shaderResourceView);
	}
	/*
	size_t count = static_cast<size_t>(Texture::TextureType::Count);
	for(size_t i = 0; i < count; i++)
	{
		auto textureType = static_cast<Texture::TextureType>(i);
		auto texture = material->GetTexture(textureType);
		if (texture)
		{
			auto shaderResourceView = texture->GetShaderResourceView();
			auto samplerState = texture->GetSamplerState();

			// TODO: 텍스처마다 슬롯 정해주기. 쉐이더가 달라질 경우도 고려해야함.
			// TODO: 우버 쉐이더에 대해서 알아보기.
			context->PSSetSamplers(0, 1, &samplerState);
			context->PSSetShaderResources(0, 1, &shaderResourceView);
		}
	}
	*/
}

void Graphics::UpdateConstantBuffer(void* data, size_t size, ID3D11Buffer* buffer)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	context->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	memcpy(mappedResource.pData, data, size);
	context->Unmap(buffer, 0);
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

void Graphics::Render(Engine::Game* game)
{
	// 파이프라인 단계는 다음과 같습니다.
	// Input Assembler 
	// Vertex Shader 
	// Hull Shader
	// Tessellator
	// Domain Shader
	// Geometry Shader
	// Rasterizer
	// Pixel Shader
	// Output Merger

	ClearColor(0.0f, 0.0f, 0.75f, 1.0f);
	SetRasterizerState();

	// 카메라
	auto mainCamera = game->GetMainCamera();
	if (mainCamera)
	{
		// 카메라 관련된 상수 버퍼를 갱신합니다.
		auto cameraDataBuffer = mainCamera->GetCameraDataBuffer();
		auto cameraData = mainCamera->GetCameraData();
		// TODO: 굳이 분기 안타고 미리 만들어놓는게 나을지도... 메인카메라 변경될경우 버퍼 만들어주고 해제하면 될거같음
		if (cameraDataBuffer)
		{
			UpdateConstantBuffer(&cameraData, sizeof(cameraData), cameraDataBuffer);
		}
		else
		{
			CreateConstantBuffer(&cameraData, sizeof(cameraData), &cameraDataBuffer);
		}
		context->VSSetConstantBuffers(0, 1, &cameraDataBuffer);
	}
	else
	{
		// TODO: 메인카메라 없을때 에러 핸들링
	}

	// 광원
	auto lights = game->GetLights();
	for (const auto& light : lights)
	{
		auto cameraPosition = mainCamera->GetPosition();
		XMFLOAT3 cameraPositionFloat3 = { cameraPosition.x, cameraPosition.y, cameraPosition.z };
		light->SetCameraPosition(cameraPositionFloat3);

		light->SetLightDirection({ 0.0f, 0.0f, 0.0f});
		light->SetLightPosition({5.0f, 5.0f, -10.0f });
		light->Update();

		auto lightConstantBuffer = light->GetLightConstantBuffer();
		auto lightConstantBufferData = light->GetLightConstantBufferData();
		auto lightConstantBufferSize = light->GetLightConstantBufferSize();
		if (lightConstantBuffer)
		{
			UpdateConstantBuffer(lightConstantBufferData, lightConstantBufferSize, lightConstantBuffer);
		}
		else
		{
			CreateConstantBuffer(lightConstantBufferData, lightConstantBufferSize, &lightConstantBuffer);
		}
		context->PSSetConstantBuffers(0, 1, &lightConstantBuffer);
	}
	
	// 엔티티
	auto entities = game->GetEntities();
	for (const auto& entity : entities)
	{
		// 월드 행렬을 갱신합니다.
		auto position = entity->GetPosition();
		auto rotation = entity->GetRotation();
		auto scale = entity->GetScale();

		auto world = XMMatrixScaling(scale.x, scale.y, scale.z)
			* XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z)
			* XMMatrixTranslation(position.x, position.y, position.z);

		auto objectVertexConstantBuffer = entity->GetObjectVertexConstantBuffer();
		auto objectVertexConstantBufferData = entity->GetObjectVertexConstantBufferData();
		objectVertexConstantBufferData->world = XMMatrixTranspose(world);
		auto objectVertexConstantBufferSize = entity->GetObjectVertexConstantBufferSize();
		// TODO: 굳이 분기 안타고 미리 만들어놓는게 나을지도... 
		if (objectVertexConstantBuffer)
		{
			UpdateConstantBuffer(objectVertexConstantBufferData, objectVertexConstantBufferSize, objectVertexConstantBuffer);
		}
		else
		{
			CreateConstantBuffer(objectVertexConstantBufferData, objectVertexConstantBufferSize, &objectVertexConstantBuffer);
		}
		context->VSSetConstantBuffers(1, 1, &objectVertexConstantBuffer);
		// context->PSSetConstantBuffers(1, 1, &ObjectVertexConstantBuffer);

		auto model = entity->GetModel();
		DrawModel(model.get());
	}

	/*
	* 
	for each shader:
    // Set the device context to use this shader
    pContext->VSSetShader(shader.pVertexShader);
    pContext->PSSetShader(shader.pPixelShader);

    for each material that uses this shader:
        // Set the device context to use any constant buffers, textures, samplers,
        // etc. needed for this material
        pContext->VSSetConstantBuffers(...);
        pContext->PSSetConstantBuffers(...);
        pContext->PSSetShaderResources(...);
        pContext->PSSetSamplers(...);

        for each mesh that uses this material:
            // Set any constant buffers containing parameters specific to the mesh
            // (e.g. world matrix)
            pContext->VSSetConstantBuffers(...);

            // Set the context to use the vertex & index buffers for this mesh
            pContext->IASetInputLayout(mesh.pInputLayout);
            pContext->IASetVertexBuffers(...);
            pContext->IASetIndexBuffer(...);
            pContext->IASetPrimitiveTopology(...)

            // Draw it
            pContext->DrawIndexed(...)

	*/

}

void Graphics::DrawMesh(Mesh* mesh)
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	auto vertexBuffer = mesh->GetVertexBuffer();
	auto indexBuffer = mesh->GetIndexBuffer();
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	context->DrawIndexed(mesh->GetIndexCount(), 0, 0);
}

void Graphics::DrawModel(Model* model)
{
	auto meshes = model->GetMeshes();
	size_t count = meshes.size();
	for (size_t i = 0; i < count; i++)
	{
		auto mesh = meshes[i];
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		auto material = mesh->GetMaterial();
		UseMaterial(material.get());

		auto vertexBuffer = mesh->GetVertexBuffer();
		auto indexBuffer = mesh->GetIndexBuffer();
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
		context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
		context->DrawIndexed(mesh->GetIndexCount(), 0, 0);
	}
}

void Graphics::CheckMultisampleQualityLevels(UINT sampleCount, UINT& numQualityLevels)
{
	d3dDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, sampleCount, &numQualityLevels);
}
