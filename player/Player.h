#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "PlayerBullet.h"
#include "CalcMath.h"
#include <list>
#include "utilities/Collider.h"
#include "utilities/CollisionConfig.h"

class Player : public Collider{
public:

	Player();

	~Player();

	void Initialize(Model* model, uint32_t textureHandle, Vector3 pos);

	void Update();

	void Draw(ViewProjection viewProjection);

	void Rotate();

	void Attack();

	Vector3 GetWorldPosition() override;
	void OnCollision() override;

	const std::list<PlayerBullet*>& GetBullets() const { return bullets_; }

	void Setparent(const WorldTransform* parent);

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;

	Input* input_ = nullptr;

	std::list<PlayerBullet*> bullets_;
};
