#pragma once

#include "pch.h"

#include <Xinput.h>
#pragma comment(lib, "xinput9_1_0.lib")

#define USE_KEYBOARD_MOUSE
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

class Input
{
public:
	enum DigitalInput
	{
		// Keyboard
		// must start at zero
		Key_Escape = 0,
		Key_1,
		Key_2,
		Key_3,
		Key_4,
		Key_5,
		Key_6,
		Key_7,
		Key_8,
		Key_9,
		Key_0,
		Key_Minus,
		Key_Equals,
		Key_Back,
		Key_Tab,
		Key_Q,
		Key_W,
		Key_E,
		Key_R,
		Key_T,
		Key_Y,
		Key_U,
		Key_I,
		Key_O,
		Key_P,
		Key_LeftBracket,
		Key_RightBracket,
		Key_Return,
		Key_LeftControl,
		Key_A,
		Key_S,
		Key_D,
		Key_F,
		Key_G,
		Key_H,
		Key_J,
		Key_K,
		Key_L,
		Key_Semicolon,
		Key_Apostrophe,
		Key_Grave,
		Key_LeftShift,
		Key_Backslash,
		Key_Z,
		Key_X,
		Key_C,
		Key_V,
		Key_B,
		Key_N,
		Key_M,
		Key_Comma,
		Key_Period,
		Key_Slash,
		Key_RightShift,
		Key_Multiply,
		Key_LeftAlt,
		Key_Space,
		Key_Capital,
		Key_F1,
		Key_F2,
		Key_F3,
		Key_F4,
		Key_F5,
		Key_F6,
		Key_F7,
		Key_F8,
		Key_F9,
		Key_F10,
		Key_NumLock,
		Key_Scroll,
		Key_Numpad7,
		Key_Numpad8,
		Key_Numpad9,
		Key_Subtract,
		Key_Numpad4,
		Key_Numpad5,
		Key_Numpad6,
		Key_Add,
		Key_Numpad1,
		Key_Numpad2,
		Key_Numpad3,
		Key_Numpad0,
		Key_Decimal,
		Key_F11,
		Key_F12,
		Key_NumpadEnter,
		Key_RightControl,
		Key_Divide,
		Key_PrintScreen,
		Key_RightAlt,
		Key_Pause,
		Key_Home,
		Key_Up,
		Key_PageUp,
		Key_Left,
		Key_Right,
		Key_End,
		Key_Down,
		Key_PageDown,
		Key_Insert,
		Key_Delete,
		Key_LeftWindows,
		Key_RightWindows,
		Key_Apps,

		NumKeys,

		// Gamepad
		Pad_Up = NumKeys,
		Pad_Down,
		Pad_Left,
		Pad_Right,
		Pad_Start,
		Pad_Back,
		Pad_LeftStickClick,
		Pad_RightStickClick,
		Pad_LeftShoulder,
		Pad_RightShoulder,
		Pad_A,
		Pad_B,
		Pad_X,
		Pad_Y,

		// Mouse
		Mouse_0,
		Mouse_1,
		Mouse_2,
		Mouse_3,
		Mouse_4,
		Mouse_5,
		Mouse_6,
		Mouse_7,

		NumDigitalInputs
	};

	enum AnalogInput
	{
		// Gamepad
		Pad_LeftTrigger,
		Pad_RightTrigger,
		Pad_LeftStickX,
		Pad_LeftStickY,
		Pad_RightStickX,
		Pad_RightStickY,

		// Mouse
		Mouse_X,
		Mouse_Y,
		Mouse_Scroll,

		NumAnalogInputs
	};

	void Initialize(HWND window);
	void Shutdown();
	void Update();

	bool IsPressed(DigitalInput input);
	bool IsFirstPressed(DigitalInput input);
	bool IsReleased(DigitalInput input);
	bool IsFirstReleased(DigitalInput input);

	float GetAnalogInput(AnalogInput input);
	float FilterAnalogInput(int value, int deadzone);

	void InitializeKeyboardMouse();
	void BuildKeyboardMapping();
	void UpdateKeyboardMouse();

private:
	HWND window;

	bool digitalInputs[2][Input::NumDigitalInputs];
	float analogInputs[Input::NumAnalogInputs];

	// Keyboard and Mouse
	IDirectInput8A* directInput;
	IDirectInputDevice8A* keyboard;
	IDirectInputDevice8A* mouse;

	DIMOUSESTATE2 mouseState;
	unsigned char keyBuffer[256];
	unsigned char directXKeyMapping[Input::NumKeys];
};