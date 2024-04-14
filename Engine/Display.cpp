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

	graphics->CreateSwapChain(hWnd, width, height, swapChain.GetAddressOf());
	graphics->CreateRenderTargetView(swapChain.Get(), renderTargetView.GetAddressOf());
	graphics->CreateDepthStencilView(width, height, depthStencilView.GetAddressOf());
	graphics->CreateDepthStencilState(depthStencilState.GetAddressOf());
	graphics->SetViewport(width, height);
	return 0;
}
