#include "Player.h"
#include "Vector3_Math.hpp"

Player::Player() {}
Player::~Player() {}

void Player::Initialize(const std::vector<Model*>& models) {
	Entity::Initialize(models);

	worldTransformHead_.Initialize();
	worldTransformBody_.Initialize();
	worldTransformLeftArm_.Initialize();
	worldTransformRightArm_.Initialize();
	worldTransformWeapon_.Initialize();
	//ペアレント
	worldTransformHead_.parent_ = &worldTransformBody_;
	worldTransformBody_.parent_ = &worldTransform_;
	worldTransformLeftArm_.parent_ = &worldTransformBody_;
	worldTransformRightArm_.parent_ = &worldTransformBody_;
	worldTransformWeapon_.parent_ = &worldTransformBody_;

	InitializeFloatingGimmick();
	InitializeAttackGimmick();
}

void Player::Update() {
	
	if (behaviorRequest_) {
		behavior_ = behaviorRequest_.value();
		switch (behavior_)
		{
		case Player::Behavior::kRoot:
		default:
			BehaviorRootInitialize();
			break;
		case Player::Behavior::kAttack:
			BehaviorAttackInitialize();
			break;
		}
		behaviorRequest_ = std::nullopt;
	}

	switch (behavior_)
	{
	case Player::Behavior::kRoot:
	default:
		BehaviorRootUpdata();
		break;
	case Player::Behavior::kAttack:
		BehaviorAttackUpdata();
		break;
	}

	ImGui::Begin("PlayerWindow");
	ImGui::SliderFloat3("Head", &worldTransformHead_.translation_.x, -5, 5);
	ImGui::SliderFloat3("LeftArm", &worldTransformLeftArm_.translation_.x, -5, 5);
	ImGui::SliderFloat3("RightArm", &worldTransformRightArm_.translation_.x, -5, 5);
	ImGui::SliderInt("SwingCycle", &cycle_, 1, 120);
	ImGui::SliderFloat("SwingWidth", &swingWidth_, 0.0f, 10.0f);
	ImGui::SliderInt("AttackCycle", &attackCycle_, 1, 120);
	ImGui::End();

	//行列の更新
	worldTransform_.UpdateMatrix();
	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformLeftArm_.UpdateMatrix();
	worldTransformRightArm_.UpdateMatrix();
	worldTransformWeapon_.UpdateMatrix();
}

void Player::Draw(ViewProjection viewProjection) {
	this->models_[PlayerModelIndex::kModelIndexHead]->Draw(worldTransformHead_, viewProjection);
	this->models_[PlayerModelIndex::kModelIndexBody]->Draw(worldTransformBody_, viewProjection);
	this->models_[PlayerModelIndex::kModelIndexLArm]->Draw(worldTransformLeftArm_, viewProjection);
	this->models_[PlayerModelIndex::kModelIndexRArm]->Draw(worldTransformRightArm_, viewProjection);
	this->models_[PlayerModelIndex::kModelIndexWeapon]->Draw(worldTransformWeapon_, viewProjection);
}

void Player::InitializeFloatingGimmick() {
	floatingParameter_ = 0.0f;
	cycle_ = 60;

	//浮遊の振動の幅
	swingWidth_ = 1.5f;
}

void Player::UpdateFloatingGimmick() {
	const float step = 2.0f * M_PI / cycle_;

	floatingParameter_ += step;
	floatingParameter_ = std::fmod(floatingParameter_, 2.0f * M_PI);

	worldTransformBody_.translation_.y = std::sin(floatingParameter_) * swingWidth_;
	worldTransformBody_.translation_.y += 1;
}

void Player::BehaviorRootUpdata() {
	XINPUT_STATE joyState;
	if (Input::GetInstance()->GetJoystickStatePrevious(0, joyState)) {
		//攻撃入力
		if (joyState.Gamepad.bRightTrigger) {
			behaviorRequest_ = Player::Behavior::kAttack;
		}

		//スピード
		const float speed = 0.3f;
		//移動量
		Vector3 move = {
			(float)joyState.Gamepad.sThumbLX, 0.0f, (float)joyState.Gamepad.sThumbLY
		};
		//移動量を速さに反映
		move = Normalize(move) * speed;

		Matrix4x4 rotateXMatrix = MakeRotateXMatrix(0.0f);
		Matrix4x4 rotateYMatrix = MakeRotateYMatrix(viewProjection_->rotation_.y);
		Matrix4x4 rotateZMatrix = MakeRotateZMatrix(0.0f);

		Matrix4x4 rotateMatrix = MakeRotateXYZMatrix(rotateXMatrix, rotateYMatrix, rotateZMatrix);

		move = TransformNormal(move, rotateMatrix);

		//回転
		worldTransform_.rotation_.y = atan2f(move.x, move.z);

		//移動
		worldTransform_.translation_ += move;
	}

	UpdateFloatingGimmick();
}

void Player::BehaviorAttackUpdata() {
	
	UpdateAttackGimmick();
}

void Player::InitializeAttackGimmick() {
	attackParameter_ = 0.0f;
	attackCycle_ = 30;
}

void Player::UpdateAttackGimmick() {
	const float step = 0.75f * M_PI / attackCycle_;

	attackParameter_ += step;

	if (attackParameter_ > 0.75f * M_PI) {
		behaviorRequest_ = Player::Behavior::kRoot;
	}

	attackParameter_ = std::fmod(attackParameter_, 0.75f * M_PI);

	worldTransformLeftArm_.rotation_.x = M_PI * 0.75f;
	worldTransformRightArm_.rotation_.x = M_PI * 0.75f;
	worldTransformWeapon_.rotation_.x = -M_PI * 0.25f;

	worldTransformLeftArm_.rotation_.x += fabs(attackParameter_);
	worldTransformRightArm_.rotation_.x += fabs(attackParameter_);
	worldTransformWeapon_.rotation_.x += fabs(attackParameter_);
}

void Player::BehaviorRootInitialize() {
	worldTransformLeftArm_.rotation_.x = 0.0f;
	worldTransformRightArm_.rotation_.x = 0.0f;
	worldTransformWeapon_.rotation_.x = 0.0f;

	worldTransformHead_.translation_.y = 4.0f;
	worldTransformLeftArm_.translation_.y = 3.5f;
	worldTransformLeftArm_.translation_.x = -1.5f;
	worldTransformRightArm_.translation_.y = 3.5f;
	worldTransformRightArm_.translation_.x = 1.5f;

	InitializeFloatingGimmick();
}

void Player::BehaviorAttackInitialize() {
	InitializeAttackGimmick();
}