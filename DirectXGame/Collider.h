#pragma once
#include <cstdint>
#include "Vector3.h"
#include "CollisionConfig.h"

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
	//属性
	inline uint32_t GetCollisionAttribute() { return collisionAttribute_; }
	inline void SetCollisionAttribute(uint32_t collisionAttribute) { this->collisionAttribute_ = collisionAttribute; }
	//マスク
	inline uint32_t GetCollisionMask() { return collisionMask_; }
	inline void SetCollisionMask(uint32_t collisionMask) { this->collisionMask_ = collisionMask; }
private:
	//半径
	float radius_ = 2; 
	//衝突属性(自分)
	uint32_t collisionAttribute_ = 0xffffffff;
	//衝突マスク(相手)
	uint32_t collisionMask_ = 0xffffffff;
};
