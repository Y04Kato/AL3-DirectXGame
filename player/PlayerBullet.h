#pragma once
#include "Model.h"
#include "WorldTransform.h"

class PlayerBullet {
public:
	void Initialize(Model* model, const Vector3& position,const Vector3& velocity);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	void OnCollision();

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;

	//���x
	Vector3 velocity_;

	//�^�C�}�[
	static const int32_t kLifeTime = 60 * 5;
	int32_t deathTimer_ = kLifeTime;
	bool isDead_ = false;

public:
	bool IsDead() const { return isDead_; }

};
