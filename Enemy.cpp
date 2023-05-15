#include "Enemy.h"
#include <cassert>
#include "Affin.h"

void Enemy::Initialize(Model* model, const Vector3& position) {
	assert(model);

	model_ = model;
	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("black.png");

	worldTransform_.Initialize();

	worldTransform_.translation_ = position;
}

void Enemy::Update() {
	Vector3 move = {0, 0, 0};
	const float kCharacterSpeed = 0.2f;
	switch (phase_) { 
	case Phase::Approach:
	default:
		//移動
		move.z -= kCharacterSpeed;
		if (worldTransform_.translation_.z < 0.0f) {
			phase_ = Phase::Leave;
		}
		break;
	case Phase::Leave:
		move.x -= kCharacterSpeed;
		move.y += kCharacterSpeed;
		break;
	}

	worldTransform_.translation_ = VectorMultiply(worldTransform_.translation_, move);

	// ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	// モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}