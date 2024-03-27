#include "pch.h"
#include "Game.h"
#include "Input.h"
#include "Graphics.h"
#include "Window.h"

namespace Engine
{
	Game::Game()
	{
        input = std::make_unique<Input>();
        graphics = std::make_unique<Graphics>();
        window = std::make_unique<Window>();
		display = std::make_unique<Display>();
	}

	Game::~Game()
	{
		
	}

    void Game::Run(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow, int width, int height)
    {
		this->width = width;
		this->height = height;

		// 윈도우 초기화
		ASSERT(window->Initialize(hInstance, nCmdShow, L"WindowClass", L"Window", width, height) == 0, "Window initialization failed.");
		
		// 인풋 초기화
		HWND hWnd = window->GetWindowHandle();
		ASSERT(input->Initialize(hWnd) == 0, "Input initialization failed.");

		// DirectX 11, 그래픽 관련 초기화
		ASSERT(graphics->Initialize(display.get(), hWnd, width, height) == 0, "Graphics initialization failed.");

		// 디스플레이 초기화
		ASSERT(display->Initialize(graphics.get(), hWnd, width, height) == 0, "Display initialization failed.");

		isRunning = true;
		while (isRunning)
		{
			MSG msg;
			while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT)
				{
					isRunning = false;
				}
				else
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}

			if (!isRunning)
			{
				break;
			}
			Update();
			Render();
		}
    }

	void Game::Update()
	{
		input->Update();
	}

	void Game::Render()
	{
		// 파이프라인 단계는 다음과 같습니다.
		// Input Assembler 
		// Vertex Shader 
		// Hull Shader
		// Tessellator
		// Domain Shader
		// Geometry Shader
		// Rasterizer
		// Pixel Shader
		// Output Merger

		display->SetViewport(width, height);
		graphics->ClearColor(0.0f, 0.0f, 0.75f, 1.0f);
		graphics->SetRasterizerState();
		// TODO: 메쉬 렌더링

		display->GetSwapChain()->Present(1, 0);
	}
}
