#include "WorldTransform.h"
#include "Affin.h"

void WorldTransform::UpdateMatrix() {

	matWorld_ = MakeAffineMatrix(scale_,rotation_,translation_);

	// �s���萔�o�b�t�@�ɓ]��
	TransferMatrix();
}