#include "Enemy.h"
#include "Affin.h"
#include <cassert>

void Enemy::Initialize(Model* model, const Vector3& position) {
	assert(model);

	model_ = model;
	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("black.png");

	worldTransform_.Initialize();

	worldTransform_.translation_ = position;

	move_ = {0, 0, 0};
	kCharacterApproachSpeed_ = 0.001f;
	kCharacterLeaveSpeed_ = 0.005f;

	static_cast<size_t>(phase_);
}

void Enemy::Update() {

	(this->*phasetable_[0])();

	worldTransform_.translation_ = VectorMultiply(worldTransform_.translation_, move_);

	// ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	// モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void (Enemy::*Enemy::phasetable_[])() = {
    &Enemy::Approach, // 接近
    &Enemy::Leave     // 離脱
};

void Enemy::Approach() {
	// 移動
	move_.z -= kCharacterApproachSpeed_;
	if (worldTransform_.translation_.z < 0.0f) {
		(this->*phasetable_[1])();
	}
}

void Enemy::Leave() {
	move_.x -= kCharacterLeaveSpeed_;
	move_.y += kCharacterLeaveSpeed_;
}