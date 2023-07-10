#include "Affin.h"
#include "EnemyBullet.h"
#include <cassert>
#include "Player.h"

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	assert(model);

	model_ = model;
	// �e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("white.png");

	worldTransform_.Initialize();

	worldTransform_.translation_ = position;

	worldTransform_.scale_ = {0.5f, 0.5f, 3.0f};

	velocity_ = velocity;

	//Y���p�x
	worldTransform_.rotation_.y = std::atan2(velocity_.x, velocity_.z);
	float velociteXZ = Length({velocity_.x, 0.0f, velocity_.z});
	//X���p�x
	worldTransform_.rotation_.x = std::atan2(-velocity_.y, velociteXZ);
}

void EnemyBullet::Update() {
	Vector3 toPlayer = Subtract(player_->GetWorldPosition(), worldTransform_.translation_);
	toPlayer = Normalise(toPlayer);
	velocity_ = Normalise(velocity_);
	velocity_ = Slerp(0.1f, velocity_, toPlayer);

	// Y�����
	worldTransform_.rotation_.y = std::atan2(velocity_.x, velocity_.z);
	float velociteXZ = Length({velocity_.x, 0.0f, velocity_.z});
	// X�����
	worldTransform_.rotation_.x = std::atan2(-velocity_.y, velociteXZ);


	//worldTransform_.translation_ = VectorMultiply(worldTransform_.translation_, velocity_);

	// ���Ԍo�߂ŏ���
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	// ���[���h�g�����X�t�H�[���̍X�V
	worldTransform_.UpdateMatrix();
}

void EnemyBullet::Draw(const ViewProjection& viewProjection) {
	// ���f���̕`��
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}