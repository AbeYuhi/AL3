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
	void Initialize(Model* model, uint32_t textureHandle);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection viewProjection);

	void Fire();

private:

	void PhaseApproach();

	void PhaseLeave();

	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル	
	uint32_t textureHandle_ = 0u;

	//フェーズ
	Phase phase_ = Phase::Approach;

	std::unique_ptr<EnemyBullet> bullets_;
};
