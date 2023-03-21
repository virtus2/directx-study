#include "Graphics.h"

Graphics::Graphics()
{
	direct3D = 0;
	camera = 0;
	model = 0;
	light = 0;
	specularMapShader = 0;
	bumpMapShader = 0;
	multiTextureShader = 0;
	lightMapShader = 0;
	alphaMapShader = 0;
	lightShader = 0;
	textureShader = 0;
	bitmap = 0;
	// colorShader = 0;
	text = 0;
	frustum = 0;
	debugWindow = 0;
	renderTexture = 0;
	fogShader = 0;
	clipPlaneShader = 0;
}

Graphics::Graphics(const Graphics&)
{
}

Graphics::~Graphics()
{
}

bool Graphics::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	char bitmapFilename[128];
	bool result = false;
	direct3D = new D3DClass;
	XMMATRIX baseViewMatrix;

	result = direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	camera = new Camera;
	if(!camera)
	{
		return false;
	}

	// Initialize a base view matrix with the camera for 2D user interface rendering.
	camera->SetPosition(0.0f, 0.0f, -1.0f);
	camera->Render();
	camera->GetViewMatrix(baseViewMatrix);
	
	// Create and initialize the model object.
	char modelFilename[128];
	wchar_t textureFilename[128];
	wchar_t textureFilename1[128];
	wchar_t textureFilename2[128];
	wchar_t textureFilename3[128];
	strcpy_s(modelFilename, "triangle.txt");
	wcscpy_s(textureFilename, 128, L"seafloor.dds");
	wcscpy_s(textureFilename1, 128, L"stone02.dds");
	wcscpy_s(textureFilename2, 128, L"bump02.dds");
	wcscpy_s(textureFilename3, 128, L"spec02.dds");
	model = new Model;
	result = model->Initialize(direct3D->GetDevice(), modelFilename, textureFilename);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object", L"Error", MB_OK);
		return false;
	}
	
	clipPlaneShader = new ClipPlaneShader;
	result = clipPlaneShader->Initialize(direct3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the clip plane shader object", L"Error", MB_OK);
		return false;
	}

	return true;
}

void Graphics::Shutdown()
{
	if(clipPlaneShader)
	{
		clipPlaneShader->Shutdown();
		delete clipPlaneShader;
		clipPlaneShader = 0;
	}

	if(fogShader)
	{
		fogShader->Shutdown();
		delete fogShader;
		fogShader = 0;
	}

	if (renderTexture)
	{
		renderTexture->Shutdown();
		delete renderTexture;
		renderTexture = 0;
	}

	if (debugWindow)
	{
		debugWindow->Shutdown();
		delete debugWindow;
		debugWindow = 0;
	}

	if(specularMapShader)
	{
		specularMapShader->Shutdown();
		delete specularMapShader;
		specularMapShader = 0;
	}

	if(bumpMapShader)
	{
		bumpMapShader->Shutdown();
		delete bumpMapShader;
		bumpMapShader = 0;
	}

	if(alphaMapShader)
	{
		alphaMapShader->Shutdown();
		delete alphaMapShader;
		alphaMapShader = 0;
	}

	if (lightMapShader)
	{
		lightMapShader->Shutdown();
		delete lightMapShader;
		lightMapShader = 0;
	}

	if(multiTextureShader)
	{
		multiTextureShader->Shutdown();
		delete multiTextureShader;
		multiTextureShader = 0;
	}

	// Release the frustum object.
	if(frustum)
	{
		delete frustum;
		frustum = 0;
	}

	// Release the model list object.
	if(modelList)
	{
		modelList->Shutdown();
		delete modelList;
		modelList = 0;
	}

	// Release the bitmap object.
	if (bitmap)
	{
		bitmap->Shutdown();
		delete bitmap;
		bitmap = 0;
	}
	// Release the text object.
	if (text)
	{
		text->Shutdown();
		delete text;
		text = 0;
	}

	if(lightShader)
	{
		lightShader->Shutdown();
		delete lightShader;
		lightShader = 0;
	}
	/*
	if(colorShader)
	{
		colorShader->Shutdown();
		delete colorShader;
		colorShader = 0;
	}
	*/
	if(light)
	{
		delete light;
		light = 0;
	}
	if (model)
	{
		model->Shutdown();
		delete model;
		model = 0;
	}
	if (camera)
	{
		delete camera;
		camera = 0;
	}
	if(direct3D)
	{
		direct3D->Shutdown();
		delete direct3D;
		direct3D = 0;
	}
}

bool Graphics::Frame(int fps, int cpu, float rotationY, int mouseX, int mouseY)
{
	bool result = false;

	// Set the position of the camera.
	camera->SetPosition(0.0f, 0.0f, -5.0f);

	Render(mouseX, mouseY);
	return true;
}

bool Graphics::Render(int mouseX, int mouseY)
{
	XMFLOAT4 clipPlane = XMFLOAT4(0.0f, -1.0f, 0.0f, 0.0f);
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	int modelCount, renderCount;
	float positionX, positionY, positionZ;
	XMFLOAT4 color;
	bool result, renderModel;
	static float rotation = 0.0f;
	
	
	// Clear the buffers to begin the scene.
	direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	camera->Render();

	direct3D->GetWorldMatrix(worldMatrix);
	camera->GetViewMatrix(viewMatrix);
	direct3D->GetProjectionMatrix(projectionMatrix);

	model->Render(direct3D->GetDeviceContext());

	result = clipPlaneShader->Render(direct3D->GetDeviceContext(), model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, model->GetTexture(), clipPlane);
	if(!result)
	{
		return false;
	}

	// Present the rendered scene to the screen.
	direct3D->EndScene();

	return true;
}

bool Graphics::RenderToTexture()
{
	bool result;
	
	// Set the render target to be the render to texture.
	renderTexture->SetRenderTarget(direct3D->GetDeviceContext(), direct3D->GetDepthStencilView());

	// Clear the render to texture.
	renderTexture->ClearRenderTarget(direct3D->GetDeviceContext(), direct3D->GetDepthStencilView(), 0.0f, 0.0f, 1.0f, 1.0f);

	// Render the scene now and it will draw to the render to texture instead of the back buffer.
	result = RenderScene();
	if (!result)
	{
		return false;
	}

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	direct3D->SetBackBufferRenderTarget();

	return true;
}

bool Graphics::RenderScene()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;
	static float rotation = 0.0f;
	
	// Generate the view matrix based on the camera's position.
	camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	direct3D->GetWorldMatrix(worldMatrix);
	camera->GetViewMatrix(viewMatrix);
	direct3D->GetProjectionMatrix(projectionMatrix);

	// Update the rotation variable each frame.
	rotation += (float)XM_PI * 0.0025f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	worldMatrix = XMMatrixRotationY(rotation);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	model->Render(direct3D->GetDeviceContext());

	// Render the model using the light shader.
	result = lightShader->Render(direct3D->GetDeviceContext(), model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		model->GetTexture(), light->GetDirection(), light->GetDiffuseColor());
	if (!result)
	{
		return false;
	}
}


