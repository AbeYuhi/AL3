#include "FollowCamera.h"
#include "Vector3_Math.hpp"
#include "Replay.h"

FollowCamera::FollowCamera() {}
FollowCamera::~FollowCamera() {}

void FollowCamera::Initialize() {
	viewProjection_.Initialize();
	viewProjection_.translation_.y = 5.0f;

	//行列の更新
	viewProjection_.matView = Inverse(MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, viewProjection_.rotation_, viewProjection_.translation_));
	viewProjection_.TransferMatrix();
}

void FollowCamera::InGameUpdate() {
	XINPUT_STATE joyState;
	joyState = Replay::GetInstance()->GetJoyState();
	const float kRotateSpeed = 0.05f;

	viewProjection_.rotation_.y += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * kRotateSpeed;
	viewProjection_.rotation_.x += (float)joyState.Gamepad.sThumbRY / SHRT_MAX * kRotateSpeed;

	if (target_) {
		Vector3 offset = {0.0f, 5.0f, -40.0f};

		Matrix4x4 rotateMatrix = MakeRotateMatrix(viewProjection_.rotation_);

		offset = TransformNormal(offset, rotateMatrix);
		
		//座標をコピーしてオフセット分ずらす
		viewProjection_.translation_ = target_->translation_ + offset;
	}

	//行列の更新
	viewProjection_.matView = Inverse(MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, viewProjection_.rotation_, viewProjection_.translation_));
	viewProjection_.TransferMatrix();
}

void FollowCamera::ReplayUpdate() {
	XINPUT_STATE joyState;

	if (Input::GetInstance()->GetJoystickStatePrevious(0, joyState)) {
		const float kRotateSpeed = 0.05f;

		viewProjection_.rotation_.y += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * kRotateSpeed;
		viewProjection_.rotation_.x += (float)joyState.Gamepad.sThumbRY / SHRT_MAX * kRotateSpeed;

	}

	if (target_) {
		Vector3 offset = { 0.0f, 5.0f, -40.0f };

		Matrix4x4 rotateMatrix = MakeRotateMatrix(viewProjection_.rotation_);

		offset = TransformNormal(offset, rotateMatrix);

		//座標をコピーしてオフセット分ずらす
		viewProjection_.translation_ = target_->translation_ + offset;
	}

	//行列の更新
	viewProjection_.matView = Inverse(MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, viewProjection_.rotation_, viewProjection_.translation_));
	viewProjection_.TransferMatrix();
}