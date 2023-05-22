#include "PlayerBullet.h"


PlayerBullet::PlayerBullet()
{
}

PlayerBullet::~PlayerBullet()
{

}

void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	assert(model);

	this->model_ = model;
	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("black.png");

	worldTransform_.Initialize();

	worldTransform_.translation_ = position;

	this->velocity_ = velocity;

	//衝突属性を設定
	SetCollisionAttribute(kCollisionAttributePlayer);
	SetCollisionMask(~kCollisionAttributePlayer);
}

void PlayerBullet::Update() {

	worldTransform_.translation_ += velocity_;

	worldTransform_.UpdateMatrix();

	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void PlayerBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}


void PlayerBullet::OnCollision() {
	isDead_ = true;
}