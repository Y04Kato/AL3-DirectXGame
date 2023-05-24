#pragma once
#include "Model.h"
#include "WorldTransform.h"

enum class Phase {
	Approach, // 接近する
	Leave,    // 離脱する
};

class Enemy {
public:
	void Initialize(Model* model, const Vector3& position);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	// 自作メンバ関数
	void Approach();

	void Leave();

private:
	// メンバ関数ポインタのテーブル
	static void (Enemy::*phasetable_[])();

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;

	Vector3 move_;
	float kCharacterApproachSpeed_;
	float kCharacterLeaveSpeed_;

	Phase phase_ = Phase::Approach;
};