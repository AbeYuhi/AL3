#pragma once
#pragma once
#include <algorithm>
#include <iostream>
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Model.h"
#include "Input.h"
#include "ImGuiManager.h"

class PlayerBullet
{
public:
	PlayerBullet();
	~PlayerBullet();


	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="position">初期座標</param>
	void Initialize(Model* model, const Vector3& position);

	void Update();

	/// <summary>
	/// 
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw(const ViewProjection& viewProjection);


private:


	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル	
	uint32_t textureHandle_ = 0u;
};