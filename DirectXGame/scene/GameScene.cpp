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

	viewProjection_.Initialize();

	//デバックカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());
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
	//自弾リストの取得
	std::list<PlayerBullet*> playerBullets = player_->GetBullets();
	//敵弾リストの取得
	std::list<EnemyBullet*> enemyBullets = enemy_->GetBullets();

	std::list<Collider*> colliders_;
	//コライダーをリストに登録
	colliders_.push_back(player_);
	colliders_.push_back(enemy_);
	//自弾すべて
	for (auto bullet : playerBullets) {
		colliders_.push_back(bullet);
	}
	//敵弾すべて
	for (auto bullet : enemyBullets) {
		colliders_.push_back(bullet);
	}
	//リストのペアを総当たり
	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA) {
		auto colliderA = *itrA;

		//イテレーターBはイテレーターAの次の要素から回す
		std::list<Collider*>::iterator itrB = itrA;
		itrB++;
		for (; itrB != colliders_.end(); ++itrB) {
			auto colliderB = *itrB;

			//ペアの当たり判定
			CheckCollisionPair(colliderA, colliderB);
		}
	}

}

void GameScene::CheckCollisionPair(Collider* colliderA, Collider* colliderB) {

	if ((colliderA->GetCollisionAttribute() & colliderB->GetCollisionMask()) == 0 || (colliderA->GetCollisionMask() & colliderB->GetCollisionAttribute()) == 0) {
		return;
	}

	Vector3 posA = colliderA->GetWorldPosition();
	Vector3 posB = colliderB->GetWorldPosition();
	float length = Length(posA, posB);
	//交差判定
	if (length <= colliderA->GetRadius() + colliderB->GetRadius()) {
		colliderA->OnCollision();
		colliderB->OnCollision();
	}
}