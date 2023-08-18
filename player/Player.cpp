#include "Player.h"
#include <cassert>

void Player::Initialize(Model* model) {
	//NULL�|�C���^�`�F�b�N
	assert(model);

	model_ = model;

	worldTransform_.Initialize();
}

void Player::Update() {
	//�s���萔�o�b�t�@�ɓ]��
	worldTransform_.TransferMatrix();
}

void Player::Draw(ViewProjection viewProjection) {
	//3D���f����`��
	model_->Draw(worldTransform_, viewProjection);
}