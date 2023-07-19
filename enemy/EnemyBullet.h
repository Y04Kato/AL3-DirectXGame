#pragma once
#include "Model.h"
#include "WorldTransform.h"
class Player;

class EnemyBullet {
public:
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	bool IsDead() const { return isDead_; }

	void SetPlayer(Player* player) { player_ = player; }

	void OnCollision();

	Vector3 GetWorldPosition();

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;

	// ���x
	Vector3 velocity_;

	// �^�C�}�[
	static const int32_t kLifeTime = 60 * 5;
	int32_t deathTimer_ = kLifeTime;
	bool isDead_ = false;

	Player* player_;
};
