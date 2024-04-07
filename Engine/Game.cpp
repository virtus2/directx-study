#include "pch.h"
#include "Game.h"
#include "Input.h"
#include "Graphics.h"
#include "Window.h"
#include "Entity.h"
#include "Vertex.h"

namespace Engine
{
	Game::Game()
	{
        input = std::make_unique<Input>();
        graphics = std::make_unique<Graphics>();
        window = std::make_unique<Window>();
		display = std::make_unique<Display>();
		modelLoader = std::make_unique<ModelLoader>();
	}

	Game::~Game()
	{
		
	}

    void Game::Run(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow, int width, int height)
    {
		this->width = width;
		this->height = height;

		// 윈도우 초기화
		window->Initialize(hInstance, nCmdShow, L"WindowClass", L"Window", width, height);
		
		// 인풋 초기화
		HWND hWnd = window->GetWindowHandle();
		input->Initialize(hWnd);

		// DirectX 11, 그래픽 관련 초기화
		graphics->Initialize(display.get(), hWnd, width, height);

		// 디스플레이 초기화
		display->Initialize(graphics.get(), hWnd, width, height);

		// 모델 로더 초기화
		modelLoader->Initialize(graphics.get());

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
			Update();
			Render();
		}
    }

	void Game::Update()
	{
		input->Update();

		// TODO: Delta Time 계산해서 넘겨줘야함
		OnUpdate();
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
		// 테스트 코드
		for (const auto& entity : entities)
		{
			auto model = entity->GetModel();
			graphics->SetShader(model->GetShader().get());
			if (model)
			{
				graphics->DrawMesh(model->GetMesh(0).get());
			}
		}
		// graphics->Render();

		display->GetSwapChain()->Present(1, 0);
	}

	std::shared_ptr<Entity> Game::CreateEntity()
	{
		// TODO: Entity를 생성하고 관련 데이터를 세팅한다.
		auto entity = std::make_shared<Entity>();
		entities.push_back(entity);
		return entity;
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

	std::shared_ptr<Shader> Game::CreateShader(const std::wstring& vertexShaderFilePath, const std::wstring& pixelShaderFilePath)
	{
		auto shader = std::make_shared<Shader>();
		shader->CreateShader(graphics.get(), vertexShaderFilePath, pixelShaderFilePath);
		return shader;
	}
}
