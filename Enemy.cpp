#include "Enemy.h"
#include <cassert>
#include "Affin.h"

void Enemy::Initialize(Model* model, const Vector3& position) {
	assert(model);

	model_ = model;
	// �e�N�X�`���ǂݍ���
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
		//�ړ�
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

	// ���[���h�g�����X�t�H�[���̍X�V
	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	// ���f���̕`��
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}