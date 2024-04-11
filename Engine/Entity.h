#pragma once
#include "Matrix.h"

class Model;

class Entity
{
public:
	// TODO: 아직 구조 어떻게 짤 지 안정했음. 추후 수정 필요.
	Entity() {}
	Entity(std::shared_ptr<Model> model) : model(model) {}
	~Entity() {}

	virtual void Update(float deltaTime) {}
	virtual void Render() {}

	// 테스트 코드
	void SetModel(std::shared_ptr<Model> model) { this->model = model; }
	std::shared_ptr<Model> GetModel() { return model; }

	void SetPosition(Math::Vector3 position) { this->position = position; }
	void SetRotation(Math::Vector3 rotation) { this->rotation = rotation; }
	void SetScale(Math::Vector3 scale) { this->scale = scale; }

	Math::Vector3 GetPosition() { return position; }
	Math::Vector3 GetRotation() { return rotation; }
	Math::Vector3 GetScale() { return scale; }

protected:
	Math::Vector3 position = Math::Vector3::Zero();
	Math::Vector3 rotation = Math::Vector3::Zero();
	Math::Vector3 scale = Math::Vector3::One();

	std::shared_ptr<Model> model;

};