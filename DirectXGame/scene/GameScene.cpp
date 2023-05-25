#include "GameScene.h"
#include "TextureManager.h"
#include "AxisIndicator.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete player_;
	delete enemy_;
	delete debugCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	textureHandle = TextureManager::Load("Player.png");
	enemyTexture = TextureManager::Load("Slime.png");
	model_ = Model::Create();

	player_ = new Player();
	player_->Initialize(model_, textureHandle);

	enemy_ = new Enemy();
	enemy_->Initialize(model_, enemyTexture);

	player_->SetEnemy(enemy_);
	enemy_->SetPlayer(player_);

	//ビュープロジェクションの初期化
	viewProjection_.farZ = 1000;
	viewProjection_.Initialize();

	//デバックカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());

	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_);
}

void GameScene::Update() {
	//デバッグカメラの更新
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_0)) {
		if (isDebugCameraActive_) {
			isDebugCameraActive_ = false;
		}
		else {
			isDebugCameraActive_ = true;
		}
	}
#endif // _DEBUG
	if (isDebugCameraActive_) {
		debugCamera_->Update();

		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		//ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	}
	else {
		//ビュープロジェクション行列の更新と転送
		viewProjection_.UpdateMatrix();
	}

	ImGui::Begin("DebugCamera");
	ImGui::Text("True(1) or False(0), %d", isDebugCameraActive_);
	ImGui::Text("%f", viewProjection_.farZ);
	ImGui::End();

	player_->Update();

	if (enemy_) {
		enemy_->Update();
	}

	CheckAllCollisions();

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

	skydome_->Draw(viewProjection_);

	player_->Draw(viewProjection_);

	if (enemy_) {
		enemy_->Draw(viewProjection_);
	}

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

void GameScene::CheckAllCollisions() {
	Vector3 posA, posB;

	//自弾リストの取得
	std::list<PlayerBullet*> playerBullets = player_->GetBullets();
	//敵弾リストの取得
	std::list<EnemyBullet*> enemyBullets = enemy_->GetBullets();

#pragma region 自キャラと敵弾の当たり判定
	posA = player_->GetPlayerPosition();

	for (auto bullet : enemyBullets) {
		posB = bullet->GetPos();

		float length = Length(posA, posB);

		if (length <= player_->kSize + enemy_->kSize) {
			//自キャラの衝突時コールバックを呼び出す
			player_->OnCollision();
			//敵弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}

	}

#pragma endregion

#pragma region 敵キャラと自弾の当たり判定
	posA = enemy_->GetEnemyPosition();

	for (auto bullet : playerBullets) {
		posB = bullet->GetPos();

		float length = Length(posA, posB);

		if (length <= player_->kSize + enemy_->kSize) {
			//敵キャラの衝突時コールバックを呼び出す
			enemy_->OnCollision();
			//自弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}

	}
#pragma endregion

#pragma region 自弾と敵弾の当たり判定

	for(auto pBullet : playerBullets){
		posA = pBullet->GetPos();
		for (auto eBullet : enemyBullets) {
			posB = eBullet->GetPos();

			float length = Length(posA, posB);

			if (length <= player_->kSize + enemy_->kSize) {
				//自弾の衝突時コールバックを呼び出す
				pBullet->OnCollision();
				//敵弾の衝突時コールバックを呼び出す
				eBullet->OnCollision();
			}
		}
	}
#pragma endregion
}