#include "Player.h"
#include <cassert>

Player::Player()
{
}

Player::~Player()
{
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
}


void Player::Initialize(Model* model, uint32_t textureHandle) {
	assert(model);

	this->model_ = model;
	this->textureHandle_ = textureHandle;

	worldTransform_.Initialize();

	input_ = Input::GetInstance();

	//衝突属性を設定
	SetCollisionAttribute(kCollisionAttributePlayer);
	SetCollisionMask(~kCollisionAttributePlayer);
}

void Player::Update() {

	//プレイヤーの移動ベクトル
	Vector3 move = {0, 0, 0};

	//キャラクターの移動速さ
	const float kCharacterSpeed = 0.2f;


	//攻撃処理
	Attack();

	//弾更新
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}
	//デスフラグのたった弾の削除
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	//旋回処理
	Rotate();

	//移動処理
	//押した方向で移動ベクトルを変更(左右)
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	}
	else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}

	//押した方向で移動ベクトルを変更(上下)
	if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	}
	else if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	}

	worldTransform_.translation_ += move;

	//移動限界座標
	const int kMoveLimitX = 30;
	const int kMoveLimitY = 15;
	const int kMoveLimitZ = 10;

	worldTransform_.translation_.x = std::clamp(static_cast<float>(worldTransform_.translation_.x), static_cast<float>(kMoveLimitX * -1), static_cast<float>(kMoveLimitX));
	worldTransform_.translation_.y = std::clamp(static_cast<float>(worldTransform_.translation_.y), static_cast<float>(kMoveLimitY * -1), static_cast<float>(kMoveLimitY));


	worldTransform_.UpdateMatrix();

	ImGui::Begin("PlayerState");
	ImGui::SliderFloat("PosX", &worldTransform_.translation_.x, -1 * kMoveLimitX, kMoveLimitX);
	ImGui::SliderFloat("PosY", &worldTransform_.translation_.y, -1 * kMoveLimitY, kMoveLimitY);
	ImGui::SliderFloat("PosZ", &worldTransform_.translation_.z, -1 * kMoveLimitZ, kMoveLimitZ);
	ImGui::End();
}

void Player::Draw(ViewProjection viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Player::Rotate() {
	//回転速さ
	const float kRotSpeed = 0.02f;

	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}
	else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	}

}

void Player::Attack() {
	if (input_->TriggerKey(DIK_SPACE)) {

		//弾の速度
		const Vector3 kbulletSpeed(0, 0, 1.0f);

		//速度ベクトルを自機の向きにあわせる
		Vector3 velocity = TransformNormal(kbulletSpeed, worldTransform_.matWorld_);

		//弾を生成し初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, worldTransform_.translation_, velocity);

		//弾を登録する
		bullets_.push_back(newBullet);
	}
}

void Player::OnCollision() {

}

const std::list<PlayerBullet*> Player::GetBullets() {
	return bullets_;
}