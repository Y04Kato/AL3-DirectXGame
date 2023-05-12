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
	// NULLƒ|ƒCƒ“ƒ^ƒ`ƒFƒbƒN
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

	worldTransform_.translation_ = VectorMultiply(worldTransform_.translation_, move);

	// ù‰ñˆ—
	Rotate();

	// UŒ‚ˆ—
	Attack();

	//’eXV
	if (bullet_) {
		bullet_->Update();
	}

	// Gui‚É‚æ‚éˆÚ“®
	float inputFloat3[3] = {
	    worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z};
	ImGui::Begin("Player");
	ImGui::Text("debugCamera = Q");
	ImGui::SliderFloat3("Player", inputFloat3, -40.0f, 40.0f);
	ImGui::End();
	worldTransform_.translation_.x = inputFloat3[0];
	worldTransform_.translation_.y = inputFloat3[1];
	worldTransform_.translation_.z = inputFloat3[2];

	// ˆÚ“®§ŒÀ
	const float kMoveLimitX = 33;
	const float kMoveLimitY = 18;

	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	worldTransform_.UpdateMatrix();
}

void Player::Rotate() {
	// ‰ñ“]‘¬‚³
	const float kRotSpeed = 0.02f;

	// „‚µ‚½•ûŒü‚ÅˆÚ“®ƒxƒNƒgƒ‹•ÏX
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y += kRotSpeed;
	} else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	}
}

void Player::Attack() {
	if (input_->PushKey(DIK_SPACE)) {
	//’e‚ð¶¬‚µ‰Šú‰»
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, worldTransform_.translation_);

		//’e‚ð“o˜^‚·‚é
		bullet_ = newBullet;
	}
}

void Player::Draw(ViewProjection viewProjection) {
	// 3Dƒ‚ƒfƒ‹‚ð•`‰æ
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	//’e•`‰æ
	if (bullet_) {
		bullet_->Draw(viewProjection);
	}
}