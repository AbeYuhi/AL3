#include "Enemy.h"


void (Enemy::* Enemy::spPhaseTable[])() = {
	&Enemy::PhaseApproach,
	&Enemy::PhaseLeave
};

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

	//現在のフェーズ
	inPhase = 0;

}

void Enemy::Update() {

	(this->*spPhaseTable[inPhase])();

	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(ViewProjection viewProjection) {

	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void Enemy::PhaseApproach() {
	worldTransform_.translation_ += {0, 0, -0.2f };

	if (worldTransform_.translation_.z <= -10) {
		inPhase = 1;
	}
}

void Enemy::PhaseLeave() {
	worldTransform_.translation_ += {0, 0, 0.2f };

	if (worldTransform_.translation_.z >= 10) {
		inPhase = 0;
	}
}