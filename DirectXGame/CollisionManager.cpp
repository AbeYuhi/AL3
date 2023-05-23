#include "CollisionManager.h"


CollisionManager::CollisionManager(){
}

CollisionManager::~CollisionManager(){
}

void CollisionManager::UpDate() {
	ClearCollider();

	SetCollider();

	CheckAllCollisions();
}

void CollisionManager::ClearCollider() {
	colliders_.clear();
}

void CollisionManager::SetCollider() {

	//自弾リストの取得
	std::list<PlayerBullet*> playerBullets = player_->GetBullets();
	//敵弾リストの取得
	std::list<EnemyBullet*> enemyBullets = enemy_->GetBullets();

	//コライダーをリストに登録
	colliders_.push_back(player_);
	colliders_.push_back(enemy_);
	//自弾すべて
	for (auto bullet : playerBullets) {
		colliders_.push_back(bullet);
	}
	//敵弾すべて
	for (auto bullet : enemyBullets) {
		colliders_.push_back(bullet);
	}
}

void CollisionManager::CheckAllCollisions() {
	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA) {
		auto colliderA = *itrA;

		std::list<Collider*>::iterator itrB = itrA;
		itrB++;
		for (; itrB != colliders_.end(); ++itrB) {
			auto colliderB = *itrB;

			CheckCollisionPair(colliderA, colliderB);
		}
	}
}

void CollisionManager::CheckCollisionPair(Collider* colliderA, Collider* colliderB) {

	if ((colliderA->GetCollisionAttribute() & colliderB->GetCollisionMask()) == 0 || (colliderA->GetCollisionMask() & colliderB->GetCollisionAttribute()) == 0){
		return;
	}

	Vector3 posA = colliderA->GetWorldPosition();
	Vector3 posB = colliderB->GetWorldPosition();
	float length = Length(posA, posB);

	if (length <= colliderA->GetRadius() + colliderB->GetRadius()) {
		colliderA->OnCollision();
		colliderB->OnCollision();
	}
}