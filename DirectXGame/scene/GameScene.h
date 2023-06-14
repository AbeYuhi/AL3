#pragma once

#include <sstream>
#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"
#include "Player.h"
#include "Enemy.h"
#include "Skydome.h"
#include "RailCamera.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 衝突判定
	/// </summary>
	void CheckAllCollisions();

	/// <summary>
	/// 敵弾を追加する
	/// </summary>
	/// <param name="enemyBullet">敵弾</param>
	void AddEnemyBullet(EnemyBullet* enemyBullet);

	void PopEnemy(Vector3 pos);

	void LoadEnemyPopData();

	/// <summary>
	/// 敵発生コマンドの更新
	/// </summary>
	void UpdateEnemyPopCommands();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	uint32_t textureHandle = 0;
	Model* model_ = 0;

	//自キャラ
	Player* player_ = 0;
	//2Dレティクル


	//敵キャラ
	std::list<Enemy*> enemys_;
	uint32_t enemyTexture = 0;

	ViewProjection viewProjection_;

	DebugCamera* debugCamera_;

	bool isDebugCameraActive_ = false;

	Model* modelSkydome_ = 0;
	Skydome* skydome_ = 0;

	RailCamera* railCamera_ = 0;

	std::list<EnemyBullet*> enemyBullets_;

	//敵発生コマンド
	std::stringstream enemyPopCommands_;
	bool isWait_;
	int waitTime_;
};
