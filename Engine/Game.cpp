﻿#include "pch.h"
#include "Game.h"
#include "Input.h"
#include "Graphics.h"
#include "Window.h"
#include "Entity.h"
#include "Vertex.h"
#include "Camera.h"
#include "Timer.h"

namespace Engine
{
	Game::Game()
	{
        input = std::make_unique<Input>();
        graphics = std::make_unique<Graphics>();
        window = std::make_unique<Window>();
		display = std::make_unique<Display>();
		modelLoader = std::make_unique<ModelLoader>();
		timer = std::make_unique<Timer>();
	}

	Game::~Game()
	{
		
	}

    void Game::Run(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow, int width, int height)
    {
		this->width = width;
		this->height = height;

		window->Initialize(hInstance, nCmdShow, L"WindowClass", L"Window", width, height);

		HWND hWnd = window->GetWindowHandle();
		input->Initialize(hWnd);

		graphics->Initialize(display.get(), hWnd, width, height);

		display->Initialize(graphics.get(), hWnd, width, height);

		modelLoader->Initialize(graphics.get());

		timer->Initialize();

		// 게임 루프
		isRunning = true;
		BeginRun();
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
			timer->Tick();
			float deltaTime = timer->GetDeltaTime();
			Update(deltaTime);
			Render();
		}
    }

	void Game::Update(float deltaTime)
	{
		input->Update();
		Utility::Printf("%f\n", deltaTime);
		// TODO: Delta Time 계산해서 넘겨줘야함
		OnUpdate(deltaTime);
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

		if (mainCamera)
		{
			graphics->UpdateViewProjectionMatrix(mainCamera->GetView(), mainCamera->GetProjection());
		}
		else
		{
			// TODO: 메인카메라 없을때 에러 핸들링
		}

		for (const auto& entity : entities)
		{
			auto position = entity->GetPosition();
			auto rotation = entity->GetRotation();
			auto scale = entity->GetScale();

			// TODO: 월드행렬 계산을 graphics 클래스로 옮기는게 나을수도 있음
			auto world = XMMatrixScaling(scale.x(), scale.y(), scale.z()) 
				* XMMatrixRotationRollPitchYaw(rotation.x(), rotation.y(), rotation.z()) 
				* XMMatrixTranslation(position.x(), position.y(), position.z());
			auto model = entity->GetModel();
			graphics->UpdateWorldMatrix(world);
			if (model)
			{
				auto material = model->GetMaterial();
				if (material)
				{
					graphics->UpdateMaterialConstants(material.get());
					graphics->UseMaterial(material.get());
					graphics->DrawMesh(model->GetMesh(0).get());
				}
			}
		}

		display->GetSwapChain()->Present(1, 0);
	}

	std::shared_ptr<Entity> Game::CreateEntity()
	{
		// TODO: Entity를 생성하고 관련 데이터를 세팅한다.
		auto entity = std::make_shared<Entity>();
		entities.push_back(entity);
		return entity;
	}

	std::shared_ptr<Camera> Game::CreateCamera()
	{
		auto camera = std::make_shared<Camera>();
		// 카메라가 메인 카메라 1개밖에 없다고 가정한다.
		mainCamera = camera;
		mainCamera->SetViewParameters({ 0.0f, 0.0f, -5.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f });
		mainCamera->SetProjectionParameters(70.0f, static_cast<float>(width) / static_cast<float>(height), 0.01f, 100.0f);
		return camera;
	}

	std::shared_ptr<Model> Game::CreateModel(const std::string& filePath)
	{
		// TODO: filePath 타입을 wstring으로 바꾼다.
		auto model = std::make_shared<Model>();
		modelLoader->Load(filePath, model.get());

		// TODO: 모델 생성 타이밍과 렌더링 준비 타이밍을 분리한다.
		// 지금은 모델을 생성하자마자 바로 버텍스 버퍼, 인덱스 버퍼를 만들어주고 있다.
		// 사실 함수 이름도 마음에 안들어...
		model->PrepareRender(graphics.get());
		return model;
	}

	std::shared_ptr<Model> Game::CreateRectangle()
	{
		auto model = std::make_shared<Model>();
		auto mesh = std::make_shared<Mesh>();

		std::vector<Vertex> vertices;
		Vertex v1(Vector3(-0.5f, 0.5f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector2(0.0f, 0.0f));
		Vertex v2(Vector3(0.5f, 0.5f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector2(1.0f, 0.0f));
		Vertex v3(Vector3(0.5f, -0.5f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector2(1.0f, 1.0f));
		Vertex v4(Vector3(-0.5f, -0.5f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector2(0.0f, 1.0f));
		vertices.push_back(v1);
		vertices.push_back(v2);
		vertices.push_back(v3);
		vertices.push_back(v4);

		std::vector<uint32_t> indices = { 0, 1, 2, 0, 2, 3 };
		mesh->SetMeshData(vertices, indices);
		model->AddMesh(mesh);
		model->PrepareRender(graphics.get());
		return model;
	}

	std::shared_ptr<Model> Game::CreateCube()
	{
		auto model = std::make_shared<Model>();
		auto mesh = std::make_shared<Mesh>();
		float scale = 1.0f;
		std::vector<Vertex> vertices;

		Vertex v1(Vector3(-0.5f, 0.5f, -0.5f), Vector3(0.0f, 1.0f, 0.0f), Vector2(0.0f, 0.0f)); vertices.push_back(v1);
		Vertex v2(Vector3(0.5f, 0.5f, -0.5f), Vector3(0.0f, 1.0f, 0.0f), Vector2(1.0f, 0.0f)); vertices.push_back(v2);
		Vertex v3(Vector3(0.5f, 0.5f, 0.5f), Vector3(0.0f, 1.0f, 0.0f), Vector2(1.0f, 1.0f)); vertices.push_back(v3);
		Vertex v4(Vector3(-0.5f, 0.5f, 0.5f), Vector3(0.0f, 1.0f, 0.0f), Vector2(0.0f, 1.0f)); vertices.push_back(v4);

		Vertex v5(Vector3(-0.5f, -0.5f, 0.5f), Vector3(0.0f, -1.0f, 0.0f), Vector2(0.0f, 0.0f)); vertices.push_back(v5);
		Vertex v6(Vector3(0.5f, -0.5f, 0.5f), Vector3(0.0f, -1.0f, 0.0f), Vector2(1.0f, 0.0f)); vertices.push_back(v6);
		Vertex v7(Vector3(0.5f, -0.5f, -0.5f), Vector3(0.0f, -1.0f, 0.0f), Vector2(1.0f, 1.0f)); vertices.push_back(v7);
		Vertex v8(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.0f, -1.0f, 0.0f), Vector2(0.0f, 1.0f)); vertices.push_back(v8);

		Vertex v9(Vector3(0.5f, 0.5f, 0.5f), Vector3(1.0f, 0.0f, 0.0f), Vector2(0.0f, 0.0f)); vertices.push_back(v9);
		Vertex v10(Vector3(0.5f, 0.5f, -0.5f), Vector3(1.0f, 0.0f, 0.0f), Vector2(1.0f, 0.0f)); vertices.push_back(v10);
		Vertex v11(Vector3(0.5f, -0.5f, -0.5f), Vector3(1.0f, 0.0f, 0.0f), Vector2(1.0f, 1.0f)); vertices.push_back(v11);
		Vertex v12(Vector3(0.5f, -0.5f, 0.5f), Vector3(1.0f, 0.0f, 0.0f), Vector2(0.0f, 1.0f)); vertices.push_back(v12);

		Vertex v13(Vector3(-0.5f, 0.5f, -0.5f), Vector3(-1.0f, 0.0f, 0.0f), Vector2(0.0f, 0.0f)); vertices.push_back(v13);
		Vertex v14(Vector3(-0.5f, 0.5f, 0.5f), Vector3(-1.0f, 0.0f, 0.0f), Vector2(1.0f, 0.0f)); vertices.push_back(v14);
		Vertex v15(Vector3(-0.5f, -0.5f, 0.5f), Vector3(-1.0f, 0.0f, 0.0f), Vector2(1.0f, 1.0f)); vertices.push_back(v15);
		Vertex v16(Vector3(-0.5f, -0.5f, -0.5f), Vector3(-1.0f, 0.0f, 0.0f), Vector2(0.0f, 1.0f)); vertices.push_back(v16);

		Vertex v17(Vector3(-0.5f, 0.5f, 0.5f), Vector3(0.0f, 0.0f, 1.0f), Vector2(0.0f, 0.0f)); vertices.push_back(v17);
		Vertex v18(Vector3(0.5f, 0.5f, 0.5f), Vector3(0.0f, 0.0f, 1.0f), Vector2(1.0f, 0.0f)); vertices.push_back(v18);
		Vertex v19(Vector3(0.5f, -0.5f, 0.5f), Vector3(0.0f, 0.0f, 1.0f), Vector2(1.0f, 1.0f)); vertices.push_back(v19);
		Vertex v20(Vector3(-0.5f, -0.5f, 0.5f), Vector3(0.0f, 0.0f, 1.0f), Vector2(0.0f, 1.0f)); vertices.push_back(v20);

		Vertex v21(Vector3(0.5f, 0.5f, -0.5f), Vector3(0.0f, 0.0f, -1.0f), Vector2(0.0f, 0.0f)); vertices.push_back(v21);
		Vertex v22(Vector3(-0.5f, 0.5f, -0.5f), Vector3(0.0f, 0.0f, -1.0f), Vector2(1.0f, 0.0f)); vertices.push_back(v22);
		Vertex v23(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.0f, 0.0f, -1.0f), Vector2(1.0f, 1.0f)); vertices.push_back(v23);
		Vertex v24(Vector3(0.5f, -0.5f, -0.5f), Vector3(0.0f, 0.0f, -1.0f), Vector2(0.0f, 1.0f)); vertices.push_back(v24);

		std::vector<uint32_t> indices = {
			0, 1, 2,
			0, 2, 3,

			4, 5, 6,
			4, 6, 7,

			8, 9, 10,
			8, 10, 11,

			12, 13, 14,
			12, 14, 15,

			16, 17, 18,
			16, 18, 19,

			20, 21, 22,
			20, 22, 23
		};
		mesh->SetMeshData(vertices, indices);
		model->AddMesh(mesh);
		model->PrepareRender(graphics.get());
		return model;
	}

	std::shared_ptr<Shader> Game::CreateShader(const std::wstring& vertexShaderFilePath, const std::wstring& pixelShaderFilePath)
	{
		auto shader = std::make_shared<Shader>();
		shader->CreateShader(graphics.get(), vertexShaderFilePath, pixelShaderFilePath);
		return shader;
	}
	std::shared_ptr<Material> Game::CreateMaterial()
	{
		auto material = std::make_shared<Material>();
		return material;
	}
}
