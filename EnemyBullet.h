#pragma once
#include <algorithm>
#include <iostream>
#include <list>
#include <memory>
#include "Vector3.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Model.h"
#include "Input.h"
#include "ImGuiManager.h"

class Player;

class EnemyBullet
{
public:
	EnemyBullet();
	~EnemyBullet();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="position">初期座標</param>
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	void Update();

	/// <summary>
	/// 
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw(const ViewProjection& viewProjection);

	//ゲッターセッター
	inline bool IsDead() const { return isDead_; }
	inline void SetPlayer(Player* player) { player_ = player; }


private:


	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル	
	uint32_t textureHandle_ = 0u;

	//速度
	Vector3 velocity_;

	//寿命
	static const int32_t kLifeTime = 60 * 5;

	//デスタイマー
	int32_t deathTimer_ = kLifeTime;
	bool isDead_ = false;
	float t = 0;

	Player* player_ = nullptr;

};
