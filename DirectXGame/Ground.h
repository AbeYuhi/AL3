#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Model.h"

class Ground
{
public:
	Ground();
	~Ground();

	void Initialize(Model* model);

	void Update();

	void Draw(ViewProjection viewProjection);

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;

};

