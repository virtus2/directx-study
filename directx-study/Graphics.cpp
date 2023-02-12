#include "Graphics.h"

Graphics::Graphics()
{
	direct3D = 0;
	camera = 0;
	model = 0;
	light = 0;
	lightShader = 0;
	// colorShader = 0;
	// textureShader = 0;

}

Graphics::Graphics(const Graphics&)
{
	
}

Graphics::~Graphics()
{
	
}

bool Graphics::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	char textureFilename[128];
	bool result = false;
	direct3D = new D3DClass;

	result = direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	camera = new Camera;

	// Set the initial position of the camera.
	camera->SetPosition(0.0f, 0.0f, -10.0f);

	// Create and initialize the model object.
	strcpy_s(textureFilename, "stone01.tga");
	model = new Model;
	result = model->Initialize(direct3D->GetDevice(), direct3D->GetDeviceContext(), textureFilename);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object", L"Error", MB_OK);
		return false;
	}

	light = new Light;
	if(!light)
	{
		return false;
	}
	light->SetDiffuseColor(0.0f, 1.0f, 0.0f, 1.0f);
	light->SetDirection(0.0f, 0.0f, 1.0f);

	lightShader = new LightShader;
	result = lightShader->Initialize(direct3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object", L"Error", MB_OK);
		return false;
	}

	/*
	textureShader = new TextureShader;
	result = textureShader->Initialize(direct3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object", L"Error", MB_OK);
		return false;
	}
	*/

	/*
	// Create and initialize the color shader object.
	colorShader = new ColorShader;
	result = colorShader->Initialize(direct3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the color shader object", L"Error", MB_OK);
		return false;
	}
	
	return true;
	*/
}

void Graphics::Shutdown()
{
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

bool Graphics::Frame()
{
	bool result = false;
	static float rotation = 0.0f;

	// Update the rotation variable each frame.
	rotation += (float)XM_PI * 0.005f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	result = Render(rotation);
	if(!result)
	{
		return false;
	}
	return true;
}

bool Graphics::Render(float rotation)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;

	// Clear the buffers to begin the scene.
	direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position
	camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	camera->GetViewMatrix(viewMatrix);
	direct3D->GetWorldMatrix(worldMatrix);
	direct3D->GetProjectionMatrix(projectionMatrix);

	// Rotate the world matrix by the rotation value so that the triangle will spin.
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
	/*
	result = textureShader->Render(direct3D->GetDeviceContext(), model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, model->GetTexture());
	if(!result)
	{
		return false;
	}
	*/

	/*
	result = colorShader->Render(direct3D->GetDeviceContext(), model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
	if(!result)
	{
		return false;
	}
	*/
	// Present the rendered scene to the screen.
	direct3D->EndScene();
	return true;
}


