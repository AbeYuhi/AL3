#pragma once
#include <memory>

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"

#include "Skydome.h"
#include "Ground.h"
#include "Player.h"
#include "Enemy.h"
#include "FollowCamera.h"

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

	void ReplayUpdate();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

public: //ゲッターセッター
	inline int32_t GetFrame() { return frame_; }
	inline void SetFrame(int32_t frame) { frame_ = frame; }

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	std::unique_ptr<FollowCamera> followCamera_;
	ViewProjection viewProjection_;

	std::unique_ptr<Model> skydomeModel_ = nullptr;
	std::unique_ptr<Skydome> skydome_ = nullptr;

	std::unique_ptr<Model> groundModel_ = nullptr;
	std::unique_ptr<Ground> ground_ = nullptr;

	std::unique_ptr<Model> playerHeadModel_ = nullptr;
	std::unique_ptr<Model> playerBodyModel_ = nullptr;
	std::unique_ptr<Model> playerLeftArmModel_ = nullptr;
	std::unique_ptr<Model> playerRightArmModel_ = nullptr;
	std::unique_ptr<Model> playerWeaponModel_ = nullptr;
	std::unique_ptr<Player> player_ = nullptr;

	std::unique_ptr<Model> enemyBodyModel_ = nullptr;
	std::unique_ptr<Enemy> enemy_ = nullptr;

	//フレーム
	int32_t frame_;
};
