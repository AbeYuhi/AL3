#include "RailCamera.h"

RailCamera::RailCamera() {}

RailCamera::~RailCamera() {}

void RailCamera::Initialize(Vector3 position, Vector3 rotate) {
	worldTransform_.translation_ = position;
	worldTransform_.rotation_ = rotate;
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	//viewProjection_.farZ = 200;
	viewProjection_.Initialize();
	viewProjection_.matView = Inverse(worldTransform_.matWorld_);
	viewProjection_.matProjection = MakePerspectiveFovMatrix(viewProjection_.fovAngleY, viewProjection_.aspectRatio, viewProjection_.nearZ, viewProjection_.farZ);
	}


void RailCamera::Update(Vector3 translation, Vector3 rotate) {

	worldTransform_.translation_ += translation;
	worldTransform_.rotation_ += rotate;

	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	viewProjection_.matView = Inverse(worldTransform_.matWorld_);
	viewProjection_.matProjection = MakePerspectiveFovMatrix(viewProjection_.fovAngleY, viewProjection_.aspectRatio, viewProjection_.nearZ, viewProjection_.farZ);

	ImGui::Begin("Camera");
	float* pos[3] = { &worldTransform_.translation_.x, &worldTransform_.translation_.y, &worldTransform_.translation_.z };
	float* rotation[3] = { &worldTransform_.rotation_.x, &worldTransform_.rotation_.y, &worldTransform_.rotation_.z };
	ImGui::SliderFloat3("position", *pos, -500, 500);
	ImGui::SliderFloat3("rotation", *rotation, -2.0f * 3.14f, 2.0f * 3.14f);
	ImGui::End();
}