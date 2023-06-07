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
	delete skydome_;
	delete railCamera_;
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

	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection_);

	//デバックカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());

	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_);

	railCamera_ = new RailCamera();
	railCamera_->Initialize({0, 0, -50}, {0, 0, 0});

	//自キャラとレールカメラの親子関係
	player_->SetParent(&railCamera_->GetWorldTtansform());

	//スプライン制御点
	controlPoints_ = {
		{0, 0, 0},
		{10, 10, 0},
		{10, 15, 0},
		{20, 15, 0},
		{20, 0, 0},
		{30, 0, 0},
	};
}

void GameScene::Update() {
	//デバッグカメラの更新
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_0) && input_->PushKey(DIK_LSHIFT)) {
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
		Vector3 eye = {};
		Vector3 target = {};



		Vector3 railCameraRotation = {};
		railCameraRotation.y = std::atan2(velocity_.x, velocity_.z);
		float widthLength = Length({ velocity_.x, 0, velocity_.z });
		railCameraRotation.worldTransform_.rotation_.x = std::atan2(-velocity_.y, widthLength); */

		railCamera_->Update(eye, {0, 0, 0});
		viewProjection_.matView = railCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;
		//ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
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

	std::vector<Vector3> pointsDrawing;
	const size_t segmentCount = 100;
	for (size_t i = 0; i < segmentCount + 1; i++) {
		float t = 1.0f / segmentCount * i;
		Vector3 pos = CatmullRomSpline(controlPoints_, t);
		pointsDrawing.push_back(pos);
	}
	for (size_t i = 0; i < pointsDrawing.size() - 2; i++) {
		Vector3 posA = pointsDrawing[i];
		Vector3 posB = pointsDrawing[i + 1];
		PrimitiveDrawer::GetInstance()->DrawLine3d(posA, posB, { 1.0f, 0.0f, 0.0f, 1.0f });
	}
	pointsDrawing.clear();

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
	posA = player_->GetWorldPosition();

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
	posA = enemy_->GetWorldPosition();

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