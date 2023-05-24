#include "Enemy.h"
#include "Affin.h"
#include <cassert>

void Enemy::Initialize(Model* model, const Vector3& position) {
	assert(model);

	model_ = model;
	// �e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("black.png");

	worldTransform_.Initialize();

	worldTransform_.translation_ = position;

	move_ = {0, 0, 0};
	kCharacterApproachSpeed_ = 0.001f;
	kCharacterLeaveSpeed_ = 0.005f;

	static_cast<size_t>(phase_);
}

void Enemy::Update() {

	(this->*phasetable_[0])();

	worldTransform_.translation_ = VectorMultiply(worldTransform_.translation_, move_);

	// ���[���h�g�����X�t�H�[���̍X�V
	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	// ���f���̕`��
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void (Enemy::*Enemy::phasetable_[])() = {
    &Enemy::Approach, // �ڋ�
    &Enemy::Leave     // ���E
};

void Enemy::Approach() {
	// �ړ�
	move_.z -= kCharacterApproachSpeed_;
	if (worldTransform_.translation_.z < 0.0f) {
		(this->*phasetable_[1])();
	}
}

void Enemy::Leave() {
	move_.x -= kCharacterLeaveSpeed_;
	move_.y += kCharacterLeaveSpeed_;
}