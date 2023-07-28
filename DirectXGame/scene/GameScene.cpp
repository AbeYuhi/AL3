#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();

	viewProjection_.Initialize();

	skydomeModel_.reset(Model::CreateFromOBJ("Skydome", true));
	groundModel_.reset(Model::CreateFromOBJ("Ground", true));
	playerHeadModel_.reset(Model::CreateFromOBJ("PHead", true));
	playerBodyModel_.reset(Model::CreateFromOBJ("PBody", true));
	playerLeftArmModel_.reset(Model::CreateFromOBJ("PLeftArm", true));
	playerRightArmModel_.reset(Model::CreateFromOBJ("PRightArm", true));
	playerWeaponModel_.reset(Model::CreateFromOBJ("Hummer", true));
	enemyBodyModel_.reset(Model::CreateFromOBJ("Enemy", true));

	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(skydomeModel_.get());

	ground_ = std::make_unique<Ground>();
	ground_->Initialize(groundModel_.get());

	player_ = std::make_unique<Player>();
	std::vector<Model*> playerModels = {
		playerHeadModel_.get(), playerBodyModel_.get(), playerLeftArmModel_.get(), playerRightArmModel_.get(), playerWeaponModel_.get()
	};
	player_->Initialize(playerModels);

	enemy_ = std::make_unique<Enemy>();
	std::vector<Model*> enemyModels = {
		enemyBodyModel_.get()
	};
	enemy_->Initialize(enemyModels);

	//セッター
	followCamera_->SetTarget(&player_->GetWorldTransform());
	player_->SetViewProjection(&followCamera_->GetViewProjection());
	player_->SetFrame(&GlobalVariables::frame_);
}

void GameScene::Update() {

	followCamera_->Update();
	viewProjection_.matView = followCamera_->GetViewProjection().matView;
	viewProjection_.matProjection = followCamera_->GetViewProjection().matProjection;
	viewProjection_.TransferMatrix();

	skydome_->Update();	
	ground_->Update();	
	player_->Update();	
	enemy_->Update();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	ground_->Draw(viewProjection_);
	skydome_->Draw(viewProjection_);
	enemy_->Draw(viewProjection_);
	player_->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
