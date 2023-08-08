#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "player/Player.h"
#include "DebugCamera.h"
#include "enemy/Enemy.h"
#include "utilities/CollisionManager.h"
#include "utilities/CollisionConfig.h"
#include "utilities/RailCamera.h"
#include "object/Skydome.h"
#include "utilities/CatmullRomSpline.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	uint32_t textureHandle_ = 0;
	Model* model_ = nullptr;

	WorldTransform worldTransform_;
	ViewProjection viewProjection_;

	Player* player_ = nullptr;

	// デバッグカメラ有効化
	bool isDebugCameraActive_ = false;

	// デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

	Enemy* enemy_ = nullptr;

	CollisionManager* collisionManager_;

	Skydome* skydome_;
	Model* skydomeModel_ = nullptr;

	RailCamera* railCamera_;
	CatmullRomSpline* Spline_;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
