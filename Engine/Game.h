﻿#pragma once

#include "pch.h"
#include "Input.h"
#include "Graphics.h"
#include "Window.h"
#include "Display.h"
#include "Model.h"
#include "ModelLoader.h"

class Entity;
class Shader;
class Material;
class Camera;
class Timer;

namespace Engine
{
    class Game
    {
    public:
        Game();
        virtual ~Game();
        
        virtual void BeginRun() {}
        virtual void OnUpdate(float deltaTime) {}

        void Run(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow, int width, int height);
        void Update(float deltaTime);
        void Render();

        std::shared_ptr<Entity> CreateEntity();
        std::shared_ptr<Camera> CreateCamera();
        std::shared_ptr<Model> CreateModel(const std::string& filePath);
        std::shared_ptr<Shader> CreateShader(const std::wstring& vertexShaderFilePath, const std::wstring& pixelShaderFilePath);
        std::shared_ptr<Material> CreateMaterial();
        
        // 테스트 Primitives
        std::shared_ptr<Model> CreateRectangle();
        std::shared_ptr<Model> CreateCube();

        Input* GetInput() { return input.get(); }
        Graphics* GetGraphics() { return graphics.get(); }
        Window* GetWindow() { return window.get(); }
        Display* GetDisplay() { return display.get(); }

    protected:
        int width;
        int height;
        bool isRunning = false;

        std::vector<std::shared_ptr<Entity>> entities;
        std::shared_ptr<Camera> mainCamera;

    private:
        std::unique_ptr<Input> input;
        std::unique_ptr<Graphics> graphics;
        std::unique_ptr<Window> window;
        std::unique_ptr<Display> display;
        std::unique_ptr<ModelLoader> modelLoader;
        std::unique_ptr<Timer> timer;
    };
}