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

	switch (phase_)
	{
	case Enemy::Phase::Approach:
		PhaseApproach();
		break;
	case Enemy::Phase::Leave:
		PhaseLeave();
		break;
	default:
		break;
	}
	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(ViewProjection viewProjection) {

	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void Enemy::PhaseApproach() {
	worldTransform_.translation_ += {0, 0, -0.2f };

	if (worldTransform_.translation_.z <= -10) {
		phase_ = Phase::Leave;
	}
}

void Enemy::PhaseLeave() {
	worldTransform_.translation_ += {0, 0, 0.2f };

	if (worldTransform_.translation_.z >= 10) {
		phase_ = Phase::Approach;
	}
}