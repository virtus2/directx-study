#include "Graphics.h"

Graphics::Graphics()
{
	direct3D = 0;
}

Graphics::Graphics(const Graphics&)
{
	
}

Graphics::~Graphics()
{
	
}

bool Graphics::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result = false;
	direct3D = new D3DClass;

	result = direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}
	return true;
}

void Graphics::Shutdown()
{
	if(direct3D)
	{
		direct3D->Shutdown();
		delete direct3D;
		direct3D = 0;
	}
}

bool Graphics::Frame()
{
	bool result = false;
	result = Render();
	if(!result)
	{
		return false;
	}
	return true;
}

bool Graphics::Render()
{
	// Clear the buffers to begin the scene.
	direct3D->BeginScene(0.5f, 0.5f, 0.5f, 1.0f);
	// Present the rendered scene to the screen.
	direct3D->EndScene();
	return true;
}


