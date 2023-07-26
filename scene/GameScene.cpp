#include "GameScene.h"
#include "AxisIndicator.h"
#include "TextureManager.h"
#include <cassert>
#include <fstream>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete player_;
	delete debugCamera_;
	for (Enemy* enemy : enemys_) {
		delete enemy;
	}
	delete collisionManager_;
	delete skydome_;
	delete skydomeModel_;
	delete railCamera_;
	for (EnemyBullet* bullet : enemyBullets_) {

		delete bullet;
	}
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	textureHandle_ = TextureManager::Load("sample.png");
	TextureManager::Load("Reticle.png");
	model_ = Model::Create();

	worldTransform_.Initialize();
	viewProjection_.Initialize();

	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	input_ = Input::GetInstance();

	// 軸方向表示の表示を有効化
	AxisIndicator::GetInstance()->SetVisible(true);
	// 参照するビュープロジェクションを指定
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	// 自キャラの生成
	player_ = new Player();
	// 自キャラの初期化
	Vector3 playerPos(0, 0, 30);
	player_->Initialize(model_, textureHandle_, playerPos);

	collisionManager_ = new CollisionManager();

	skydomeModel_ = Model::CreateFromOBJ("skydome", true);
	skydome_ = new Skydome();
	skydome_->Initialize(skydomeModel_);

	railCamera_ = new RailCamera();
	railCamera_->Initialize({0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f});
	player_->Setparent(&railCamera_->GetWorldTransform());

	LoadEnemyPopData();
}

void GameScene::Update() {
	// 自キャラの更新
	player_->Update(viewProjection_);

	// 敵キャラの更新
	UpdateEnemyPopCommands();
	for (Enemy* enemy : enemys_) {
		enemy->Update();
	}

	for (EnemyBullet* bullet : enemyBullets_) {

		bullet->Update();
	}

	collisionManager_->ClearColliders();
	collisionManager_->AddCollider(player_);
	for (Enemy* enemy : enemys_) {
		collisionManager_->AddCollider(enemy);
	}

	for (PlayerBullet* pBullet : player_->GetBullets()) {
		collisionManager_->AddCollider(pBullet);
	}

	for (EnemyBullet* eBullet : enemyBullets_) {
		collisionManager_->AddCollider(eBullet);
	}

	collisionManager_->CheckAllCollision();

	skydome_->Update();

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
		railCamera_->Update();
		viewProjection_.matView = railCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;
		// ビュープロジェクション行列の更新と転送
		viewProjection_.TransferMatrix();
	}
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
	for (Enemy* enemy : enemys_) {
		enemy->Draw(viewProjection_);
	}
	for (EnemyBullet* bullet : enemyBullets_) {

		bullet->Draw(viewProjection_);
	}
	player_->Draw(viewProjection_);
	skydome_->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	
	player_->DrawUI();

	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet) { enemyBullets_.push_back(enemyBullet); }

void GameScene::LoadEnemyPopData() {
	std::ifstream file;
	file.open("csv/enemyPop.csv");
	assert(file.is_open());

	enemyPopCommands << file.rdbuf();

	file.close();
}

void GameScene::UpdateEnemyPopCommands() {
	if (enemyPopWait == true) {
		enemyPopWaitTimer--;
		if (enemyPopWaitTimer <= 0) {
			enemyPopWait = false;
		}
		return;
	}

	std::string line;

	while (getline(enemyPopCommands, line)) {
		std::istringstream line_stream(line);

		std::string word;

		getline(line_stream, word, ',');

		if (word.find("//") == 0) {
			continue;
		}

		if (word.find("POP") == 0) {
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());
			EnemySpawn({x, y, z}, {0.0f, 0.0f, -0.5f});

		} else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');

			int32_t waitTime = atoi(word.c_str());
			enemyPopWait = true;
			enemyPopWaitTimer = waitTime;

			break;
		}
	}
}

void GameScene::EnemySpawn(Vector3 pos, Vector3 velocity) {
	Enemy* enemy = new Enemy();
	enemy->SetPlayer(player_);
	enemy->SetGameScene(this);
	enemy->Initialize(model_, pos, velocity);

	enemys_.push_back(enemy);
}