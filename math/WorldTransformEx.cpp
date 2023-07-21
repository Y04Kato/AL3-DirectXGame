#include "WorldTransform.h"
#include "Affin.h"

void WorldTransform::UpdateMatrix() {

	matWorld_ = MakeAffineMatrix(scale_,rotation_,translation_);

	if (parent_) {
		matWorld_ = Multiply(matWorld_, parent_->matWorld_);
	}

	// 行列を定数バッファに転送
	TransferMatrix();
}