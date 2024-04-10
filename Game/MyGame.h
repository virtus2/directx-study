#pragma once
#include "Game.h"
#pragma comment(lib, "Engine.lib")

// TODO: 애플리케이션과 엔진 네임스페이스 분리
class MyGame : public Engine::Game
{
public:
    MyGame() {}
    ~MyGame() {}

    virtual void BeginRun() override;
    virtual void OnUpdate(float deltaTime) override;

private:
	std::shared_ptr<Entity> entity;
    std::shared_ptr<Model> model;
    std::shared_ptr<Shader> shader;
    std::shared_ptr<Material> material;
    std::shared_ptr<Camera> camera;
};