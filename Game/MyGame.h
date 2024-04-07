﻿#pragma once
#include "Game.h"
#pragma comment(lib, "Engine.lib")

// TODO: 애플리케이션과 엔진 네임스페이스 분리
class MyGame : public Engine::Game
{
public:
    MyGame() {}
    ~MyGame() {}

    virtual void BeginRun() override;
    virtual void OnUpdate() override;

private:
	std::shared_ptr<Entity> entity;
};