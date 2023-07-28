#include "Player.h"
#include "Vector3_Math.hpp"

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

	GlobalVariables* globalVaruables = GlobalVariables::GetInstance();
	const char* groupName = "Player";
	//グループを追加
	globalVaruables->CreateGroup(groupName);
	globalVaruables->AddValue(groupName, "Head Translation", worldTransformHead_.translation_);
	globalVaruables->AddValue(groupName, "ArmL Translation", worldTransformLeftArm_.translation_);
	globalVaruables->AddValue(groupName, "ArmR Translation", worldTransformRightArm_.translation_);
	globalVaruables->AddValue(groupName, "floatingCycle", cycle_);
	globalVaruables->AddValue(groupName, "swingWidth", swingWidth_);
	globalVaruables->AddValue(groupName, "attackCycle", attackCycle_);
}

void Player::ApplyGlobalVariables() {
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Player";
	worldTransformHead_.translation_ = globalVariables->GetVector3Value(groupName, "Head Translation");
	worldTransformLeftArm_.translation_ = globalVariables->GetVector3Value(groupName, "ArmL Translation");
	worldTransformRightArm_.translation_ = globalVariables->GetVector3Value(groupName, "ArmR Translation");
	cycle_ = globalVariables->GetIntValue(groupName, "floatingCycle");
	swingWidth_ = globalVariables->GetFloatValue(groupName, "swingWidth");
	attackCycle_ = globalVariables->GetIntValue(groupName, "attackCycle");
}

void Player::Update() {

	ApplyGlobalVariables();
	
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

	//行列の更新
	worldTransform_.UpdateMatrix();
	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformLeftArm_.UpdateMatrix();
	worldTransformRightArm_.UpdateMatrix();
	worldTransformWeapon_.UpdateMatrix();

	isSetJoyState_ = false;
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
	GlobalVariables* globalVaruables = GlobalVariables::GetInstance();
	if (!GlobalVariables::isReplay_) {
		isSetJoyState_ = Input::GetInstance()->GetJoystickState(0, GlobalVariables::joyState_);
	}
	else {
		GlobalVariables::joyState_ = globalVaruables->GetXINPUT_STATEValue("Replay", "joyState" + std::to_string(GlobalVariables::frame_));
		isSetJoyState_ = true;
	}

	if (isSetJoyState_) {
		//攻撃入力
		if (GlobalVariables::joyState_.Gamepad.bRightTrigger) {
			behaviorRequest_ = Player::Behavior::kAttack;
		}

		//スピード
		const float speed = 0.3f;
		//移動量
		Vector3 move = {
			(float)GlobalVariables::joyState_.Gamepad.sThumbLX, 0.0f, (float)GlobalVariables::joyState_.Gamepad.sThumbLY
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