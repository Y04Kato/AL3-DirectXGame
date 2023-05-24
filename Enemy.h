#pragma once
#include "Model.h"
#include "WorldTransform.h"

enum class Phase {
	Approach, // �ڋ߂���
	Leave,    // ���E����
};

class Enemy {
public:
	void Initialize(Model* model, const Vector3& position);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	// ���상���o�֐�
	void Approach();

	void Leave();

private:
	// �����o�֐��|�C���^�̃e�[�u��
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