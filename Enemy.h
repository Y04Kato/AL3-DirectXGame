#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "EnemyState.h"
#include "EnemyBullet.h"
#include "Affin.h"

class EnemyState;

class Enemy {
public:
	Enemy();

	~Enemy();

	void Initialize(Model* model);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	void Move(Vector3 speed);

	void ChangePhase(EnemyState* newState);

	Vector3 GetTranslation() { return worldTransform_.translation_; };

	void Fire();

private:
	// メンバ関数ポインタのテーブル
	static void (Enemy::*phasetable_[])();

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;

	std::list<EnemyBullet*> bullets_;

	EnemyState* phase_ = nullptr;
};