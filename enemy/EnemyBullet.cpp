#include "Affin.h"
#include "CalcMath.h"
#include "EnemyBullet.h"
#include <cassert>
#include "Player/Player.h"

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	assert(model);

	model_ = model;
	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("white.png");

	worldTransform_.Initialize();

	worldTransform_.translation_ = position;

	worldTransform_.scale_ = {0.5f, 0.5f, 3.0f};

	velocity_ = velocity;

	//Y軸角度
	worldTransform_.rotation_.y = std::atan2(velocity_.x, velocity_.z);
	float velociteXZ = Length({velocity_.x, 0.0f, velocity_.z});
	//X軸角度
	worldTransform_.rotation_.x = std::atan2(-velocity_.y, velociteXZ);

	SetCollisionAttribute(CollisionConfig::kCollisionAttributeEnemy);
	SetCollisionMask(~CollisionConfig::kCollisionAttributeEnemy);
}

void EnemyBullet::Update() {
	Vector3 toPlayer = Subtract(player_->GetWorldPosition(), worldTransform_.translation_);
	toPlayer = Normalise(toPlayer);
	velocity_ = Normalise(velocity_);
	velocity_ = Slerp(velocity_, toPlayer, 0.1f);

	// Y軸
	worldTransform_.rotation_.y = std::atan2(velocity_.x, velocity_.z);
	float velociteXZ = Length({velocity_.x, 0.0f, velocity_.z});
	// X軸
	worldTransform_.rotation_.x = std::atan2(-velocity_.y, velociteXZ);

	worldTransform_.translation_ = VectorMultiply(worldTransform_.translation_, velocity_);

	// 時間経過で消滅
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	// ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();
}

void EnemyBullet::OnCollision() { isDead_ = true; }

void EnemyBullet::Draw(const ViewProjection& viewProjection) {
	// モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

Vector3 EnemyBullet::GetWorldPosition() {
	Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}