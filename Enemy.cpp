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

	worldTransform_.translation_.x = 10;

	PhaseApproachInitialize();

}

void Enemy::Update() {

	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		if (bullet->IsDead()) {
			return true;
		}
		return false;
	});
	for (auto &bullet : bullets_) {
		bullet->Update();
	}
	

	timedCalls_.remove_if([](std::unique_ptr<TimedCall>& timedCall) {
		if (timedCall->IsFinished()) {
			return true;
		}
		return false;
	});
	for (auto& timedCall : timedCalls_) {
		timedCall->Update();
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

	//弾の速度
	const Vector3 kbulletSpeed(0, 0, -0.1f);

	//速度ベクトルを自機の向きにあわせる
	Vector3 velocity = TransformNormal(kbulletSpeed, worldTransform_.matWorld_);

	//弾を生成し初期化
	//std::unique_ptr<EnemyBullet> newBullet(new EnemyBullet());
	//newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	////弾を登録する
	//bullets_.push_back(newBullet);

	std::unique_ptr<EnemyBullet> newBullet(new EnemyBullet());
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	bullets_.push_back(std::move(newBullet));

}

void Enemy::FireRiset() {
	Fire();

	std::unique_ptr<TimedCall> newTimedCall(new TimedCall(std::bind(&Enemy::FireRiset, this), 60));
	timedCalls_.push_back(std::move(newTimedCall));
}

void Enemy::PhaseApproachInitialize() {
	FireRiset();

	bulletCooldown = kFireIntervel;
}

void Enemy::PhaseApproach() {

	worldTransform_.translation_ += {0, 0, -0.2f };

	if (worldTransform_.translation_.z <= -10) {

		timedCalls_.remove_if([](std::unique_ptr<TimedCall>& timedCall) {
			if (timedCall) {
				return true;
			}
			return false;
		});
		phase_ = Phase::Leave;
	}
}

void Enemy::PhaseLeave() {
	worldTransform_.translation_ += {0, 0, 0.2f };

	if (worldTransform_.translation_.z >= 10) {
		PhaseApproachInitialize();
		phase_ = Phase::Approach;
	}
}