#include "Application.h"

Application::Application()
{
	input = 0;
	direct3D = 0;
	camera = 0;
	terrain = 0;
	colorShader = 0;
	timer = 0;
	position = 0;
	fpsCounter = 0;
	cpuUsage = 0;
	fontShader= 0;
	text = 0;
}


Application::Application(const Application& other)
{
}


Application::~Application()
{
}


bool Application::Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
{
	bool result;
	float cameraX, cameraY, cameraZ;
	XMMATRIX baseViewMatrix;
	char videoCard[128];
	int videoMemory;

	// Create the input object.  The input object will be used to handle reading the keyboard and mouse input from the user.
	input = new Input;
	if (!input)
	{
		return false;
	}

	// Initialize the input object.
	result = input->Initialize(hinstance, hwnd, screenWidth, screenHeight);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the input object.", L"Error", MB_OK);
		return false;
	}

	// Create the Direct3D object.
	direct3D = new D3DClass;
	if (!direct3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize DirectX 11.", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	camera = new Camera;
	if (!camera)
	{
		return false;
	}

	// Initialize a base view matrix with the camera for 2D user interface rendering.
	camera->SetPosition(0.0f, 0.0f, -1.0f);
	camera->Render();
	camera->GetViewMatrix(baseViewMatrix);

	// Set the initial position of the camera.
	cameraX = 50.0f;
	cameraY = 2.0f;
	cameraZ = -7.0f;

	camera->SetPosition(cameraX, cameraY, cameraZ);

	// Create the terrain object.
	terrain = new Terrain;
	if (!terrain)
	{
		return false;
	}

	// Initialize the terrain object.
	result = terrain->Initialize(direct3D->GetDevice());
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the terrain object.", L"Error", MB_OK);
		return false;
	}

	// Create the color shader object.
	colorShader = new ColorShader;
	if (!colorShader)
	{
		return false;
	}

	// Initialize the color shader object.
	result = colorShader->Initialize(direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the timer object.
	timer = new Timer;
	if (!timer)
	{
		return false;
	}

	// Initialize the timer object.
	result = timer->Initialize();
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the timer object.", L"Error", MB_OK);
		return false;
	}

	// Create the position object.
	position = new Position;
	if (!position)
	{
		return false;
	}

	// Set the initial position of the viewer to the same as the initial camera position.
	position->SetPosition(cameraX, cameraY, cameraZ);
	

	// Create the fps object.
	fpsCounter = new FpsCounter;
	if (!fpsCounter)
	{
		return false;
	}

	// Initialize the fps object.
	fpsCounter->Initialize();
	
	// Create the cpu object.
	cpuUsage = new CpuUsage;
	if (!cpuUsage)
	{
		return false;
	}

	// Initialize the cpu object.
	cpuUsage->Initialize();

	// Create the font shader object.
	fontShader = new FontShader;
	if (!fontShader)
	{
		return false;
	}

	// Initialize the font shader object.
	result = fontShader->Initialize(direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the font shader object.", L"Error", MB_OK);
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

	// Retrieve the video card information.
	direct3D->GetVideoCardInfo(videoCard, videoMemory);

	/*
	// Set the video card information in the text object.
	result = text->SetVideoCardInfo(videoCard, videoMemory, direct3D->GetDeviceContext());
	if (!result)
	{
		MessageBox(hwnd, L"Could not set video card info in the text object.", L"Error", MB_OK);
		return false;
	}
	*/
	return true;
}

void Application::Shutdown()
{
	// Release the text object.
	if (text)
	{
		text->Shutdown();
		delete text;
		text = 0;
	}

	// Release the font shader object.
	if (fontShader)
	{
		fontShader->Shutdown();
		delete fontShader;
		fontShader = 0;
	}

	// Release the cpu object.
	if (cpuUsage)
	{
		cpuUsage->Shutdown();
		delete cpuUsage;
		cpuUsage = 0;
	}

	// Release the fps object.
	if (fpsCounter)
	{
		delete fpsCounter;
		fpsCounter = 0;
	}

	// Release the position object.
	if (position)
	{
		delete position;
		position = 0;
	}

	// Release the timer object.
	if (timer)
	{
		delete timer;
		timer = 0;
	}

	// Release the color shader object.
	if (colorShader)
	{
		colorShader->Shutdown();
		delete colorShader;
		colorShader = 0;
	}

	// Release the terrain object.
	if (terrain)
	{
		terrain->Shutdown();
		delete terrain;
		terrain = 0;
	}

	// Release the camera object.
	if (camera)
	{
		delete camera;
		camera = 0;
	}

	// Release the Direct3D object.
	if (direct3D)
	{
		direct3D->Shutdown();
		delete direct3D;
		direct3D = 0;
	}

	// Release the input object.
	if (input)
	{
		input->Shutdown();
		delete input;
		input = 0;
	}

	return;
}

bool Application::Frame()
{
	bool result;

	// Read the user input.
	result = input->Frame();
	if (!result)
	{
		return false;
	}

	// Check if the user pressed escape and wants to exit the application.
	if (input->IsEscapePressed() == true)
	{
		return false;
	}

	// Update the system stats.
	timer->Frame();
	fpsCounter->Frame();
	cpuUsage->Frame();

	// Update the FPS value in the text object.
	result = text->SetFps(fpsCounter->GetFps(), direct3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	// Update the CPU usage value in the text object.
	result = text->SetCpu(cpuUsage->GetCpuPercentage(), direct3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	// Do the frame input processing.
	result = HandleInput(timer->GetTime());
	if (!result)
	{
		return false;
	}

	// Render the graphics.
	result = RenderGraphics();
	if (!result)
	{
		return false;
	}

	return result;
}

bool Application::HandleInput(float frameTime)
{
	bool keyDown, result;
	float posX, posY, posZ, rotX, rotY, rotZ;
	

	// Set the frame time for calculating the updated position.
	position->SetFrameTime(frameTime);
	
	// Handle the input.
	keyDown = input->IsKeyPressed(DIK_LEFTARROW);
	position->TurnLeft(keyDown);

	keyDown = input->IsKeyPressed(DIK_RIGHTARROW);
	position->TurnRight(keyDown);

	keyDown = input->IsKeyPressed(DIK_UPARROW);
	position->MoveForward(keyDown);

	keyDown = input->IsKeyPressed(DIK_DOWNARROW);
	position->MoveBackward(keyDown);

	keyDown = input->IsKeyPressed(DIK_A);
	position->MoveUpward(keyDown);

	keyDown = input->IsKeyPressed(DIK_Z);
	position->MoveDownward(keyDown);

	keyDown = input->IsKeyPressed(DIK_PGUP);
	position->LookUpward(keyDown);

	keyDown = input->IsKeyPressed(DIK_PGDN);
	position->LookDownward(keyDown);
	
	// Get the view point position/rotation.
	position->GetPosition(posX, posY, posZ);
	position->GetRotation(rotX, rotY, rotZ);

	// Set the position of the camera.
	camera->SetPosition(posX, posY, posZ);
	camera->SetRotation(rotX, rotY, rotZ);

	/*
	// Update the position values in the text object.
	result = text->SetCameraPosition(posX, posY, posZ, m_Direct3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	// Update the rotation values in the text object.
	result = text->SetCameraRotation(rotX, rotY, rotZ, m_Direct3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}*/

	return true;
}

bool Application::RenderGraphics()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	bool result;

	// Clear the scene.
	direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	camera->Render();

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	direct3D->GetWorldMatrix(worldMatrix);
	camera->GetViewMatrix(viewMatrix);
	direct3D->GetProjectionMatrix(projectionMatrix);
	direct3D->GetOrthoMatrix(orthoMatrix);

	// Render the terrain buffers.
	terrain->Render(direct3D->GetDeviceContext());

	// Render the model using the color shader.
	result = colorShader->Render(direct3D->GetDeviceContext(), terrain->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}

	// Turn off the Z buffer to begin all 2D rendering.
	direct3D->TurnZBufferOff();

	// Turn on the alpha blending before rendering the text.
	direct3D->TurnOnAlphaBlending();

	// Render the text user interface elements.
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