#pragma once
class Window
{
public:
	Window();
	~Window();

	int Initialize(HINSTANCE hInstance, int nCmdShow, const wchar_t* className, const wchar_t* windowName, int width, int height);
	
	HWND GetWindowHandle() const { return hWnd; }

private:
	HWND hWnd = nullptr;
};

