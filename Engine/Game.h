#pragma once

#include "pch.h"
#include "Input.h"
#include "Graphics.h"
#include "Window.h"
#include "Display.h"
#include "ModelLoader.h"

namespace Engine
{
    class Game
    {
    public:
        Game();
        virtual ~Game();

        void Run(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow, int width, int height);
        void Update();
        void Render();

        Input* GetInput() { return input.get(); }
        Graphics* GetGraphics() { return graphics.get(); }
        Window* GetWindow() { return window.get(); }
        Display* GetDisplay() { return display.get(); }

    protected:
        int width;
        int height;
        bool isRunning = false;

        std::unique_ptr<Input> input;
        std::unique_ptr<Graphics> graphics;
        std::unique_ptr<Window> window;
        std::unique_ptr<Display> display;
        std::unique_ptr<ModelLoader> modelLoader;
    };
}