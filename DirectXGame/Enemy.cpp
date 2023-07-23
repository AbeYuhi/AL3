#include "Enemy.h"
#include "Vector3_Math.hpp"

Enemy::Enemy(){}
Enemy::~Enemy(){}

void Enemy::Initialize(const std::vector<Model*>& models) {
	Entity::Initialize(models);

	worldTransform_.translation_.z = 15;
}

void Enemy::Update() {
	Vector3 speed = { 0.0f, 0.0f, -0.2f };
	worldTransform_.rotation_.y += 0.02f;
	Matrix4x4 rotateMatrix = MakeRotateMatrix(worldTransform_.rotation_);

	speed = TransformNormal(speed, rotateMatrix);

	worldTransform_.translation_ += speed;

	Entity::Update();
}

void Enemy::Draw(ViewProjection viewProjection) {
	models_[EnemyModels::Body]->Draw(worldTransform_, viewProjection);
}