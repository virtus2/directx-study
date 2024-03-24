#include "pch.h"
#include "Input.h"

void Input::Initialize(HWND window)
{
    // Initialize the input system
    this->window = window;

    ZeroMemory(digitalInputs, sizeof(digitalInputs));
    ZeroMemory(analogInputs, sizeof(analogInputs));

    InitializeKeyboardMouse();
}

float Input::FilterAnalogInput(int value, int deadzone)
{
    if (value < 0)
    {
        if (value > -deadzone)
            return 0.0f;
        else
            return (value + deadzone) / (32768.0f - deadzone);
    }
    else
    {
        if (value < deadzone)
            return 0.0f;
        else
            return (value - deadzone) / (32767.0f - deadzone);
    }
}

void Input::InitializeKeyboardMouse()
{
    BuildKeyboardMapping();

    if (FAILED(DirectInput8Create(GetModuleHandle(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, nullptr)))
        ASSERT(false, "DirectInput8 initialization failed.");

    if (FAILED(directInput->CreateDevice(GUID_SysKeyboard, &keyboard, nullptr)))
        ASSERT(false, "Keyboard CreateDevice failed.");
    if (FAILED(keyboard->SetDataFormat(&c_dfDIKeyboard)))
        ASSERT(false, "Keyboard SetDataFormat failed.");
    if (FAILED(keyboard->SetCooperativeLevel(window, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
        ASSERT(false, "Keyboard SetCooperativeLevel failed.");

    DIPROPDWORD dipdw;
    dipdw.diph.dwSize = sizeof(DIPROPDWORD);
    dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    dipdw.diph.dwObj = 0;
    dipdw.diph.dwHow = DIPH_DEVICE;
    dipdw.dwData = 10;
    if (FAILED(keyboard->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph)))
        ASSERT(false, "Keyboard set buffer size failed.");

    if (FAILED(directInput->CreateDevice(GUID_SysMouse, &mouse, nullptr)))
        ASSERT(false, "Mouse CreateDevice failed.");
    if (FAILED(mouse->SetDataFormat(&c_dfDIMouse2)))
        ASSERT(false, "Mouse SetDataFormat failed.");
    if (FAILED(mouse->SetCooperativeLevel(window, DISCL_FOREGROUND | DISCL_EXCLUSIVE)))
        ASSERT(false, "Mouse SetCooperativeLevel failed.");

    memset(&mouseState, 0, sizeof(DIMOUSESTATE2));
    memset(&keyBuffer, 0, sizeof(keyBuffer));
}

void Input::BuildKeyboardMapping()
{
    directXKeyMapping[Input::Key_Escape] = 1;
    directXKeyMapping[Input::Key_1] = 2;
    directXKeyMapping[Input::Key_2] = 3;
    directXKeyMapping[Input::Key_3] = 4;
    directXKeyMapping[Input::Key_4] = 5;
    directXKeyMapping[Input::Key_5] = 6;
    directXKeyMapping[Input::Key_6] = 7;
    directXKeyMapping[Input::Key_7] = 8;
    directXKeyMapping[Input::Key_8] = 9;
    directXKeyMapping[Input::Key_9] = 10;
    directXKeyMapping[Input::Key_0] = 11;
    directXKeyMapping[Input::Key_Minus] = 12;
    directXKeyMapping[Input::Key_Equals] = 13;
    directXKeyMapping[Input::Key_Back] = 14;
    directXKeyMapping[Input::Key_Tab] = 15;
    directXKeyMapping[Input::Key_Q] = 16;
    directXKeyMapping[Input::Key_W] = 17;
    directXKeyMapping[Input::Key_E] = 18;
    directXKeyMapping[Input::Key_R] = 19;
    directXKeyMapping[Input::Key_T] = 20;
    directXKeyMapping[Input::Key_Y] = 21;
    directXKeyMapping[Input::Key_U] = 22;
    directXKeyMapping[Input::Key_I] = 23;
    directXKeyMapping[Input::Key_O] = 24;
    directXKeyMapping[Input::Key_P] = 25;
    directXKeyMapping[Input::Key_LeftBracket] = 26;
    directXKeyMapping[Input::Key_RightBracket] = 27;
    directXKeyMapping[Input::Key_Return] = 28;
    directXKeyMapping[Input::Key_LeftControl] = 29;
    directXKeyMapping[Input::Key_A] = 30;
    directXKeyMapping[Input::Key_S] = 31;
    directXKeyMapping[Input::Key_D] = 32;
    directXKeyMapping[Input::Key_F] = 33;
    directXKeyMapping[Input::Key_G] = 34;
    directXKeyMapping[Input::Key_H] = 35;
    directXKeyMapping[Input::Key_J] = 36;
    directXKeyMapping[Input::Key_K] = 37;
    directXKeyMapping[Input::Key_L] = 38;
    directXKeyMapping[Input::Key_Semicolon] = 39;
    directXKeyMapping[Input::Key_Apostrophe] = 40;
    directXKeyMapping[Input::Key_Grave] = 41;
    directXKeyMapping[Input::Key_LeftShift] = 42;
    directXKeyMapping[Input::Key_Backslash] = 43;
    directXKeyMapping[Input::Key_Z] = 44;
    directXKeyMapping[Input::Key_X] = 45;
    directXKeyMapping[Input::Key_C] = 46;
    directXKeyMapping[Input::Key_V] = 47;
    directXKeyMapping[Input::Key_B] = 48;
    directXKeyMapping[Input::Key_N] = 49;
    directXKeyMapping[Input::Key_M] = 50;
    directXKeyMapping[Input::Key_Comma] = 51;
    directXKeyMapping[Input::Key_Period] = 52;
    directXKeyMapping[Input::Key_Slash] = 53;
    directXKeyMapping[Input::Key_RightShift] = 54;
    directXKeyMapping[Input::Key_Multiply] = 55;
    directXKeyMapping[Input::Key_LeftAlt] = 56;
    directXKeyMapping[Input::Key_Space] = 57;
    directXKeyMapping[Input::Key_Capital] = 58;
    directXKeyMapping[Input::Key_F1] = 59;
    directXKeyMapping[Input::Key_F2] = 60;
    directXKeyMapping[Input::Key_F3] = 61;
    directXKeyMapping[Input::Key_F4] = 62;
    directXKeyMapping[Input::Key_F5] = 63;
    directXKeyMapping[Input::Key_F6] = 64;
    directXKeyMapping[Input::Key_F7] = 65;
    directXKeyMapping[Input::Key_F8] = 66;
    directXKeyMapping[Input::Key_F9] = 67;
    directXKeyMapping[Input::Key_F10] = 68;
    directXKeyMapping[Input::Key_NumLock] = 69;
    directXKeyMapping[Input::Key_Scroll] = 70;
    directXKeyMapping[Input::Key_Numpad7] = 71;
    directXKeyMapping[Input::Key_Numpad8] = 72;
    directXKeyMapping[Input::Key_Numpad9] = 73;
    directXKeyMapping[Input::Key_Subtract] = 74;
    directXKeyMapping[Input::Key_Numpad4] = 75;
    directXKeyMapping[Input::Key_Numpad5] = 76;
    directXKeyMapping[Input::Key_Numpad6] = 77;
    directXKeyMapping[Input::Key_Add] = 78;
    directXKeyMapping[Input::Key_Numpad1] = 79;
    directXKeyMapping[Input::Key_Numpad2] = 80;
    directXKeyMapping[Input::Key_Numpad3] = 81;
    directXKeyMapping[Input::Key_Numpad0] = 82;
    directXKeyMapping[Input::Key_Decimal] = 83;
    directXKeyMapping[Input::Key_F11] = 87;
    directXKeyMapping[Input::Key_F12] = 88;
    directXKeyMapping[Input::Key_NumpadEnter] = 156;
    directXKeyMapping[Input::Key_RightControl] = 157;
    directXKeyMapping[Input::Key_Divide] = 181;
    directXKeyMapping[Input::Key_PrintScreen] = 183;
    directXKeyMapping[Input::Key_RightAlt] = 184;
    directXKeyMapping[Input::Key_Pause] = 197;
    directXKeyMapping[Input::Key_Home] = 199;
    directXKeyMapping[Input::Key_Up] = 200;
    directXKeyMapping[Input::Key_PageUp] = 201;
    directXKeyMapping[Input::Key_Left] = 203;
    directXKeyMapping[Input::Key_Right] = 205;
    directXKeyMapping[Input::Key_End] = 207;
    directXKeyMapping[Input::Key_Down] = 208;
    directXKeyMapping[Input::Key_PageDown] = 209;
    directXKeyMapping[Input::Key_Insert] = 210;
    directXKeyMapping[Input::Key_Delete] = 211;
    directXKeyMapping[Input::Key_LeftWindows] = 219;
    directXKeyMapping[Input::Key_RightWindows] = 220;
    directXKeyMapping[Input::Key_Apps] = 221;
}

void Input::UpdateKeyboardMouse()
{
    HWND foregroundWindow = GetForegroundWindow();
    bool visible = IsWindowVisible(foregroundWindow) != 0;
    if (foregroundWindow != window || !visible)
    {
        memset(&mouseState, 0, sizeof(DIMOUSESTATE2));
        memset(&keyBuffer, 0, sizeof(keyBuffer));
        return;
    }
    else
    {
        mouse->Acquire();
        mouse->GetDeviceState(sizeof(DIMOUSESTATE2), &mouseState);
        keyboard->Acquire();
        keyboard->GetDeviceState(sizeof(keyBuffer), keyBuffer);
    }
}

void Input::Shutdown()
{
}

void Input::Update()
{
    memcpy(digitalInputs[1], digitalInputs[0], sizeof(digitalInputs[0]));
    memset(digitalInputs[0], 0, sizeof(digitalInputs[0]));
    memset(analogInputs, 0, sizeof(analogInputs));

    XINPUT_STATE newInputState;
    if (ERROR_SUCCESS == XInputGetState(0, &newInputState))
    {
        digitalInputs[0][Pad_Up] = !!(newInputState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP);
        digitalInputs[0][Pad_Down] = !!(newInputState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN);
        digitalInputs[0][Pad_Left] = !!(newInputState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT);
        digitalInputs[0][Pad_Right] = !!(newInputState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT);
        digitalInputs[0][Pad_Start] = !!(newInputState.Gamepad.wButtons & XINPUT_GAMEPAD_START);
        digitalInputs[0][Pad_Back] = !!(newInputState.Gamepad.wButtons & XINPUT_GAMEPAD_BACK);
        digitalInputs[0][Pad_LeftStickClick] = !!(newInputState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB);
        digitalInputs[0][Pad_RightStickClick] = !!(newInputState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB);
        digitalInputs[0][Pad_LeftShoulder] = !!(newInputState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER);
        digitalInputs[0][Pad_RightShoulder] = !!(newInputState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER);
        digitalInputs[0][Pad_A] = !!(newInputState.Gamepad.wButtons & XINPUT_GAMEPAD_A);
        digitalInputs[0][Pad_B] = !!(newInputState.Gamepad.wButtons & XINPUT_GAMEPAD_B);
        digitalInputs[0][Pad_X] = !!(newInputState.Gamepad.wButtons & XINPUT_GAMEPAD_X);
        digitalInputs[0][Pad_Y] = !!(newInputState.Gamepad.wButtons & XINPUT_GAMEPAD_Y);

        analogInputs[Pad_LeftTrigger] = newInputState.Gamepad.bLeftTrigger / 255.0f;
        analogInputs[Pad_RightTrigger] = newInputState.Gamepad.bRightTrigger / 255.0f;
        analogInputs[Pad_LeftStickX] = FilterAnalogInput(newInputState.Gamepad.sThumbLX, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
        analogInputs[Pad_LeftStickY] = FilterAnalogInput(newInputState.Gamepad.sThumbLY, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
        analogInputs[Pad_RightStickX] = FilterAnalogInput(newInputState.Gamepad.sThumbRX, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
        analogInputs[Pad_RightStickY] = FilterAnalogInput(newInputState.Gamepad.sThumbRY, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
    }

    UpdateKeyboardMouse();
}

