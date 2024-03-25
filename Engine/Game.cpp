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
	}

	Game::~Game()
	{

	}

    void Game::Run(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
    {
		if (window->Initialize(hInstance, nCmdShow, L"WindowClass", L"Window", 800, 600) == 0)
		{
			isRunning = true;
		}

		MSG msg = { 0 };
		while (isRunning)
		{
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
			}
		}
    }
}
