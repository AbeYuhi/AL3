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
	delete sprite2DReticle_;
}


void Player::Initialize(Model* model, uint32_t textureHandle) {
	assert(model);

	this->model_ = model;
	this->textureHandle_ = textureHandle;
	uint32_t textureReticle = TextureManager::Load("Reticle.png");
	//スプライト生成
	sprite2DReticle_ = Sprite::Create(textureReticle, {640, 360}, {1, 1, 1, 1}, {0.5, 0.5});

	worldTransform_.Initialize();
	worldTransform_.translation_.z = 20;

	input_ = Input::GetInstance();

	worldTransform3DReticle_.Initialize();
}

void Player::Update(ViewProjection viewProjection) {

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

	ImGui::Begin("PlayerState");
	ImGui::SliderFloat3("Position", &worldTransform_.translation_.x, -50, 50);
	ImGui::End();
	worldTransform_.translation_ += move;

	//移動限界座標
	const int kMoveLimitX = 30;
	const int kMoveLimitY = 15;

	worldTransform_.translation_.x = std::clamp(static_cast<float>(worldTransform_.translation_.x), static_cast<float>(kMoveLimitX * -1), static_cast<float>(kMoveLimitX));
	worldTransform_.translation_.y = std::clamp(static_cast<float>(worldTransform_.translation_.y), static_cast<float>(kMoveLimitY * -1), static_cast<float>(kMoveLimitY));

	worldTransform_.UpdateMatrix();

	//自機から3Dレティクルへの距離
	const float kDistancePlayer3DRericle = 50.0f;
	//自機から3Dレティクルへのオフセット
	Vector3 offset{ 0, 0, 1.0f };
	//自機のワールド座標の回転を反映
	offset = TransformNormal(offset, worldTransform_.matWorld_);
	//ベクトルの長さを整える
	offset = Normalize(offset);
	offset *= kDistancePlayer3DRericle;
	//3Dレティクルの座標を設定
	worldTransform3DReticle_.translation_ = worldTransform_.translation_ + offset;
	worldTransform3DReticle_.UpdateMatrix();

	Vector3 positionReticle = { worldTransform3DReticle_.matWorld_.m[3][0], worldTransform3DReticle_.matWorld_.m[3][1], worldTransform3DReticle_.matWorld_.m[3][2] };

	//ビューポート行列
	Matrix4x4 matViewport = MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
	//ビュー行列とプロジェクション行列、ビューポート行列を合成する
	Matrix4x4 matViewProjectionViewport = (viewProjection.matView * viewProjection.matProjection);
	matViewProjectionViewport *= matViewport;

	//ワールド->スクリーン座標変換
	positionReticle = Transform(positionReticle, matViewProjectionViewport);
	//スプライトのレティクルに座標設定
	sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));
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
		worldTransform_.rotation_.y -= kRotSpeed;
	}
	else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}

}

void Player::Attack() {
	if (input_->TriggerKey(DIK_SPACE)) {

		//弾の速度
		const float kbulletSpeed = 1.0f;

		//速度ベクトルを自機の向きにあわせる
		Vector3 velocity = worldTransform3DReticle_.translation_ - GetWorldPosition();
		velocity = Normalize(velocity);
		velocity *= kbulletSpeed;

		//弾を生成し初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, GetWorldPosition(), velocity);

		//弾を登録する
		bullets_.push_back(newBullet);
	}
}

void Player::OnCollision() {

}

void Player::DrawUI() {
	sprite2DReticle_->Draw();
}

const std::list<PlayerBullet*> Player::GetBullets() {
	return bullets_;
}