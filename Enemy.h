#pragma once
#include <algorithm>
#include <iostream>
#include <list>
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Model.h"
#include "Input.h"
#include "ImGuiManager.h"

class Enemy;

class PhaseState
{
public:
	PhaseState();
	virtual ~PhaseState();

	virtual void Update(Enemy* pEnemy) = 0;

private:
	Enemy* enemy_;
};

class PhaseApproach : public PhaseState
{
public:
	PhaseApproach();
	~PhaseApproach();

	void Update(Enemy* pEnemy);

private:

};

class PhaseLeave : public PhaseState
{
public:
	PhaseLeave();
	~PhaseLeave();

	void Update(Enemy* pEnemy);

private:

};

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

	void ChangeState(PhaseState* newState);

	void Move(Vector3 speed);

	//ゲッターセッター
	inline Vector3 GetPosition() { return worldTransform_.translation_; }

private:

	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル	
	uint32_t textureHandle_ = 0u;

	PhaseState* state_;
};