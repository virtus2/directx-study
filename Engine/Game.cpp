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

	float degree = 1.0f;
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
		float xScale = 1.42814801f;
		float yScale = 1.42814801f;
		if (width > height)
		{
			xScale = yScale *
				static_cast<float>(height) /
				static_cast<float>(width);
		}
		else
		{
			yScale = xScale *
				static_cast<float>(width) /
				static_cast<float>(height);
		}

		// TODO: 카메라 클래스 만들어서 거기서 값 읽어오기
		graphics->constantBufferData.world = Math::Matrix::RotationY(-degree);
		degree += (float)1/60;
		graphics->constantBufferData.projection = Math::Matrix(
			xScale, 0.0f, 0.0f, 0.0f,
			0.0f, yScale, 0.0f, 0.0f,
			0.0f, 0.0f, -1.0f, -0.01f,
			0.0f, 0.0f, -1.0f, 0.0f
		);
		graphics->constantBufferData.view = Math::Matrix(
			-1.00000000f, 0.00000000f, 0.00000000f, 0.00000000f,
			0.00000000f, 0.89442718f, 0.44721359f, 0.00000000f,
			0.00000000f, 0.44721359f, -0.89442718f, -2.23606800f,
			0.00000000f, 0.00000000f, 0.00000000f, 1.00000000f
		);

		for (const auto& entity : entities)
		{
			auto model = entity->GetModel();
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
