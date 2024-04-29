﻿#include "MyGame.h"

// TODO: 엔진 관련 include를 여기에 놓는게 맞을까
#include "Entity.h"
#include "ModelLoader.h"
#include "Camera.h"

#define PALADIN 0
#define ZELDA 1
#define RECTANGLE 0
#define CUBE 0
#define HELMET 0

void MyGame::BeginRun()
{
	entity = CreateEntity();
#if HELMET==1
	// 헬멧 테스트 코드
	std::string modelPath = "Models/DamagedHelmet.glb";
	model = CreateModel(modelPath);
	entity->SetModel(model);
#endif 

#if PALADIN==1
	// 팔라딘 테스트 코드
	std::string modelPath = "Models/Paladin.fbx";
	model = CreateModel(modelPath);
	entity->SetModel(model);
	entity->SetScale(Vector3(0.1f, 0.1f, 0.1f));
#endif 
#if ZELDA==1
	// 젤다 테스트 코드
	std::string modelPath = "Models/zeldaPosed001/zeldaPosed001.fbx";
	model = CreateModel(modelPath);
	entity->SetModel(model);
	entity->SetScale(XMFLOAT3(0.01f, 0.01f, 0.01f));
#endif
 
#if RECTANGLE==1
	//네모 만들기 테스트 코드 
	model = CreateRectangle();
	entity->SetModel(model);
#endif

#if CUBE==1
	//네모 만들기 테스트 코드 
	model = CreateCube();
	entity->SetModel(model);
#endif

	const std::wstring vertexShaderPath = L"Shaders/SimpleVertexShader.hlsl";
	const std::wstring pixelShaderPath = L"Shaders/SimplePixelShader.hlsl";
	shader = CreateShader(vertexShaderPath, pixelShaderPath);
	auto meshes = model->GetMeshes();
	for (auto& mesh : meshes)
	{
		mesh->GetMaterial()->SetShader(shader);
	}

	camera = CreateCamera();
}

void MyGame::OnUpdate(float deltaTime)
{
	auto input = GetInput();

	// 1인칭 카메라 이동 테스트
	float moveSpeed = 0.01f * deltaTime;
	float mouseX = input->GetAnalogInput(Input::Mouse_X);
	float mouseY = input->GetAnalogInput(Input::Mouse_Y);

	Vector3 camPos = camera->GetPosition();
	Vector3 camTarget = camera->GetTargetPosition();
	Vector3 camUp = camera->GetUpVector();
	camUp.Normalize();

	Vector3 viewDir = camTarget - camPos;
	viewDir.Normalize();
	Vector3 rightDir = viewDir.Cross(camUp);

	Vector3 movement(0.0f, 0.0f, 0.0f);
	if (input->IsPressed(Input::DigitalInput::Key_W))
	{
		movement.z += 1.0f;
	}
	if (input->IsPressed(Input::DigitalInput::Key_S))
	{
		movement.z -= 1.0f;
	}
	if (input->IsPressed(Input::DigitalInput::Key_A))
	{
		movement.x += 1.0f;
	}
	if (input->IsPressed(Input::DigitalInput::Key_D))
	{
		movement.x -= 1.0f;
	}

	if (input->IsPressed(Input::DigitalInput::Mouse_1)) 
	{
		Vector3 xAxis(1.0f, 0.0f, 0.0f);
		Vector3 yAxis(0.0f, 1.0f, 0.0f);

		float sensitivity = 10.0f;
		float yaw = mouseX * DirectX::XM_2PI;
		float pitch = mouseY * DirectX::XM_2PI;
		// Rotate around X axis
		Matrix rotY = Matrix::CreateFromAxisAngle(rightDir, sensitivity * XMConvertToRadians(pitch));
		viewDir = Vector3::TransformNormal(viewDir, rotY);
		camUp = Vector3::TransformNormal(camUp, rotY);

		// Rotate around Y axis
		Matrix rotX = Matrix::CreateRotationY(sensitivity * XMConvertToRadians(yaw));
		viewDir = Vector3::Transform(viewDir, rotX);
		camUp = Vector3::TransformNormal(camUp, rotX);
	}

	camTarget = camPos + viewDir;
	camera->SetTargetPosition(camTarget);
	camera->SetUpVector(camUp);

	camPos += movement.z * moveSpeed * viewDir;
	camPos += movement.x * moveSpeed * rightDir;
	camTarget = camPos + viewDir;

	camera->SetPosition(camPos);
	camera->SetTargetPosition(camTarget);
}
