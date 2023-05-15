#include "Enemy.h"
#include "Player.h"

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

	worldTransform_.translation_.z = 20;

	PhaseApproachInitialize();

}

void Enemy::Update() {

	bullets_.remove_if([](std::unique_ptr<EnemyBullet> &bullet) {
		if (bullet->IsDead()) {
			return true;
		}
		return false;
	});
	for (auto& bullet : bullets_) {
		if (bullet) {
			bullet->Update();
		}
	}

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

	for (auto& bullet : bullets_) {
		if (bullet) {
			bullet->Draw(viewProjection);
		}
	}
}

void Enemy::Fire() {
	assert(player_);

	std::unique_ptr<EnemyBullet> newBullet(new EnemyBullet());
	newBullet->Initialize(model_, GetEnemyPosition(), {0, 0, -1});
	newBullet->SetPlayer(player_);

	bullets_.push_back(std::move(newBullet));
}

void Enemy::PhaseApproachInitialize() {
	bulletCooldown = kFireIntervel;
}

void Enemy::PhaseApproach() {
	if (--bulletCooldown <= 0) {
		bulletCooldown = kFireIntervel;
		Fire();
	}

	//worldTransform_.translation_ += {0, 0, -0.2f };
}

void Enemy::PhaseLeave() {
	worldTransform_.translation_ += {0, 0, 0.2f };

	if (worldTransform_.translation_.z >= 10) {
		PhaseApproachInitialize();
		phase_ = Phase::Approach;
	}
}