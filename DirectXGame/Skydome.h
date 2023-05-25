#pragma once
#include <algorithm>
#include <iostream>
#include <list>
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Model.h"
#include "Input.h"
#include "ImGuiManager.h"

class Skydome {
public:
	Skydome();
	~Skydome();

	void Initialize(Model* model);

	void Update();

	void Draw(ViewProjection viewProjection);

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
};
