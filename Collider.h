#pragma once
#include "Vector3.h"

class Collider
{
public:
	Collider();
	virtual ~Collider();

	virtual void OnCollision();

	virtual Vector3 GetWorldPosition();

	//ゲッターセッター
	inline float GetRadius() { return radius_; }
	inline void SetRadius(float radius) { this->radius_ = radius; }

private:
	//半径
	float radius_ = 2;
};
