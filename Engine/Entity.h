#pragma once

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

protected:
	std::shared_ptr<Model> model;

};