#pragma once
#include "CalcMath.h"
#include "Input.h"
#include "Model.h"
#include "PlayerBullet.h"
#include "WorldTransform.h"
#include "utilities/Collider.h"
#include "utilities/CollisionConfig.h"
#include <list>
#include <Sprite.h>

class Player : public Collider {
public:
	Player();

	~Player();

	void Initialize(Model* model, uint32_t textureHandle, Vector3 pos);

	void Update(ViewProjection viewProjection);

	void Draw(ViewProjection viewProjection);

	void Rotate();

	void Attack();

	void DrawUI();

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

	WorldTransform worldtransform3Dreticle_;
	Sprite* sprite2DReticle_ = nullptr;
};
