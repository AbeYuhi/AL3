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

	Fire();

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

void Enemy::Fire() {

	//弾の速度
	const Vector3 kbulletSpeed(0, 0, 1.0f);

	//速度ベクトルを自機の向きにあわせる
	Vector3 velocity = TransformNormal(kbulletSpeed, worldTransform_.matWorld_);

	//弾を生成し初期化
	//std::unique_ptr<EnemyBullet> newBullet(new EnemyBullet());
	//newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	////弾を登録する
	//bullets_.push_back(newBullet);

	bullets_->Initialize(model_, worldTransform_.translation_, velocity);

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