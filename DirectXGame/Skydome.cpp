#include "Skydome.h"

Skydome::Skydome() {}

Skydome::~Skydome() {}

void Skydome::Initialize(Model* model) {
	assert(model);
	this->model_ = model;

	worldTransform_.Initialize();
}

void Skydome::Update() {

}

void Skydome::Draw(ViewProjection viewProjection) { 
	model_->Draw(worldTransform_, viewProjection); 
}