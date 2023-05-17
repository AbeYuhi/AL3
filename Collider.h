#pragma once
#include "Vector3.h"

class Collider
{
public:
	Collider();
	virtual ~Collider();

	virtual void OnCollision() = 0;

	virtual Vector3 GetWorldPosition() = 0;

	//ゲッターセッター
	inline float GetRadius() { return radius_; }
	inline void SetRadius(float radius) { this->radius_ = radius; }

private:
	//半径
	float radius_ = 2; 
};
