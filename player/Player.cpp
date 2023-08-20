#include "Player.h"
#include <cassert>

void Player::Initialize(Model* modelBody, Model* modelHead, Model* modelL_arm, Model* modelR_arm) {
	// NULLポインタチェック
	assert(model);

	model_ = model;

	input_ = Input::GetInstance();

	worldTransform_.Initialize();
}

void Player::Update() {
	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();

	XINPUT_STATE joystate;

	if (Input::GetInstance()->GetJoystickState(0, joystate)) {
		const float kCharacterSpeed = 0.3f;
		Vector3 move = {
		    (float)joystate.Gamepad.sThumbLX / SHRT_MAX, 0.0f,
		    (float)joystate.Gamepad.sThumbLY / SHRT_MAX};
		move = Multiply(kCharacterSpeed, Normalise(move));

		Matrix4x4 rotateMatrix = MakeRotateMatrix(viewProjection_->rotation_);
		move = TransformNormal(move, rotateMatrix);
		
		worldTransform_.translation_ = Add(move, worldTransform_.translation_);
		worldTransform_.rotation_.y = std::atan2(move.x, move.z);
	}

	worldTransform_.UpdateMatrix();
}

void Player::Draw(ViewProjection viewProjection) {
	// 3Dモデルを描画
	model_->Draw(worldTransform_, viewProjection);
}