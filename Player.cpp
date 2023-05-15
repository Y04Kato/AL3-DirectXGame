#include "Player.h"
#include "Affin.h"
#include "ImGuiManager.h"
#include <cassert>

Player::Player() {

}

Player::~Player() {
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
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
	//ƒfƒXƒtƒ‰ƒO‚Ì—§‚Á‚½’e‚Ìíœ
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});


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

	// ’eXV
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
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
	if (input_->TriggerKey(DIK_SPACE)) {
		//’e‚Ì‘¬“x
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		//‘¬“xƒxƒNƒgƒ‹‚ðŽ©‹@‚É‡‚í‚¹‚Ä‰ñ“]‚³‚¹‚é
		velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		// ’e‚ð¶¬‚µ‰Šú‰»
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, worldTransform_.translation_, velocity);

		// ’e‚ð“o˜^‚·‚é
		bullets_.push_back(newBullet);
	}
}

void Player::Draw(ViewProjection viewProjection) {
	// 3Dƒ‚ƒfƒ‹‚ð•`‰æ
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	// ’e•`‰æ
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}