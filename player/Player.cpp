#include "Player.h"
#include <cassert>

void Player::Initialize(Model* model) {
	//NULLポインタチェック
	assert(model);

	model_ = model;

	worldTransform_.Initialize();
}

void Player::Update() {
	//行列を定数バッファに転送
	worldTransform_.TransferMatrix();
}

void Player::Draw(ViewProjection viewProjection) {
	//3Dモデルを描画
	model_->Draw(worldTransform_, viewProjection);
}