#pragma once
#include <memory>
#include <string>
#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class GameScene;

class Replay
{
public:
	static Replay* GetInstance();

	void Initialize(GameScene* gameScene);

	void InitializeSave();

	void InitializeReplay();

	void SaveUpdate();

	void ReplayUpdate();

	void ReplayPreparation();

	void SettingFrame();

	void EndInGame();

	enum class GameStatus {
		kInGame,
		kReplay,
		kNone,
	};

public: //ゲッターセッター
	inline std::string GetGroupName() { return groupName; }
	inline GameStatus GetGemeStatus() { return gameStatus_; }
	inline void SetGemeStatus(GameStatus gameStatus) { gameStatus_ = gameStatus; }
	inline XINPUT_STATE GetJoyState() { return joyState_; }
	inline bool IsReplay() { return isReplay_; }
	inline uint32_t GetStartFrame() { return startFrame_; }
	inline uint32_t GetEndFrame() { return endFrame_; }
	inline void SetFrame(int32_t* frame) { frame_ = frame; }
	inline int32_t GetPreFrame() { return preFrame_; }
	inline void SetPreFrame(int32_t preFrame) { preFrame_ = preFrame; }
	int32_t* frame_;
private:
	Replay() = default;
	~Replay() = default;
	Replay(const Replay& globalVariables) = delete;
	Replay& operator=(const Replay&) = delete;

	//グループネーム
	const std::string groupName = "Replay";

	//ゲームシーン
	GameScene* gameScene_;

	//操作記録
	//コントローラー
	XINPUT_STATE joyState_;

	//リプレイ
	GameStatus gameStatus_;
	bool isReplay_;

	int32_t startFrame_;
	int32_t endFrame_;

	int32_t preFrame_;
	int32_t targetFrame_;
};

