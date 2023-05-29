#pragma once
#include <algorithm>
#include <iostream>
#include <list>
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "ImGuiManager.h"

class RailCamera {
public:
	RailCamera();
	~RailCamera();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Vector3 position, Vector3 rotate);

	/// <summary>
	/// 更新
	/// </summary>
	void Update(Vector3 position, Vector3 rotate);

	inline ViewProjection& GetViewProjection() { return viewProjection_; };
	inline WorldTransform& GetWorldTtansform() { return worldTransform_; };

	/// <summary>
	/// プロジェクション行列計算用のメンバ設定関数群
	/// </summary>
	void SetFovAngleY(float value) { viewProjection_.fovAngleY = value; }
	void SetAspectRatio(float value) { viewProjection_.aspectRatio = value; }
	void SetNearZ(float value) { viewProjection_.nearZ = value; }
	void SetFarZ(float value) { viewProjection_.farZ = value; }

private:
	//ワールド変換データ
	WorldTransform worldTransform_;

	ViewProjection viewProjection_;
};
