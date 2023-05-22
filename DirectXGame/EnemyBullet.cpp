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

	worldTransform_.Initialize();

	worldTransform_.translation_ = position;

	this->velocity_ = velocity;

	//衝突属性を設定
	SetCollisionAttribute(kCollisionAttributeEnemy);
	SetCollisionMask(~kCollisionAttributeEnemy);
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

void EnemyBullet::OnCollision() {
	isDead_ = true;
}