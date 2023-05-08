#include "EnemyBullet.h"


EnemyBullet::EnemyBullet()
{
	this->velocity_ = { 0, 0, 0 };
}

EnemyBullet::~EnemyBullet()
{
}

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	assert(model);

	this->model_ = model;
	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("red.png");

	this->velocity_ = velocity;

	worldTransform_.Initialize();

	worldTransform_.translation_ = position;

	worldTransform_.scale_.x = 0.5f;
	worldTransform_.scale_.y = 0.5f;
	worldTransform_.scale_.z = 3.0f;

	worldTransform_.rotation_.y = std::atan2(velocity_.x, velocity_.z);
	float widthLength = Length({velocity_.x, 0, velocity_.z});
	worldTransform_.rotation_.x = std::atan2(-velocity_.y, widthLength);
}

void EnemyBullet::Update() {

	worldTransform_.translation_ += velocity_;

	worldTransform_.UpdateMatrix();

	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void EnemyBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
