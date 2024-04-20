#pragma once
#include "Input.h"
#include "Resource.h"
#include "Graphics.h"
#include "Window.h"
#include "Display.h"
#include "Model.h"
#include "ModelLoader.h"

#include <map>

class Entity;
class Shader;
class Material;
class Camera;
class Timer;
class Texture;
class Light;

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
        std::shared_ptr<Texture> CreateTexture(const std::wstring& filePath);
        
        // 테스트 Primitives
        std::shared_ptr<Model> CreateRectangle();
        std::shared_ptr<Model> CreateCube();

        Input* GetInput() { return input.get(); }
        std::shared_ptr<Camera> GetMainCamera() { return mainCamera; }
        std::vector<std::shared_ptr<Entity>>& GetEntities() { return entities; }
        std::vector<std::shared_ptr<Light>>& GetLights() { return lights; }

    protected:
        int width;
        int height;
        bool isRunning = false;

        /// TODO: Scene 클래스로 이동
        std::vector<std::shared_ptr<Entity>> entities;
        std::vector<std::shared_ptr<Light>> lights;
        std::shared_ptr<Camera> mainCamera;
        ////////////////////////////

    private:
        std::unique_ptr<Input> input;
        std::unique_ptr<Graphics> graphics;
        std::unique_ptr<Window> window;
        std::unique_ptr<Display> display;
        std::unique_ptr<ModelLoader> modelLoader;
        std::unique_ptr<Timer> timer;
        std::unique_ptr<Resource> resource;

        // TODO: Resource 클래스로 이동
        // TODO: 각각의 리소스 생성할 때 맵에 추가하고 이를 참조해서 사용
        std::map<std::string, std::shared_ptr<Model>> models;
        std::map<std::string, std::shared_ptr<Shader>> shaders;
        std::map<std::string, std::shared_ptr<Material>> materials;
        std::map<std::string, std::shared_ptr<Texture>> textures;
        ////////////////////////////
    };
}