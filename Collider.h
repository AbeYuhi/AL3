#pragma once
#include "Vector3.h"

class Collider
{
public:
	Collider();
	virtual ~Collider();

	virtual void OnCollision() = 0;

	virtual Vector3 GetWorldPosition() = 0;

public: //ゲッターセッター
	//半径
	inline float GetRadius() { return radius_; }
	inline void SetRadius(float radius) { this->radius_ = radius; }
	//衝突属性(自分)
	inline uint32_t GetCollisionAttriute() { return collisionAttriute_; }
	inline void SetCollisionAttriute(uint32_t collisionAttriute) { this->collisionAttriute_ = collisionAttriute; }
	//衝突マスク(相手)
	inline uint32_t GetCollisionMask() { return collisionMask_; }
	inline void SetCollisionMask(uint32_t collisionMask) { this->collisionMask_ = collisionMask; }

private:
	//半径
	float radius_ = 2; 

	//衝突属性(自分)
	uint32_t collisionAttriute_ = 0xffffffff;
	//衝突マスク(相手)
	uint32_t collisionMask_ = 0xffffffff;
};
