#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Collider.h"

class PlayerBullet : public Collider {
public:
	void Initialize(Model* model, const Vector3& position,const Vector3& velocity);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	void OnCollision() override;

	Vector3 GetWorldPosition() override;

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;

	//速度
	Vector3 velocity_;

	//タイマー
	static const int32_t kLifeTime = 60 * 5;
	int32_t deathTimer_ = kLifeTime;
	bool isDead_ = false;

public:
	bool IsDead() const { return isDead_; }

};
