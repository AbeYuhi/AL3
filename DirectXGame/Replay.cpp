#include "Replay.h"
#include "GameScene.h"
#include "GlobalVariables.h"

Replay* Replay::GetInstance() {
	static Replay instance;
	return &instance;
}

void Replay::Initialize(GameScene* gameScene) {
	gameScene_ = gameScene;

	//リプレイ中か
	gameStatus_ = Replay::GameStatus::kNone;
	isReplay_ = false;
}

void Replay::InitializeSave() {
	isReplay_ = false;
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	globalVariables->DeletionFile(groupName);
	globalVariables->CreateGroup(groupName);
	globalVariables->SetValue(groupName, "startReplayFrame", 1);
}

void Replay::InitializeReplay() {
	isReplay_ = true;
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	startFrame_ = globalVariables->GetIntValue(groupName, "startReplayFrame");
	endFrame_ = globalVariables->GetIntValue(groupName, "endReplayFrame");
	targetFrame_ = startFrame_;
	ReplayPreparation();
}

void Replay::SaveUpdate() {
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	std::string str;
	if (Input::GetInstance()->GetJoystickState(0, joyState_)) {
		str = "joyState";
		str += std::to_string(gameScene_->GetFrame());
		globalVariables->SetValue(groupName, str, joyState_);
	}
	else {
		joyState_ = { 0 };
		str = "joyState";
		str += std::to_string(gameScene_->GetFrame());
		globalVariables->SetValue(groupName, str, joyState_);
	}

	ImGui::Begin("SaveReplay");
	//リプレイを始めたいFrameの値
	if (ImGui::Button("StartReplayFrame")) {
		globalVariables->SetValue(groupName, "startReplayFrame", gameScene_->GetFrame());
	}
	//リプレイを止めたいFrameの値
	if (ImGui::Button("EndReplayFrame")) {
		globalVariables->SetValue(groupName, "endReplayFrame", gameScene_->GetFrame());
	}
	ImGui::End();
}

void Replay::ReplayUpdate() {
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();

	if (preFrame_ + 1 != gameScene_->GetFrame()) {
		targetFrame_ = gameScene_->GetFrame();
		gameScene_->Initialize();
		ReplayPreparation();
	}

	SettingFrame();

	if(!globalVariables->GetXINPUT_STATEValue("Replay", "joyState" + std::to_string(gameScene_->GetFrame()), &joyState_)){
		isReplay_ = false;
		gameScene_->Initialize();
		gameStatus_ = GameStatus::kNone;
	}

	preFrame_ = gameScene_->GetFrame();
}

void Replay::ReplayPreparation() {
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	while (gameScene_->GetFrame() < targetFrame_) {
		globalVariables->GetXINPUT_STATEValue("Replay", "joyState" + std::to_string(gameScene_->GetFrame()), &joyState_);
		gameScene_->ReplayUpdate();
	}

	preFrame_ = gameScene_->GetFrame() - 1;
}

void Replay::SettingFrame() {
	//0
	if (Input::GetInstance()->PushKey(DIK_0)) {
		gameScene_->Initialize();
		targetFrame_ = 1;
		ReplayPreparation();
	}
	//1
	if (Input::GetInstance()->PushKey(DIK_1)) {
		gameScene_->Initialize();
		targetFrame_ = endFrame_ / 10;
		ReplayPreparation();
	}

	if (Input::GetInstance()->PushKey(DIK_2)) {
		gameScene_->Initialize();
		targetFrame_ = endFrame_ / 10 * 2;
		ReplayPreparation();
	}

	if (Input::GetInstance()->PushKey(DIK_3)) {
		gameScene_->Initialize();
		targetFrame_ = endFrame_ / 10 * 3;
		ReplayPreparation();
	}

	if (Input::GetInstance()->PushKey(DIK_4)) {
		gameScene_->Initialize();
		targetFrame_ = endFrame_ / 10 * 4;
		ReplayPreparation();
	}

	if (Input::GetInstance()->PushKey(DIK_5)) {
		gameScene_->Initialize();
		targetFrame_ = endFrame_ / 10 * 5;
		ReplayPreparation();
	}

	if (Input::GetInstance()->PushKey(DIK_6)) {
		gameScene_->Initialize();
		targetFrame_ = endFrame_ / 10 * 6;
		ReplayPreparation();
	}

	if (Input::GetInstance()->PushKey(DIK_7)) {
		gameScene_->Initialize();
		targetFrame_ = endFrame_ / 10 * 7;
		ReplayPreparation();
	}

	if (Input::GetInstance()->PushKey(DIK_8)) {
		gameScene_->Initialize();
		targetFrame_ = endFrame_ / 10 * 8;
		ReplayPreparation();
	}

	if (Input::GetInstance()->PushKey(DIK_9)) {
		gameScene_->Initialize();
		targetFrame_ = endFrame_ / 10 * 9;
		ReplayPreparation();
	}

	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		gameScene_->Initialize();
		targetFrame_ = startFrame_;
		ReplayPreparation();
	}
}

void Replay::EndInGame() {
	if (!isReplay_) {
		GlobalVariables::GetInstance()->AddValue(groupName, "endReplayFrame", gameScene_->GetFrame());
		GlobalVariables::GetInstance()->SaveFile(groupName);
	}
}