#include "Enemy.h"

//行動パターン
PhaseState::PhaseState()
{
}

PhaseState::~PhaseState()
{	
}

//向かってくる
PhaseApproach::PhaseApproach()
{
}

PhaseApproach::~PhaseApproach()
{
}

void PhaseApproach::Update(Enemy* pEnemy) {

	pEnemy->Move({ 0, 0, -0.1f });

	if (pEnemy->GetPosition().z <= -10) {
		pEnemy->ChangeState(new PhaseLeave());
	}
}

//逃走
PhaseLeave::PhaseLeave()
{
}

PhaseLeave::~PhaseLeave()
{
}
void PhaseLeave::Update(Enemy* pEnemy) {

	pEnemy->Move({ 0, 0, 0.1f });

	if (pEnemy->GetPosition().z >= 10) {
		pEnemy->ChangeState(new PhaseApproach());
	}
}

Enemy::Enemy()
{

}

Enemy::~Enemy()
{
	delete state_;
}

void Enemy::Initialize(Model* model, uint32_t textureHandle) {
	assert(model);

	this->model_ = model;
	this->textureHandle_ = textureHandle;

	worldTransform_.Initialize();

	state_ = new PhaseApproach();
}

void Enemy::Update() {
	state_->Update(this);

	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(ViewProjection viewProjection) {

	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void Enemy::ChangeState(PhaseState* newState) {
	delete state_;
	state_ = newState;
}

void Enemy::Move(Vector3 speed) {
	worldTransform_.translation_ += speed;
}