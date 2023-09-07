#pragma once
#include "CalcMath.h"
#include "Input.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "baseCharacter/BaseCharacter.h"
#include <optional>
#include "baseCharacter/bomb/Bomb.h"

class Player : public BaseCharacter{
public:
	void Initialize(const std::vector<Model*>& models) override;

	void Update() override;

	void Draw(const ViewProjection& viewProjection) override;

	const WorldTransform& GetWorldtransformBase() { return worldTransformBase_; }
	const WorldTransform& GetWorldTransformBody() { return worldTransformBody_; }

	void SetViewProjection(const ViewProjection* viewProjection) {
		viewProjection_ = viewProjection;
	}

	void InitializeFloatingGimmick();
	void UpdateFloatingGimmick();

	void BehaviorRootInitialize();
	void BehaviorRootUpdate();

	void BehaviorAttackInitialize();
	void BehaviorAttackUpdate();

	void ApplyGlobalVariables();

private:
	WorldTransform worldTransformBase_;
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformHead_;
	WorldTransform worldTransformL_arm_;
	WorldTransform worldTransformR_arm_;

	WorldTransform worldTransformWeapon_;

	Model* modelBody_ = nullptr;
	Model* modelHead_ = nullptr;
	Model* modelL_arm_ = nullptr;
	Model* modelR_arm_ = nullptr;
	Model* modelWeapon_ = nullptr;

	Input* input_ = nullptr;

	std::unique_ptr<Bomb> bomb = nullptr;

	const ViewProjection* viewProjection_ = nullptr;

	float floatingParameter_ = 0.0f;

	int32_t floatingCycle_ = 120;

	float floatingAmplitude_ = 0.3f;

	enum class Behavior { kRoot, kAttack };

	Behavior behavior_ = Behavior::kRoot;
	std::optional<Behavior> behaviorRequest_ = std::nullopt;

	int animationFrame;
};
