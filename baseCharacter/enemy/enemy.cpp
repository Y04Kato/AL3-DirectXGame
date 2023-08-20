#include "Enemy.h"
#include "calcmath.h"

void Enemy::Initialize(const std::vector<Model*>& models) { BaseCharacter::Initialize(models); }

void Enemy::Update() {
	const float kSpeed = 0.2f;

	Vector3 velocity{0.0f, 0.0f, kSpeed};

	velocity = TransformNormal(velocity, worldTransform_.matWorld_);
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity);
	worldTransform_.rotation_.y += 0.02f;

	BaseCharacter::Update();
}

void Enemy::Draw(const ViewProjection& viewProjection) { BaseCharacter::Draw(viewProjection); }