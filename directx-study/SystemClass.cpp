#include "SystemClass.h"

SystemClass::SystemClass()
{
	input = 0;
	graphics = 0;
}

SystemClass::SystemClass(const SystemClass&)
{
	
}

SystemClass::~SystemClass()
{
	
}

bool SystemClass::Initialize()
{
	int screenWidth = 0, screenHeight = 0;
	bool result = false;

	InitializeWindows(screenWidth, screenHeight);
	input = new Input;
	result = input->Initialize(hInstance, hWnd, screenWidth, screenHeight);
	if(!result)
	{
		MessageBox(hWnd, L"Could not initialize the input object.", L"Error", MB_OK);
		return false;
	}

	graphics = new Graphics;
	result = graphics->Initialize(screenWidth, screenHeight, hWnd);
	if(!result)
	{
		return false;
	}
	return true;
}

void SystemClass::InitializeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	ApplicationHandle = this;
	hInstance = GetModuleHandle(NULL);
	applicationName = L"Engine";

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	RegisterClassEx(&wc);

	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	if(FULL_SCREEN)
	{
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
		posX = posY = 0;
	}
	else
	{
		screenWidth = 800;
		screenHeight = 600;

		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	hWnd = CreateWindowEx(WS_EX_APPWINDOW, applicationName, applicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, screenWidth, screenHeight, NULL, NULL, hInstance, NULL);

	ShowWindow(hWnd, SW_SHOW);
	SetForegroundWindow(hWnd);
	SetFocus(hWnd);

	ShowCursor(false);
	return;
}

void SystemClass::Shutdown()
{
	if(graphics)
	{
		graphics->Shutdown();
		delete graphics;
		graphics = 0;
	}
	if(input)
	{
		input->Shutdown();
		delete input;
		input = 0;
	}

	ShutdownWindows();
}

void SystemClass::ShutdownWindows()
{
	ShowCursor(true);

	if(FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	DestroyWindow(hWnd);
	hWnd = NULL;

	UnregisterClass(applicationName, hInstance);
	hInstance = NULL;

	ApplicationHandle = NULL;
	return;
}


void SystemClass::Run()
{
	MSG msg;
	bool done = false, result = false;

	ZeroMemory(&msg, sizeof(msg));

	while(!done)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// if windows signals to end the application then exit out.
		if(msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			// otherwise do the frame processing.
			result = Frame();
			if(!result)
			{
				MessageBox(hWnd, L"Frame Processing Failed", L"Error", MB_OK);
				done = true;
			}
		}

		if (input->IsEscapePressed())
		{
			done = true;
		}
	}
}

bool SystemClass::Frame()
{
	bool result = false;
	int mouseX, mouseY;

	// Do the input frame processing.
	result = input->Frame();
	if (!result)
	{
		return false;
	}

	// Get the location of the mouse from the input object,
	input->GetMouseLocation(mouseX, mouseY);

	// Do the frame processing for the graphics object.
	result = graphics->Frame(mouseX, mouseY);
	if (!result)
	{
		return false;
	}
	
	return true;
}

LRESULT SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	return DefWindowProc(hwnd, umsg, wparam, lparam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch(umessage)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	default:
		return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
	}
}



