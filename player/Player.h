#pragma once
#include "CalcMath.h"
#include "Input.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class Player {
public:
	void Initialize(Model* modelBody, Model* modelHead, Model* modelL_arm, Model* modelR_arm);

	void Update();

	void Draw(ViewProjection viewProjection);

	const WorldTransform& GetWorldtransform() { return worldTransform_; }
	void SetViewProjection(const ViewProjection* viewProjection) {
		viewProjection_ = viewProjection;
	}

private:
	WorldTransform worldTransform_;
	Model* modelBody_ = nullptr;
	Model* modelHead_ = nullptr;
	Model* modelL_arm_ = nullptr;
	Model* modelR_arm_ = nullptr;

	Input* input_ = nullptr;

	const ViewProjection* viewProjection_ = nullptr;
};
