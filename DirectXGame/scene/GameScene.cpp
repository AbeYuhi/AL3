#include "GameScene.h"
#include "TextureManager.h"
#include "AxisIndicator.h"
#include <cassert>
#include <fstream>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete player_;
	enemys_.clear();
	delete debugCamera_;
	delete skydome_;
	delete railCamera_;
	enemyBullets_.clear();
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

	railCamera_ = new RailCamera();
	railCamera_->Initialize({0, 0, -50}, {0, 0, 0});

	//自キャラとレールカメラの親子関係
	player_->SetParent(&railCamera_->GetWorldTtansform());

	LoadEnemyPopData();
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
		railCamera_->Update({0, 0, 0.0f}, {0, 0, 0});

		viewProjection_.matView = railCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;
		//ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	}

	ImGui::Begin("DebugCamera");
	ImGui::Text("True(1) or False(0), %d", isDebugCameraActive_);
	ImGui::Text("%f", viewProjection_.farZ);
	ImGui::End();

	UpdateEnemyPopCommands();

	player_->Update();

	enemys_.remove_if([](Enemy* enemy) {
		if (enemy->IsDead()) {
			return true;
		}
		enemy->Update();
		return false;
		});

	enemyBullets_.remove_if([](EnemyBullet* bullet) {
	if (bullet->IsDead()) {
		return true;
	}
	bullet->Update();
	return false;
	});

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

	for (auto enemy : enemys_) {
		enemy->Draw(viewProjection_);
	}
	for (auto enemyBullet : enemyBullets_) {
		enemyBullet->Draw(viewProjection_);
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

#pragma region 自キャラと敵弾の当たり判定
	posA = player_->GetWorldPosition();

	for (auto bullet : enemyBullets_) {
		posB = bullet->GetPos();

		float length = Length(posA, posB);

		if (length <= player_->kSize + bullet->kSize) {
			//自キャラの衝突時コールバックを呼び出す
			player_->OnCollision();
			//敵弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}

	}

#pragma endregion

#pragma region 敵キャラと自弾の当たり判定
	for (auto enemy : enemys_) {
		posA = enemy->GetWorldPosition();

		for (auto bullet : playerBullets) {
			posB = bullet->GetPos();

			float length = Length(posA, posB);

			if (length <= player_->kSize + enemy->kSize) {
				//敵キャラの衝突時コールバックを呼び出す
				enemy->OnCollision();
				//自弾の衝突時コールバックを呼び出す
				bullet->OnCollision();
			}

		}
	}
#pragma endregion

#pragma region 自弾と敵弾の当たり判定

	for(auto pBullet : playerBullets){
		posA = pBullet->GetPos();
		for (auto eBullet : enemyBullets_) {
			posB = eBullet->GetPos();

			float length = Length(posA, posB);

			if (length <= player_->kSize + eBullet->kSize) {
				//自弾の衝突時コールバックを呼び出す
				pBullet->OnCollision();
				//敵弾の衝突時コールバックを呼び出す
				eBullet->OnCollision();
			}
		}
	}
#pragma endregion
}

void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet) {
	enemyBullets_.push_back(enemyBullet);
}

void GameScene::PopEnemy(Vector3 pos) {
	Enemy* enemy = new Enemy();
	enemy->Initialize(model_, enemyTexture, pos);
	enemy->SetGameScene(this);

	player_->SetEnemy(enemy);
	enemy->SetPlayer(player_);

	enemys_.push_back(enemy);
}

void GameScene::LoadEnemyPopData() {
	//ファイルを開く
	std::ifstream file;
	file.open("Resources/enemyPop.csv");
	assert(file.is_open());
	//ファイルの内容を文字列ストリームにコピー
	enemyPopCommands_ << file.rdbuf();

	//ファイルを閉じる
	file.close();
}

void GameScene::UpdateEnemyPopCommands() {
	//待機処理中
	if (isWait_) {
		waitTime_--;
		if (waitTime_ < 0) {
			isWait_ = false;
		}
		return;
	}

	//1行分の文字列を入れる変数
	std::string line;
	//コマンド実行ループ
	while (getline(enemyPopCommands_, line)) {
		std::istringstream line_stream(line);

		std::string word;
		//,区切りで行の先頭文字列を取得
		getline(line_stream, word, ',');

		if (word.find("//") == 0) {
			//コメント行を飛ばす
			continue;
		}

		if (word.find("POP") == 0) {
			//x座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());
			
			//y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			//z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			//敵を発生させえる
			PopEnemy(Vector3(x, y, z));
		}

		if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');

			//待ち時間
			int32_t waitTime = atoi(word.c_str());

			//待機開始
			isWait_ = true;
			waitTime_ = waitTime;

			//コマンドループを抜ける
			break;
		}
	}
}