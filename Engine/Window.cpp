#include "pch.h"
#include "Window.h"

Window::Window()
{
}

Window::~Window()
{
    DestroyWindow((HWND)hWnd);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_SIZE:
        // TODO: window 클래스 Resize 함수 구현
        // Display 클래스 포인터 필요할 것 같다...
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

int Window::Initialize(HINSTANCE hInstance, int nCmdShow, const wchar_t* className, const wchar_t* windowName, int width, int height)
{
    // DirectXMath 라이브러리가 현재 플랫폼을 지원하는지 확인합니다.
    if (!DirectX::XMVerifyCPUSupport())
        return 1;

    // 윈도우를 초기화합니다.
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = className;
    wcex.hIconSm = LoadIcon(hInstance, IDI_APPLICATION);
    ASSERT(RegisterClassEx(&wcex) != 0, "Unable to register a window.");

    // 윈도우를 생성합니다.
    RECT rc = { 0, 0, (LONG)width, (LONG)height};
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

    hWnd = CreateWindow(className, windowName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance, nullptr);
    ASSERT(hWnd != 0);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

	return 0;
}

