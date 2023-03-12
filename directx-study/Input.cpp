#include "Input.h"

Input::Input()
{
	directInput = 0;
	keyboard = 0;
	mouse = 0;
}

Input::Input(const Input&)
{
	
}

Input::~Input()
{
	
}

bool Input::Initialize(HINSTANCE hInstance, HWND hwnd, int screenWidth, int screenHeight)
{
	HRESULT result;

	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;

	mouseX = 0;
	mouseY = 0;

	// Initialize the main direct input interface.
	result = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&directInput, NULL);
	if(FAILED(result))
	{
		return false;
	}

	// Initialize the direct input interface for the keyboard.
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	if(FAILED(result))
	{
		return false;
	}

	// Set the data format.  In this case since it is a keyboard we can use the predefined data format.
	result = keyboard->SetDataFormat(&c_dfDIKeyboard);
	if(FAILED(result))
	{
		return false;
	}

	// Set the cooperative level of the keyboard to not share with other programs.
	result = keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}

	// Now acquire the keyboard.
	result = keyboard->Acquire();
	if (FAILED(result))
	{
		return false;
	}

	// Initialize the direct input interface for the mouse.
	result = directInput->CreateDevice(GUID_SysMouse, &mouse, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Set the data format for the mouse using the pre-defined mouse data format.
	result = mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(result))
	{
		return false;
	}

	// Set the cooperative level of the mouse to share with other programs.
	result = mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}

	// Acquire the mouse.
	result = mouse->Acquire();
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void Input::Shutdown()
{
	// Release the mouse.
	if (mouse)
	{
		mouse->Unacquire();
		mouse->Release();
		mouse = 0;
	}

	// Release the keyboard.
	if (keyboard)
	{
		keyboard->Unacquire();
		keyboard->Release();
		keyboard = 0;
	}

	// Release the main interface to direct input.
	if (directInput)
	{
		directInput->Release();
		directInput = 0;
	}
}

bool Input::Frame()
{
	bool result;
	
	// Read the current state of the keyboard.
	result = ReadKeyboard();
	if (!result)
	{
		return false;
	}

	// Read the current state of the mouse.
	result = ReadMouse();
	if (!result)
	{
		return false;
	}

	// Process the changes in the mouse and keyboard.
	ProcessInput();

	return true;
}

bool Input::IsEscapePressed()
{
	// Do a bitwise and on the keyboard state to check if the escape key is currently being pressed.
	if (keyboardState[DIK_ESCAPE] & 0x80)
	{
		return true;
	}

	return false;
}

bool Input::IsKeyPressed(int keyCode)
{
	// Do a bitwise and on the keyboard state to check if the escape key is currently being pressed.
	if (keyboardState[keyCode] & 0x80)
	{
		return true;
	}

	return false;
}

void Input::GetMouseLocation(int& mouseX, int& mouseY)
{
	mouseX = this->mouseX;
	mouseY = this->mouseY;
}

bool Input::ReadKeyboard()
{
	HRESULT result;
	
	// Read the keyboard device.
	result = keyboard->GetDeviceState(sizeof(keyboardState), (LPVOID)&keyboardState);
	if (FAILED(result))
	{
		// If the keyboard lost focus or was not acquired then try to get control back.
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			keyboard->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

bool Input::ReadMouse()
{
	HRESULT result;
	
	// Read the mouse device.
	result = mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&mouseState);
	if (FAILED(result))
	{
		// If the mouse lost focus or was not acquired then try to get control back.
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			mouse->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

void Input::ProcessInput()
{
	// Update the location of the mouse cursor based on the change of the mouse location during the frame.
	mouseX += mouseState.lX;
	mouseY += mouseState.lY;

	// Ensure the mouse location doesn't exceed the screen width or height.
	if (mouseX < 0) { mouseX = 0; }
	if (mouseY < 0) { mouseY = 0; }

	if (mouseX > screenWidth) { mouseX = screenWidth; }
	if (mouseY > screenHeight) { mouseY = screenHeight; }
}



