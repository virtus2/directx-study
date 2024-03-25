#pragma once

#include "pch.h"
#include "Input.h"
#include "Graphics.h"
#include "Window.h"

namespace Engine
{
    class Game
    {

    public:
        Game();
        virtual ~Game();

        void Run(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow);
        void Update();

    public:
        std::unique_ptr<Input> input;
        std::unique_ptr<Graphics> graphics;
        std::unique_ptr<Window> window;

    protected:
        bool isRunning = false;
    };
}