#include "PlayerBullet.h"


PlayerBullet::PlayerBullet()
{
}

PlayerBullet::~PlayerBullet()
{

}

void PlayerBullet::Initialize(Model* model, const Vector3& position) {
	assert(model);

	this->model_ = model;
	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("black.png");

	worldTransform_.Initialize();

	worldTransform_.translation_ = position;
}

void PlayerBullet::Update() {

	worldTransform_.UpdateMatrix();
}

void PlayerBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}