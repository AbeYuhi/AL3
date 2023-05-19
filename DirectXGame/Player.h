#pragma once
#include <algorithm>
#include <iostream>
#include <list>
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Model.h"
#include "Input.h"
#include "ImGuiManager.h"
#include "PlayerBullet.h"
#include "Collider.h"

class Enemy;

/// <summary>
/// 自キャラ
/// </summary>
class Player : public Collider
{
public:
	Player();
	~Player();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void Initialize(Model* model, uint32_t textureHandle);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection viewProjection);

	/// <summary>
	/// 回転
	/// </summary>
	void Rotate();

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

	/// <summary>
	/// 衝突したときに呼び出される関数
	/// </summary>
	void OnCollision();

	//ゲッターセッター
	inline Vector3 GetWorldPosition() { return worldTransform_.translation_; }
	inline void SetEnemy(Enemy* enemy) { enemy_ = enemy; }
	const std::list<PlayerBullet*> GetBullets();

private:

	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル	
	uint32_t textureHandle_ = 0u;
	//キーボード入力
	Input* input_ = nullptr;

	//玉
	std::list<PlayerBullet*> bullets_;

	Enemy* enemy_ = nullptr;
};
