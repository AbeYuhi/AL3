#pragma once
#include <algorithm>
#include <iostream>
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Model.h"
#include "Input.h"
#include "ImGuiManager.h"
#include "Collider.h"

class EnemyBullet : public Collider
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

	/// <summary>
	/// 衝突したときに呼び出される関数
	/// </summary>
	void OnCollision();

	//ゲッターセッター
	inline bool IsDead() const { return isDead_; }
	inline Vector3 GetWorldPosition() { return worldTransform_.translation_; }

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

};
