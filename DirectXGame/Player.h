#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Model.h"
#include "Input.h"

class Player
{
public:
	Player();
	~Player();

	void Initialize(Model* model);

	void Update();

	void Draw(ViewProjection viewProjection);

public: //ゲッターセッター
	WorldTransform &GetWorldTransform() { return worldTransform_; }

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
};

