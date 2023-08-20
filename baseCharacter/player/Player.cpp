#include "Player.h"
#include <cassert>
#define _USE_MATH_DEFINES
#include "ImGuiManager.h"
#include <math.h>

void Player::Initialize(const std::vector<Model*>& models) {
	// NULLポインタチェック
	BaseCharacter::Initialize(models);

	models_[kModelIndexBody] = models[kModelIndexBody];
	models_[kModelIndexHead] = models[kModelIndexHead];
	models_[kModelIndexL_arm] = models[kModelIndexL_arm];
	models_[kModelIndexR_arm] = models[kModelIndexR_arm];
	models_[kModelIndexWeapon] = models[kModelIndexWeapon];

	worldTransformHead_.translation_.y = 1.8f;

	worldTransformL_arm_.translation_.x = -0.5f;
	worldTransformL_arm_.translation_.y = 1.8f;

	worldTransformR_arm_.translation_.x = 0.5f;
	worldTransformR_arm_.translation_.y = 1.8f;

	worldTransformWeapon_.translation_.x = -0.2f;
	worldTransformWeapon_.translation_.y = 1.6f;
	worldTransformWeapon_.scale_ = {0.6f, 0.6f, 0.6f};

	worldTransformBase_.parent_ = &GetWorldTransformBody();
	worldTransformHead_.parent_ = &GetWorldTransformBody();
	worldTransformL_arm_.parent_ = &GetWorldTransformBody();
	worldTransformR_arm_.parent_ = &GetWorldTransformBody();
	worldTransformWeapon_.parent_ = &GetWorldTransformBody();

	InitializeFloatingGimmick();

	worldTransformBase_.Initialize();
	worldTransformBody_.Initialize();
	worldTransformHead_.Initialize();
	worldTransformL_arm_.Initialize();
	worldTransformR_arm_.Initialize();

	worldTransformWeapon_.Initialize();

	input_ = Input::GetInstance();
}

void Player::Update() {
	// 行列を定数バッファに転送
	worldTransformBase_.TransferMatrix();

	XINPUT_STATE joyState;

	if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
		return;
	}

	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_X) {
		behaviorRequest_ = Behavior::kAttack;
	}

	if (behaviorRequest_) {
		behavior_ = behaviorRequest_.value();

		switch (behavior_) {
		case Player::Behavior::kRoot:
		default:
			BehaviorRootInitialize();
			break;

		case Player::Behavior::kAttack:
			BehaviorAttackInitialize();
			break;
		}

		behaviorRequest_ = std::nullopt;
	}

	switch (behavior_) {
	case Player::Behavior::kRoot:
	default:
		BehaviorRootUpdate();
		break;
	case Player::Behavior::kAttack:
		BehaviorAttackUpdate();
		break;
	}

	ImGui::Begin("Player");
	ImGui::SliderFloat("Head Translation", &worldTransformHead_.translation_.x, -5.0f, 5.0f);
	ImGui::SliderFloat("ArmL Translation", &worldTransformL_arm_.translation_.x, -5.0f, 5.0f);
	ImGui::SliderFloat("ArmR Translation", &worldTransformR_arm_.translation_.x, -5.0f, 5.0f);
	ImGui::SliderInt("FloatingCycle", (int*)&floatingCycle_, 0, 240);
	ImGui::SliderFloat("FloatingAmplitude", &floatingAmplitude_, -1.0f, 1.0f);
	ImGui::Text("Attack : X");
	ImGui::End();

	worldTransformBase_.UpdateMatrix();
	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();

	worldTransformWeapon_.UpdateMatrix();
}

void Player::Draw(const ViewProjection& viewProjection) {
	// 3Dモデルを描画
	models_[kModelIndexBody]->Draw(worldTransformBody_, viewProjection);
	models_[kModelIndexHead]->Draw(worldTransformHead_, viewProjection);
	models_[kModelIndexL_arm]->Draw(worldTransformL_arm_, viewProjection);
	models_[kModelIndexR_arm]->Draw(worldTransformR_arm_, viewProjection);

	if (behavior_ == Behavior::kAttack) {
		models_[kModelIndexWeapon]->Draw(worldTransformWeapon_, viewProjection);
	}
}

void Player::InitializeFloatingGimmick() { floatingParameter_ = 0.0f; }

void Player::UpdateFloatingGimmick() {
	const float step = 2.0f * (float)M_PI / floatingCycle_;

	floatingParameter_ += step;
	floatingParameter_ = (float)std::fmod(floatingParameter_, 2.0f * M_PI);

	worldTransformBody_.translation_.y = std::sin(floatingParameter_) * floatingAmplitude_;

	worldTransformL_arm_.rotation_.x = std::sin(floatingParameter_) * 0.75f;
	worldTransformR_arm_.rotation_.x = std::sin(floatingParameter_) * 0.75f;
}

void Player::BehaviorRootInitialize() {
	worldTransformL_arm_.rotation_.x = 0.0f;
	worldTransformR_arm_.rotation_.x = 0.0f;
	worldTransformWeapon_.rotation_.x = 0.0f;

	InitializeFloatingGimmick();

	worldTransformBody_.Initialize();
	worldTransformHead_.Initialize();
	worldTransformL_arm_.Initialize();
	worldTransformR_arm_.Initialize();
	worldTransformWeapon_.Initialize();
}

void Player::BehaviorRootUpdate() {
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
}

void Player::BehaviorAttackInitialize() {
	worldTransformL_arm_.rotation_.x = (float)M_PI;
	worldTransformR_arm_.rotation_.x = (float)M_PI;
	worldTransformWeapon_.rotation_.x = 0.0f;
	animationFrame = 0;
}

void Player::BehaviorAttackUpdate() {
	if (animationFrame < 12) {
		worldTransformL_arm_.rotation_.x -= 0.05f;
		worldTransformR_arm_.rotation_.x -= 0.0f;

		worldTransformWeapon_.rotation_.x -= 0.05f;
	} else if (worldTransformWeapon_.rotation_.x <= 2.0f * (float)M_PI / 4) {
		worldTransformL_arm_.rotation_.x += 0.1f;
		worldTransformR_arm_.rotation_.x += 0.1f;

		worldTransformWeapon_.rotation_.x += 0.1f;
	} else {
		behaviorRequest_ = Behavior::kRoot;
	}

	animationFrame++;
}