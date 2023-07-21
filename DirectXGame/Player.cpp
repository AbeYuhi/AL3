#include "Player.h"
#include "Vector3_Math.hpp"

Player::Player() {}
Player::~Player() {}

void Player::Initialize(Model* model) {
	worldTransform_.Initialize();
	model_ = model;

}

void Player::Update() {
	XINPUT_STATE joyState;
	if (Input::GetInstance()->GetJoystickStatePrevious(0, joyState)){
		//スピード
		const float speed = 0.3f;
		//移動量
		Vector3 move = {
			(float)joyState.Gamepad.sThumbLX, 0.0f, (float)joyState.Gamepad.sThumbLY
		};
		//移動量を速さに反映
		move = Normalize(move) * speed;

		//移動
		worldTransform_.translation_ += move;
	}


	//行列の更新
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();
}

void Player::Draw(ViewProjection viewProjection) {
	model_->Draw(worldTransform_, viewProjection);
}