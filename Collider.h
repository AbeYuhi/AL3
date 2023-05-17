#pragma once

class Collider
{
public:
	Collider();
	~Collider();

	virtual void OnCollision();

	//�Q�b�^�[�Z�b�^�[
	inline float GetRadius() { return radius; }
	inline void SetRadius(float radius) { this->radius = radius; }

private:
	//���a
	float radius = 2;
};
