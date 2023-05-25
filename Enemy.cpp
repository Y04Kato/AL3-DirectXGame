#include "Enemy.h"
#include "Affin.h"
#include <cassert>

Enemy::Enemy() {}

Enemy::~Enemy() { delete phase_; }

void Enemy::Initialize(Model* model) {
	assert(model);

	model_ = model;

	// �e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("black.png");

	//�t�F�[�Y�J�n
	phase_ = new EnemyApproach();

	worldTransform_.Initialize();

	worldTransform_.translation_ = {0, 0, 20};
}

void Enemy::Update() {
	phase_->Update(this);

	// ���[���h�g�����X�t�H�[���̍X�V
	worldTransform_.UpdateMatrix();
}

void Enemy::ChangePhase(EnemyState* newState) {
	delete phase_;
	phase_ = newState;
}

void Enemy::Move(Vector3 speed) {worldTransform_.translation_+=speed; };

void Enemy::Draw(const ViewProjection& viewProjection) {
	// ���f���̕`��
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}