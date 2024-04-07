#pragma once

#include "pch.h"
#include "Input.h"
#include "Graphics.h"
#include "Window.h"
#include "Display.h"
#include "Model.h"
#include "ModelLoader.h"

class Entity;
class Shader;

namespace Engine
{
    class Game
    {
    public:
        Game();
        virtual ~Game();
        
        virtual void BeginRun() {}
        virtual void OnUpdate() {}

        void Run(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow, int width, int height);
        void Update();
        void Render();

        std::shared_ptr<Entity> CreateEntity();
        std::shared_ptr<Model> CreateModel(const std::string& filePath);
        std::shared_ptr<Model> CreateRectangle();
        std::shared_ptr<Shader> CreateShader(const std::wstring& vertexShaderFilePath, const std::wstring& pixelShaderFilePath);

        Input* GetInput() { return input.get(); }
        Graphics* GetGraphics() { return graphics.get(); }
        Window* GetWindow() { return window.get(); }
        Display* GetDisplay() { return display.get(); }

    protected:
        int width;
        int height;
        bool isRunning = false;

        std::vector<std::shared_ptr<Entity>> entities;

    private:
        std::unique_ptr<Input> input;
        std::unique_ptr<Graphics> graphics;
        std::unique_ptr<Window> window;
        std::unique_ptr<Display> display;
        std::unique_ptr<ModelLoader> modelLoader;
    };
}