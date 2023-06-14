#include "Enemy.h"
#include "Affin.h"
#include <cassert>

Enemy::Enemy() {}

Enemy::~Enemy() {
	// delete phase_;

	for (EnemyBullet* bullet : bullets_) {
		delete bullet;
	}
	for (TimedCall* timedCall : timedCalls_) {
		delete timedCall;
	}
}

void Enemy::Initialize(Model* model) {
	assert(model);

	model_ = model;

	// �e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("black.png");

	// �t�F�[�Y�J�n
	// phase_ = new EnemyApproach();

	worldTransform_.Initialize();

	worldTransform_.translation_ = {10, 0, 20};

	FireTimer_ = kFireInterval;
	FireandReset();
}

void Enemy::Update() {
	// �f�X�t���O�̗������e�̍폜
	bullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	// phase_->Update(this);
	
	//�^�C�}�[
	timedCalls_.remove_if([](TimedCall* timedcall) {
		if (timedcall->IsFinish()) {
			delete timedcall;
			return true;
		}
		return false;
	});
	for (TimedCall* timedCall : timedCalls_) {
		timedCall->Update();
	}


	// ���[���h�g�����X�t�H�[���̍X�V
	worldTransform_.UpdateMatrix();

	// �e�X�V
	for (EnemyBullet* bullet : bullets_) {
		bullet->Update();
	}
}

void Enemy::ChangePhase(EnemyState* newState) {
	delete phase_;
	phase_ = newState;
}

void Enemy::Move(Vector3 speed) { worldTransform_.translation_ += speed; };

void Enemy::Fire() {
	// �e�̑��x
	const float kBulletSpeed = -1.0f;
	Vector3 velocity(0, 0, kBulletSpeed);

	// ���x�x�N�g�������@�ɍ��킹�ĉ�]������
	velocity = TransformNormal(velocity, worldTransform_.matWorld_);

	// �e�𐶐���������
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	// �e��o�^����
	bullets_.push_back(newBullet);
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	// ���f���̕`��
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	// �e�`��
	for (EnemyBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Enemy::FireandReset() {
	// �U������
	Fire();

	// �^�C�}�[
	timedCalls_.push_back(new TimedCall(std::bind(&Enemy::FireandReset, this), kFireInterval));
}