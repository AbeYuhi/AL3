#pragma once
#include <algorithm>
#include <iostream>
#include <list>
#include <memory>
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Model.h"
#include "Input.h"
#include "ImGuiManager.h"
#include "EnemyBullet.h"

class Player;
class GameScene;

class Enemy
{
public:
	Enemy();
	~Enemy();

	/// <summary>
	/// 行動パターン
	/// </summary>
	enum class Phase {
		Approach,	//接近する
		Leave,		//離脱する
	};

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void Initialize(Model* model, uint32_t textureHandle, Vector3 pos);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection viewProjection);

	void Fire();

	void PhaseApproachInitialize();

	/// <summary>
	/// 衝突したときに呼び出される関数
	/// </summary>
	void OnCollision();

	static const int kFireIntervel = 60;

	//ゲッターセッター
	inline void SetPlayer(Player* player) { player_ = player; }
	inline Vector3 GetWorldPosition() {
		Vector3 worldPos;
		worldPos.x = worldTransform_.matWorld_.m[3][0];
		worldPos.y = worldTransform_.matWorld_.m[3][1];
		worldPos.z = worldTransform_.matWorld_.m[3][2];
		return worldPos;
	}
	inline void SetGameScene(GameScene* gamescene) { gameScene_ = gamescene; }
	inline bool IsDead() { return isDead_; }
	static const int kSize = 2;

private:

	void PhaseApproach();

	void PhaseLeave();

	//ゲームシーン
	GameScene* gameScene_ = nullptr;
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル	
	uint32_t textureHandle_ = 0u;

	//フェーズ
	Phase phase_ = Phase::Approach;

	int32_t bulletCooldown = 0u;

	//自キャラのクラスポインタ
	Player* player_ = nullptr;

	bool isDead_;
};
