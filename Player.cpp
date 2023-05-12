#include "Player.h"
#include "Affin.h"
#include "ImGuiManager.h"
#include <cassert>

Vector3 VectorMultiply(const Vector3& translation, const Vector3& move) {
	Vector3 result;
	result.x = translation.x + move.x;
	result.y = translation.y + move.y;
	result.z = translation.z + move.z;
	return result;
}

void Player::Initialize(Model* model, uint32_t textureHandle) {
	// NULL�|�C���^�`�F�b�N
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();

	input_ = Input::GetInstance();
}

void Player::Update() {
	Vector3 move = {0, 0, 0};
	const float kCharacterSpeed = 0.2f;
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	}
	if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}
	if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	}
	if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	}

	// Gui�ɂ��ړ�
	float inputFloat3[3] = {move.x + 1, move.y + 1, move.z + 1};
	ImGui::Begin("Player");
	ImGui::SliderFloat3("Player", inputFloat3, 0.0f, 2.0f);
	ImGui::End();
	move.x = inputFloat3[0] - 1;
	move.y = inputFloat3[1] - 1;
	move.z = inputFloat3[2] - 1;

	// �ړ�����
	const float kMoveLimitX = WinApp::kWindowWidth - 100;
	const float kMoveLimitY = WinApp::kWindowHeight - 100;

	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	worldTransform_.translation_ = VectorMultiply(worldTransform_.translation_, move);

	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	// �s���萔�o�b�t�@�ɓ]��
	worldTransform_.TransferMatrix();
}

void Player::Draw(ViewProjection viewProjection) {
	// 3D���f����`��
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}