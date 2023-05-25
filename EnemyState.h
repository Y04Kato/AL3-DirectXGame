#pragma once
#include "Enemy.h"

class Enemy;

class EnemyState {
public:
	virtual ~EnemyState(){};

	virtual void Update(Enemy* enemy) = 0;
};

class EnemyApproach : public EnemyState {
public:
	void Update(Enemy* enemy);

	const Vector3 kCharacterApproachSpeed = {0.0f, 0.0f, -0.5f};
};

class EnemyLeave : public EnemyState {
public:
	void Update(Enemy* enemy);

	const Vector3 kCharacterLeaveSpeed = {-0.5f, 0.5f, 0.0f};
};