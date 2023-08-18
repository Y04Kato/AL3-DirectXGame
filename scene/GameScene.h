#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <memory>
#include "player/Player.h"
#include "skydome/Skydome.h"
#include "ground/Ground.h"
#include "DebugCamera.h"

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

	std::unique_ptr<Model> playerModel_ = nullptr;
	std::unique_ptr<Model> skydomeModel_ = nullptr;
	std::unique_ptr<Model> groundModel_ = nullptr;

	WorldTransform worldTransform_;
	ViewProjection viewProjection_;

	std::unique_ptr<Player> player_;
	std::unique_ptr<Skydome> skydome_;
	std::unique_ptr<Ground> ground_;

	bool isDebugCameraActive_ = false;
	std::unique_ptr<DebugCamera> debugCamera_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
