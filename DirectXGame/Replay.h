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

class Replay
{
public:
	static Replay* GetInstance();

	void StartReplay();

private:
	Replay() = default;
	~Replay() = default;
	Replay(const Replay& globalVariables) = delete;
	Replay& operator=(const Replay&) = delete;

};

