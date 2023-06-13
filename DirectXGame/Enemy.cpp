#include "Enemy.h"
#include "Player.h"
#include "GameScene.h"

Enemy::Enemy()
{

}

Enemy::~Enemy()
{
}


void Enemy::Initialize(Model* model, uint32_t textureHandle, Vector3 pos) {
	assert(model);

	this->model_ = model;
	this->textureHandle_ = textureHandle;

	worldTransform_.Initialize();

	worldTransform_.translation_ = pos;

	PhaseApproachInitialize();
	
	isDead_ = false;
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
	assert(player_);

	//弾の速度
	const float kbulletSpeed = 1.0f;

	Vector3 playerPos = player_->GetWorldPosition();
	Vector3 enemyPos = GetWorldPosition();
	
	Vector3 differencialVector = playerPos - enemyPos;

	Vector3 normalizeVector = Normalize(differencialVector);

	Vector3 velocity = normalizeVector * kbulletSpeed;

	EnemyBullet* newBullet(new EnemyBullet());
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	gameScene_->AddEnemyBullet(newBullet);
}

void Enemy::PhaseApproachInitialize() {
	bulletCooldown = kFireIntervel;
}

void Enemy::PhaseApproach() {
	if (--bulletCooldown <= 0) {
		bulletCooldown = kFireIntervel;
		Fire();
	}

	worldTransform_.translation_ += {0, 0, -0.2f };

	if (worldTransform_.translation_.z <= -20) {
		phase_ = Phase::Leave;
	}
}

void Enemy::PhaseLeave() {
	worldTransform_.translation_ += {0, 0, 0.2f };

	if (worldTransform_.translation_.z >= 20) {
		PhaseApproachInitialize();
		phase_ = Phase::Approach;
	}
}

void Enemy::OnCollision() {
	isDead_ = true;
}