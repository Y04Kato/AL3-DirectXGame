#pragma once
#include "Affin.h"
#include "EnemyBullet.h"
#include "EnemyState.h"
#include "Model.h"
#include "WorldTransform.h"
#include "utilities/TimedCall.h"
#include "CalcMath.h"
#include "utilities/Collider.h"
#include "utilities/CollisionConfig.h"

class Player;

class GameScene;

class EnemyState;

class Enemy : public Collider {
public:
	Enemy();

	~Enemy();

	void Initialize(Model* model, Vector3 pos, const Vector3& velocity);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	void Move(Vector3 speed);

	void ChangePhase(EnemyState* newState);

	Vector3 GetTranslation() { return worldTransform_.translation_; };

	void Fire();

	void FireandReset();

	void SetPlayer(Player* player) { player_ = player; }

	Vector3 GetWorldPosition() override;

	void OnCollision() override;

	//const std::list<EnemyBullet*>& GetBullets() const { return bullets_; }

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

private:
	// メンバ関数ポインタのテーブル
	static void (Enemy::*phasetable_[])();

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;

	Player* player_ = nullptr;

	GameScene* gameScene_ = nullptr;

	//std::list<EnemyBullet*> bullets_;

	EnemyState* phase_ = nullptr;

	Vector3 velocity_;

	std::list<TimedCall*> timedCalls_;

public:
	static const int kFireInterval = 60;

	int32_t FireTimer_ = 0;
};