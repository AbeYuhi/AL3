#include "Enemy.h"


Enemy::Enemy()
{

}

Enemy::~Enemy()
{

}


void Enemy::Initialize(Model* model, uint32_t textureHandle) {
	assert(model);

	this->model_ = model;
	this->textureHandle_ = textureHandle;

	worldTransform_.Initialize();

}

void Enemy::Update() {

	worldTransform_.translation_ += {0, 0, -0.2f };

	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(ViewProjection viewProjection) {

	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}