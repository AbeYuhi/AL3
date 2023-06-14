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

	//移動処理
	XINPUT_STATE joyState;
	//ジョイスティック状態取得
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		move.x += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * kCharacterSpeed;
		move.y += (float)joyState.Gamepad.sThumbLY / SHRT_MAX * kCharacterSpeed;
	}

	//移動限界座標
	const int kMoveLimitX = 30;
	const int kMoveLimitY = 15;

	worldTransform_.translation_.x = std::clamp(static_cast<float>(worldTransform_.translation_.x), static_cast<float>(kMoveLimitX * -1), static_cast<float>(kMoveLimitX));
	worldTransform_.translation_.y = std::clamp(static_cast<float>(worldTransform_.translation_.y), static_cast<float>(kMoveLimitY * -1), static_cast<float>(kMoveLimitY));
	worldTransform_.translation_ += move;

	worldTransform_.UpdateMatrix();

	//マウスの場合
	//POINT mousePosition;
	////マウス座標(スクリーン座標)を取得
	//GetCursorPos(&mousePosition);
	////クライアント座標に変換する
	//HWND hwnd = WinApp::GetInstance()->GetHwnd();
	//ScreenToClient(hwnd, &mousePosition);
	//sprite2DReticle_->SetPosition(Vector2((float)mousePosition.x, (float)mousePosition.y));

	//ゲームパッド
	Vector2 spritePosition = sprite2DReticle_->GetPosition();

	//ジョイスティック取得状態
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		spritePosition.x += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * 10.0f;
		spritePosition.y -= (float)joyState.Gamepad.sThumbRY / SHRT_MAX * 10.0f;

		sprite2DReticle_->SetPosition(spritePosition);
	}

	//ビュープロジェクションビューポート合成行列
	Matrix4x4 viewportMatrix = MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
	Matrix4x4 matVPV = Multiply(Multiply(viewProjection.matView, viewProjection.matProjection), viewportMatrix);
	Matrix4x4 matInverseVPV = Inverse(matVPV);

	//スクリーン座標
	Vector3 posNear = Vector3((float)spritePosition.x, (float)spritePosition.y, 0);
	Vector3 posFar = Vector3((float)spritePosition.x, (float)spritePosition.y, 1);
	//スクリーン座標系からワールド座標系へ
	posNear = Transform(posNear, matInverseVPV);
	posFar = Transform(posFar, matInverseVPV);

	//マウスレイの方向
	Vector3 mouseDirection = posFar - posNear;
	mouseDirection = Normalize(mouseDirection);
	//カメラから照準オブジェクトの距離
	const float kDistanceTestObject = 50;
	worldTransform3DReticle_.translation_ = posNear + (mouseDirection * kDistanceTestObject);
	worldTransform3DReticle_.UpdateMatrix();

	ImGui::Begin("Player");
	ImGui::Text("2DReticle:(%f, %f)", sprite2DReticle_->GetPosition().x, sprite2DReticle_->GetPosition().y);
	ImGui::Text("Near:(%+.2f, %+.2f,%+.2f)", posNear.x, posNear.y, posNear.z);
	ImGui::Text("Far:(%+.2f, %+.2f,%+.2f)", posFar.x, posFar.y, posFar.z);
	ImGui::Text("3DReticle:(%+.2f, %+.2f,%+.2f)", worldTransform3DReticle_.translation_.x, worldTransform3DReticle_.translation_.y, worldTransform3DReticle_.translation_.z);
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
		worldTransform_.rotation_.y -= kRotSpeed;
	}
	else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}

}

void Player::Attack() {
	XINPUT_STATE joyState;
	if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
		return;
	}

	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {

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