#pragma once
#include <cstdint>
#include <concepts>
#include <compare>
#include <list>
#include "Collider.h"
#include "Player.h"
#include "PlayerBullet.h"
#include "Enemy.h"
#include "EnemyBullet.h"

class CollisionManager
{
public: //コンストラクタ、デストラクタ
	CollisionManager();
	~CollisionManager();

public: //関数

	void UpDate();

	void ClearCollider();

	void SetCollider();

	/// <summary>
	/// 衝突判定
	/// </summary>
	void CheckAllCollisions();

public: //ゲッターセッター

	inline void SetPlayer(Player* player) { player_ = player; }
	inline void SetEnemy(Enemy* enemy) { enemy_ = enemy; }

private:

	std::list<Collider*> colliders_;

	Player* player_;
	Enemy* enemy_;

	void CheckCollisionPair(Collider* colliderA, Collider* colliderB);
};

