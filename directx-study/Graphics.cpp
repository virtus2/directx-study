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
	frustum = 0;
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
	
	// Create and initialize the model object.
	strcpy_s(modelFilename, "square.txt");
	wchar_t textureFilename1[128];
	wchar_t textureFilename2[128];
	wcscpy_s(textureFilename1, 128, L"stone01.dds");
	wcscpy_s(textureFilename2, 128, L"light01.dds");
	model = new Model;
	result = model->Initialize(direct3D->GetDevice(), modelFilename, textureFilename1, textureFilename2);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object", L"Error", MB_OK);
		return false;
	}

	multiTextureShader = new MultiTextureShader;
	result = multiTextureShader->Initialize(direct3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the multi texture shader object", L"Error", MB_OK);
		return false;
	}

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

	modelList = new ModelList;
	result = modelList->Initialize(25);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the model list object.", L"Error", MB_OK);
		return false;
	}

	frustum = new Frustum;

	lightMapShader = new LightMapShader;
	result = lightMapShader->Initialize(direct3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the light map shader object.", L"Error", MB_OK);
		return false;
	}

	return true;
	
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
	camera->SetPosition(0.0f, 0.0f, -10.0f);

	// Set the rotation of the camera.
	camera->SetRotation(0.0f, rotationY, 0.0f);

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
	int modelCount, renderCount;
	float positionX, positionY, positionZ;
	XMFLOAT4 color;
	bool result, renderModel;

	// Clear the buffers to begin the scene.
	direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position
	camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	camera->GetViewMatrix(viewMatrix);
	direct3D->GetWorldMatrix(worldMatrix);
	direct3D->GetProjectionMatrix(projectionMatrix);
	direct3D->GetOrthoMatrix(orthoMatrix);

	model->Render(direct3D->GetDeviceContext());

	result = lightMapShader->Render(direct3D->GetDeviceContext(), model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, 
		model->GetTextureArray());
	if(!result)
	{
		return false;
	}

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

	// Turn off alpha blending after rendering the text.
	direct3D->TurnOffAlphaBlending();

	// Turn the Z buffer back on now that all 2D rendering has completed.
	direct3D->TurnZBufferOn();

	// Present the rendered scene to the screen.
	direct3D->EndScene();
	return true;
}


