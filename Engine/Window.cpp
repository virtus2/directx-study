#include "pch.h"
#include "Window.h"

Window::Window()
{
}

Window::~Window()
{
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_SIZE:
        // TODO: window Ŭ���� Resize �Լ� ����
        // Display::Resize((UINT)(UINT64)lParam & 0xFFFF, (UINT)(UINT64)lParam >> 16);
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
    // DirectXMath ���̺귯���� ���� �÷����� �����ϴ��� Ȯ���մϴ�.
    if (!DirectX::XMVerifyCPUSupport())
        return 1;

    // �����츦 �ʱ�ȭ�մϴ�.
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

    // �����츦 �����մϴ�.
    RECT rc = { 0, 0, (LONG)width, (LONG)height};
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

    hWnd = CreateWindow(className, windowName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance, nullptr);
    ASSERT(hWnd != 0);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

	return 0;
}

