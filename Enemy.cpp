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
	kCharacterSpeed_ = 0.001f;

	static_cast<size_t>(phase_);
}

void Enemy::Update() {

	(this->*phasetable_[0])();

	if (worldTransform_.translation_.z < 0.0f) {
		(this->*phasetable_[1])();
	}

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
	move_.z -= kCharacterSpeed_;
}

void Enemy::Leave() {
	move_.x -= kCharacterSpeed_;
	move_.y += kCharacterSpeed_;
}