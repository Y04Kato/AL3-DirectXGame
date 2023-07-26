#include "Player.h"
#include "Affin.h"
#include "ImGuiManager.h"
#include <Sprite.h>
#include <cassert>

Player::Player() {}

Player::~Player() {
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
	delete sprite2DReticle_;
}

void Player::Initialize(Model* model, uint32_t textureHandle, Vector3 pos) {
	// NULL�|�C���^�`�F�b�N
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();

	input_ = Input::GetInstance();

	fireTimer = 30;

	SetCollisionAttribute(CollisionConfig::kCollisionAttributePlayer);
	SetCollisionMask(~CollisionConfig::kCollisionAttributePlayer);
	worldTransform_.translation_ = Add(worldTransform_.translation_, pos);
	worldtransform3Dreticle_.Initialize();

	uint32_t textureReticle = TextureManager::Load("Reticle.png");

	sprite2DReticle_ =
	    Sprite::Create(textureReticle, {640.0f, 360.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f});
}

void Player::Update(ViewProjection viewProjection) {
	// �f�X�t���O�̗������e�̍폜
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	Vector3 move = {0, 0, 0};
	const float kCharacterSpeed = 0.2f;
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	}
	if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}
	if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	}
	if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	}

	XINPUT_STATE joystate;
	if (Input::GetInstance()->GetJoystickState(0, joystate)) {
		move.x += (float)joystate.Gamepad.sThumbLX / SHRT_MAX * kCharacterSpeed;
		move.y += (float)joystate.Gamepad.sThumbLY / SHRT_MAX * kCharacterSpeed;
	}

	// ���񏈗�
	Rotate();

	// �U������
	Attack();

	// �e�X�V
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}

	// Gui�ɂ��ړ�
	float inputFloat3[3] = {
	    worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z};
	ImGui::Begin("Player");
	ImGui::Text("debugCamera = Q");
	ImGui::SliderFloat3("Player", inputFloat3, -40.0f, 40.0f);
	ImGui::End();
	worldTransform_.translation_.x = inputFloat3[0];
	worldTransform_.translation_.y = inputFloat3[1];
	worldTransform_.translation_.z = inputFloat3[2];

	worldTransform_.translation_ = VectorMultiply(worldTransform_.translation_, move);
	worldTransform_.UpdateMatrix();

	SetReticle(viewProjection);
}

void Player::Rotate() {
	// ��]����
	const float kRotSpeed = 0.02f;

	// �����������ňړ��x�N�g���ύX
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y += kRotSpeed;
	} else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	}
}

void Player::Attack() {
	fireTimer--;

	if (fireTimer <= 0) {
		if (input_->TriggerKey(DIK_SPACE)) {
			// �e�̑��x
			const float kBulletSpeed = 1.0f;
			Vector3 velocity(0, 0, kBulletSpeed);

			// ���x�x�N�g�������@�ɍ��킹�ĉ�]������
			// velocity = TransformNormal(velocity, worldTransform_.matWorld_);

			velocity = Subtract(
			    {worldtransform3Dreticle_.matWorld_.m[3][0],
			     worldtransform3Dreticle_.matWorld_.m[3][1],
			     worldtransform3Dreticle_.matWorld_.m[3][2]},
			    GetWorldPosition());
			velocity = Multiply(kBulletSpeed, Normalise(velocity));

			// �e�𐶐���������
			PlayerBullet* newBullet = new PlayerBullet();
			newBullet->Initialize(model_, GetWorldPosition(), velocity);

			// �e��o�^����
			bullets_.push_back(newBullet);
			fireTimer = 30;
		}

		XINPUT_STATE joystate;
		if (!Input::GetInstance()->GetJoystickState(0, joystate)) {
			return;
		}

		if (joystate.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
			const float kBulletSpeed = 1.0f;
			Vector3 velocity(0, 0, kBulletSpeed);

			// ���x�x�N�g�������@�ɍ��킹�ĉ�]������
			// velocity = TransformNormal(velocity, worldTransform_.matWorld_);

			velocity = Subtract(
			    {worldtransform3Dreticle_.matWorld_.m[3][0],
			     worldtransform3Dreticle_.matWorld_.m[3][1],
			     worldtransform3Dreticle_.matWorld_.m[3][2]},
			    GetWorldPosition());
			velocity = Multiply(kBulletSpeed, Normalise(velocity));

			// �e�𐶐���������
			PlayerBullet* newBullet = new PlayerBullet();
			newBullet->Initialize(model_, GetWorldPosition(), velocity);

			// �e��o�^����
			bullets_.push_back(newBullet);
			fireTimer = 30;
		}
	}
}

void Player::OnCollision() {}

void Player::Draw(ViewProjection viewProjection) {
	// 3D���f����`��
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	// �e�`��
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Player::DrawUI() { sprite2DReticle_->Draw(); }

Vector3 Player::GetWorldPosition() {
	Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

void Player::Setparent(const WorldTransform* parent) { worldTransform_.parent_ = parent; }

void Player::SetReticle(const ViewProjection viewProjection) {
	const float kDistancePlayerTo3DReticle = 50.0f;
	Vector3 offset = {0, 0, 1.0f};

	offset = TransformNormal(offset, worldTransform_.matWorld_);
	offset = Multiply(kDistancePlayerTo3DReticle, Normalise(offset));

	worldtransform3Dreticle_.translation_ = Add(GetWorldPosition(), offset);
	worldtransform3Dreticle_.UpdateMatrix();

	Vector3 positionReticle = {
	    worldtransform3Dreticle_.matWorld_.m[3][0], worldtransform3Dreticle_.matWorld_.m[3][1],
	    worldtransform3Dreticle_.matWorld_.m[3][2]};

	Matrix4x4 matViewport =
	    MakeViewportMatrix(0.0f, 0.0f, WinApp::kWindowWidth, WinApp::kWindowHeight, 0.0f, 1.0f);

	Matrix4x4 matViewProjectionViewport =
	    Multiply(Multiply(viewProjection.matView, viewProjection.matProjection), matViewport);
	positionReticle = Transform(positionReticle, matViewProjectionViewport);
	// sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));

	// �}�E�X�̏ꍇ
	// POINT mousePosition;

	// �}�E�X���W�i�X�N���[�����W�j���擾����
	// GetCursorPos(&mousePosition);

	// �N���C�A���g�G���A���W�ɕϊ�����
	// HWND hwnd = WinApp::GetInstance()->GetHwnd();
	// ScreenToClient(hwnd, &mousePosition);
	// sprite2DReticle_->SetPosition({(float)mousePosition.x, (float)mousePosition.y});

	// �L�[�{�[�h�̏ꍇ
	XINPUT_STATE joystate;
	Vector2 spritePostition = sprite2DReticle_->GetPosition();
	if (Input::GetInstance()->GetJoystickState(0, joystate)) {
		spritePostition.x += (float)joystate.Gamepad.sThumbRX / SHRT_MAX * 5.0f;
		spritePostition.y -= (float)joystate.Gamepad.sThumbRY / SHRT_MAX * 5.0f;
		// �X�v���C�g�̍��W�ύX�𔽉f
		sprite2DReticle_->SetPosition(spritePostition);
	}

	Matrix4x4 matVPV =
	    Multiply(Multiply(viewProjection.matView, viewProjection.matProjection), matViewport);
	Matrix4x4 matInverseVPV = Inverse(matVPV);
	Vector3 posNear = {
	    (float)sprite2DReticle_->GetPosition().x, (float)sprite2DReticle_->GetPosition().y, 0};
	Vector3 posFar = {
	    (float)sprite2DReticle_->GetPosition().x, (float)sprite2DReticle_->GetPosition().y, 1};
	posNear = Transform(posNear, matInverseVPV);
	posFar = Transform(posFar, matInverseVPV);
	Vector3 mouseDirection = Subtract(posFar, posNear);
	mouseDirection = Normalise(mouseDirection);
	const float kDistancetestObject = 100.0f;
	worldtransform3Dreticle_.translation_ =
	    Add(posNear, Multiply(kDistancetestObject, mouseDirection));
	worldtransform3Dreticle_.UpdateMatrix();
	ImGui::Begin("Player");
	ImGui::Text("Near:(%+.2f,%+.2f,%.2f)", posNear.x, posNear.y, posNear.z);
	ImGui::Text("Far:(%+.2f,%+.2f,%.2f)", posFar.x, posFar.y, posFar.z);
	ImGui::Text(
	    "3DRetixle:(%+.2f,%+.2f,%.2f)", worldtransform3Dreticle_.translation_.x,
	    worldtransform3Dreticle_.translation_.y, worldtransform3Dreticle_.translation_.z);
	ImGui::End();
}