#include "Player.h"
#include "Affin.h"
#include "ImGuiManager.h"
#include <cassert>
#include <Sprite.h>

Player::Player() {

}

Player::~Player() {
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
	delete sprite2DReticle_;
}

void Player::Initialize(Model* model, uint32_t textureHandle, Vector3 pos) {
	// NULLƒ|ƒCƒ“ƒ^ƒ`ƒFƒbƒN
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();

	input_ = Input::GetInstance();

	SetCollisionAttribute(CollisionConfig::kCollisionAttributePlayer);
	SetCollisionMask(~CollisionConfig::kCollisionAttributePlayer);
	worldTransform_.translation_ = Add(worldTransform_.translation_, pos);
	worldtransform3Dreticle_.Initialize();

	uint32_t textureReticle = TextureManager::Load("Reticle.png");

	sprite2DReticle_ =
	    Sprite::Create(textureReticle, {640.0f, 360.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f});

}

void Player::Update(ViewProjection viewProjection) {
	//ƒfƒXƒtƒ‰ƒO‚Ì—§‚Á‚½’e‚Ìíœ
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

	worldTransform_.translation_ = VectorMultiply(worldTransform_.translation_, move);

	// ù‰ñˆ—
	Rotate();

	// UŒ‚ˆ—
	Attack();

	// ’eXV
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}

	// Gui‚É‚æ‚éˆÚ“®
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

	// ˆÚ“®§ŒÀ
	const float kMoveLimitX = 33;
	const float kMoveLimitY = 18;

	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	worldTransform_.UpdateMatrix();

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
	sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));
}

void Player::Rotate() {
	// ‰ñ“]‘¬‚³
	const float kRotSpeed = 0.02f;

	// „‚µ‚½•ûŒü‚ÅˆÚ“®ƒxƒNƒgƒ‹•ÏX
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y += kRotSpeed;
	} else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	}
}

void Player::Attack() {
	if (input_->TriggerKey(DIK_SPACE)) {
		//’e‚Ì‘¬“x
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		//‘¬“xƒxƒNƒgƒ‹‚ðŽ©‹@‚É‡‚í‚¹‚Ä‰ñ“]‚³‚¹‚é
		//velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		velocity = Subtract(
		    {worldtransform3Dreticle_.matWorld_.m[3][0], worldtransform3Dreticle_.matWorld_.m[3][1],
		     worldtransform3Dreticle_.matWorld_.m[3][2]},
		    GetWorldPosition());
		velocity = Multiply(kBulletSpeed, Normalise(velocity));

		// ’e‚ð¶¬‚µ‰Šú‰»
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, GetWorldPosition(), velocity);

		// ’e‚ð“o˜^‚·‚é
		bullets_.push_back(newBullet);
	}
}

void Player::OnCollision() {

}

void Player::Draw(ViewProjection viewProjection) {
	// 3Dƒ‚ƒfƒ‹‚ð•`‰æ
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	// ’e•`‰æ
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