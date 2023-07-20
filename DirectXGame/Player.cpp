#include "Player.h"

Player::Player() {}
Player::~Player() {}

void Player::Initialize(Model* model) {
	worldTransform_.Initialize();
	model_ = model;

}

void Player::Update() {

}

void Player::Draw(ViewProjection viewProjection) {
	model_->Draw(worldTransform_, viewProjection);
}