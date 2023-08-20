#include "Player.h"
#include <cassert>
#define _USE_MATH_DEFINES
#include <math.h>
#include "ImGuiManager.h"

void Player::Initialize(Model* modelBody, Model* modelHead, Model* modelL_arm, Model* modelR_arm) {
	// NULLポインタチェック
	assert(modelBody);
	assert(modelHead);
	assert(modelL_arm);
	assert(modelR_arm);

	modelBody_ = modelBody;
	modelHead_ = modelHead;
	modelL_arm_ = modelL_arm;
	modelR_arm_ = modelR_arm;

	worldTransformHead_.translation_.y = 1.8f;

	worldTransformL_arm_.translation_.x = -0.5f;
	worldTransformL_arm_.translation_.y = 1.8f;

	worldTransformR_arm_.translation_.x = 0.5f;
	worldTransformR_arm_.translation_.y = 1.8f;

	worldTransformBase_.parent_ = &GetWorldTransformBody();
	worldTransformHead_.parent_ = &GetWorldTransformBody();
	worldTransformL_arm_.parent_ = &GetWorldTransformBody();
	worldTransformR_arm_.parent_ = &GetWorldTransformBody();

	InitializeFloatingGimmick();

	worldTransformBase_.Initialize();
	worldTransformBody_.Initialize();
	worldTransformHead_.Initialize();
	worldTransformL_arm_.Initialize();
	worldTransformR_arm_.Initialize();

	input_ = Input::GetInstance();
}

void Player::Update() {
	// 行列を定数バッファに転送
	worldTransformBase_.TransferMatrix();

	XINPUT_STATE joystate;

	if (Input::GetInstance()->GetJoystickState(0, joystate)) {
		const float kCharacterSpeed = 0.3f;
		Vector3 move = {
		    (float)joystate.Gamepad.sThumbLX / SHRT_MAX, 0.0f,
		    (float)joystate.Gamepad.sThumbLY / SHRT_MAX};
		move = Multiply(kCharacterSpeed, Normalise(move));

		Matrix4x4 rotateMatrix = MakeRotateMatrix(viewProjection_->rotation_);
		move = TransformNormal(move, rotateMatrix);

		worldTransformBase_.translation_ = Add(move, worldTransformBase_.translation_);
		worldTransformBody_.translation_ = worldTransformBase_.translation_;

		worldTransformBase_.rotation_.y = std::atan2(move.x, move.z);
		worldTransformBody_.rotation_.y = worldTransformBase_.rotation_.y;
	}

	UpdateFloatingGimmick();

	worldTransformBase_.UpdateMatrix();
	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();
}

void Player::Draw(ViewProjection viewProjection) {
	// 3Dモデルを描画
	modelBody_->Draw(worldTransformBody_, viewProjection);
	modelHead_->Draw(worldTransformHead_, viewProjection);
	modelL_arm_->Draw(worldTransformL_arm_, viewProjection);
	modelR_arm_->Draw(worldTransformR_arm_, viewProjection);
}

void Player::InitializeFloatingGimmick() { floatingParameter_ = 0.0f; }

void Player::UpdateFloatingGimmick() {
	const float step = 2.0f * (float)M_PI / floatingCycle_;

	floatingParameter_ += step;
	floatingParameter_ = (float)std::fmod(floatingParameter_, 2.0f * M_PI);

	worldTransformBody_.translation_.y = std::sin(floatingParameter_) * floatingAmplitude_;

	worldTransformL_arm_.rotation_.x = std::sin(floatingParameter_) * 0.75f;
	worldTransformR_arm_.rotation_.x = std::sin(floatingParameter_) * 0.75f;

	ImGui::Begin("Player");
	ImGui::SliderFloat("Head Translation", &worldTransformHead_.translation_.x, -5.0f, 5.0f);
	ImGui::SliderFloat("ArmL Translation", &worldTransformL_arm_.translation_.x, -5.0f, 5.0f);
	ImGui::SliderFloat("ArmR Translation", &worldTransformR_arm_.translation_.x, -5.0f, 5.0f);
	ImGui::SliderInt("FloatingCycle", (int*)&floatingCycle_, 0, 240);
	ImGui::SliderFloat("FloatingAmplitude", &floatingAmplitude_, -1.0f, 1.0f);
	ImGui::End();
}