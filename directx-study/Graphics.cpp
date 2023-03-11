#include "Graphics.h"

Graphics::Graphics()
{
	direct3D = 0;
	camera = 0;
	model = 0;
	light = 0;
	lightShader = 0;
	bitmap = 0;
	// colorShader = 0;
	textureShader = 0;
	text = 0;

}

Graphics::Graphics(const Graphics&)
{
	
}

Graphics::~Graphics()
{
	
}

bool Graphics::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	char modelFilename[128];
	char textureFilename[128];
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

	// Set the initial position of the camera.
	camera->SetPosition(0.0f, 0.0f, -10.0f);

	/*
	// Create and initialize the model object.
	strcpy_s(modelFilename, "cube.txt");
	strcpy_s(textureFilename, "stone01.tga");
	model = new Model;
	result = model->Initialize(direct3D->GetDevice(), direct3D->GetDeviceContext(), modelFilename, textureFilename);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object", L"Error", MB_OK);
		return false;
	}
	*/

	light = new Light;
	if(!light)
	{
		return false;
	}
	light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	light->SetDirection(1.0f, 0.0f, 1.0f);
	light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	light->SetSpecularPower(32.0f);

	lightShader = new LightShader;
	result = lightShader->Initialize(direct3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object", L"Error", MB_OK);
		return false;
	}

	// Create the bitmap object.
	bitmap = new Bitmap;
	if (!bitmap)
	{
		return false;
	}

	// Initialize the bitmap object.
	strcpy_s(bitmapFilename, "seafloor.dds");
	result = bitmap->Initialize(direct3D->GetDevice(), direct3D->GetDeviceContext(), screenWidth, screenHeight, bitmapFilename, 32, 32);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}
	
	textureShader = new TextureShader;
	result = textureShader->Initialize(direct3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object", L"Error", MB_OK);
		return false;
	}

	// Create the text object.
	text = new Text;
	if (!text)
	{
		return false;
	}

	// Initialize the text object.
	result = text->Initialize(direct3D->GetDevice(), direct3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}
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

bool Graphics::Frame(int fps, int cpu, float frameTime, int mouseX, int mouseY)
{
	bool result = false;

	//result = text->SetMousePosition(mouseX, mouseY, direct3D->GetDeviceContext());
	result = text->SetFps(fps, direct3D->GetDeviceContext());
	if(!result)
	{
		return false;
	}

	result = text->SetCpu(cpu, direct3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}


	Render(mouseX, mouseY);
	return true;
}

bool Graphics::Render(int mouseX, int mouseY)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	bool result;

	// Clear the buffers to begin the scene.
	direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position
	camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	camera->GetViewMatrix(viewMatrix);
	direct3D->GetWorldMatrix(worldMatrix);
	direct3D->GetProjectionMatrix(projectionMatrix);
	direct3D->GetOrthoMatrix(orthoMatrix);

	// Turn off the Z buffer to begin all 2D rendering.
	direct3D->TurnZBufferOff();

	// Turn on the alpha blending before rendering the text.
	direct3D->TurnOnAlphaBlending();

	// Render the text strings.
	result = text->Render(direct3D->GetDeviceContext(), worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}

	result = bitmap->Render(direct3D->GetDeviceContext(), mouseX, mouseY);
	if (!result)
	{
		return false;
	}

	/*
	// Put the bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
	result = bitmap->Render(direct3D->GetDeviceContext(), 100, 100);
	if(!result)
	{
		return false;
	}

	// Render the bitmap with the texture shader.
	result = textureShader->Render(direct3D->GetDeviceContext(), bitmap->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, bitmap->GetTexture());
	if(!result)
	{
		return false;
	}
	

	// Rotate the world matrix by the rotation value so that the triangle will spin.
	worldMatrix = XMMatrixRotationY(rotation);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	model->Render(direct3D->GetDeviceContext());

	// Render the model using the light shader.
	result = lightShader->Render(direct3D->GetDeviceContext(), model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		model->GetTexture(), light->GetDirection(), light->GetAmbientColor(), light->GetDiffuseColor(), camera->GetPosition(),
		light->GetSpecularColor(), light->GetSpecularPower());
	if (!result)
	{
		return false;
	}
	*/
	result = textureShader->Render(direct3D->GetDeviceContext(), bitmap->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, bitmap->GetTexture());
	if(!result)
	{
		return false;
	}

	/*
	result = colorShader->Render(direct3D->GetDeviceContext(), model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
	if(!result)
	{
		return false;
	}
	*/
	// Present the rendered scene to the screen.

	// Turn off alpha blending after rendering the text.
	direct3D->TurnOffAlphaBlending();

	// Turn the Z buffer back on now that all 2D rendering has completed.
	direct3D->TurnZBufferOn();

	direct3D->EndScene();
	return true;
}


