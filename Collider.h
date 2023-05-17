#pragma once

class Collider
{
public:
	Collider();
	~Collider();

	virtual void OnCollision();

	//ゲッターセッター
	inline float GetRadius() { return radius; }
	inline void SetRadius(float radius) { this->radius = radius; }

private:
	//半径
	float radius = 2;
};
