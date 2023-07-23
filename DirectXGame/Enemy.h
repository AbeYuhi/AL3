#pragma once
#include <math.h>
#include <vector>
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Model.h"
#include "Input.h"
#include "ImGuiManager.h"
#include "Entity.h"

class Enemy : public Entity
{
public:
	Enemy();
	~Enemy();

	enum EnemyModels {
		Body
	};

	void Initialize(const std::vector<Model*>& models) override;

	void Update() override;

	void Draw(ViewProjection viewProjection) override;

private:


};

