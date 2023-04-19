#include "Player.h"
#include <cassert>

Player::Player()
{
}

Player::~Player()
{
}


void Player::Initialize(Model* model, uint32_t textureHandle) {
	assert(model);

	this->model_ = model;
	this->textureHandle_ = textureHandle;

	worldTransform_.Initialize();
}

void Player::Update() {
	//行列を定数バッファに転送
	worldTransform_.TransferMatrix();
}

void Player::Draw() {

}