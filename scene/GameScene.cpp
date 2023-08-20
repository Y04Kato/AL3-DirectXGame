#include "GameScene.h"
#include "AxisIndicator.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	playerModelBody_.reset(Model::CreateFromOBJ("float_Body", true));
	playerModelHead_.reset(Model::CreateFromOBJ("float_Head", true));
	playerModelL_Arm_.reset(Model::CreateFromOBJ("float_L_arm", true));
	playerModelR_Arm_.reset(Model::CreateFromOBJ("float_R_arm", true));

	playerModelWeapon_.reset(Model::CreateFromOBJ("weapon", true));

	enemyModel_.reset(Model::CreateFromOBJ("enemy", true));

	skydomeModel_.reset(Model::CreateFromOBJ("skyDome", true));

	groundModel_.reset(Model::CreateFromOBJ("ground", true));

	// 軸方向表示の表示を有効化
	AxisIndicator::GetInstance()->SetVisible(true);
	// 参照するビュープロジェクションを指定
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	worldTransform_.Initialize();
	viewProjection_.Initialize();

	debugCamera_ = std::make_unique<DebugCamera>(1280, 720);

	// 自キャラの生成
	player_ = std::make_unique<Player>();
	// 自キャラの初期化
	std::vector<Model*> playerModels = {
	    playerModelBody_.get(), playerModelHead_.get(), playerModelL_Arm_.get(),
	    playerModelR_Arm_.get(), playerModelWeapon_.get()};

	player_->Initialize(playerModels);

	enemy_ = std::make_unique<Enemy>();
	std::vector<Model*> enemyModels = {enemyModel_.get()};
	enemy_->Initialize(enemyModels);

	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(skydomeModel_.get());

	ground_ = std::make_unique<Ground>();
	ground_->Initialize(groundModel_.get());

	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();
	followCamera_->SetTarget(&player_->GetWorldtransformBase());
	player_->SetViewProjection(&followCamera_->GetViewProjection());
}

void GameScene::Update() {
	// 自キャラの更新
	player_->Update();

	enemy_->Update();

	debugCamera_->Update();

#ifdef _DEBUG
	if (input_->TriggerKey(DIK_Q)) {
		if (isDebugCameraActive_ == false) {
			isDebugCameraActive_ = true;
		} else {
			isDebugCameraActive_ = false;
		}
	}
#endif
	// カメラの処理
	if (isDebugCameraActive_) {
		// デバッグカメラの更新
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	} else {
		// ビュープロジェクション行列の更新と転送
		viewProjection_.TransferMatrix();
	}
	followCamera_->Update();
	viewProjection_.matView = followCamera_->GetViewProjection().matView;
	viewProjection_.matProjection = followCamera_->GetViewProjection().matProjection;
	viewProjection_.TransferMatrix();

	ImGui::Begin("Debug Info");
	ImGui::Text("Debug Camera : Q");
	ImGui::End();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	player_->Draw(viewProjection_);
	enemy_->Draw(viewProjection_);
	skydome_->Draw(viewProjection_);
	ground_->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
