#pragma once

#include "ParticleManager.h"
#include "Input.h"
#include "DirectXCommon.h"
#include "GameBaseScene.h"
#include "Sprite.h"
#include "GameSceneManager.h"
#include "GameTitleScene.h"

#include <DirectXMath.h>

class GameClearScene : public GameBaseScene
{
public: // メンバ関数
	// コンストラクタ
	GameClearScene();
	// デストラクタ
	~GameClearScene();

public: // メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="dXCommon"></param>
	void Draw() override;

	/// <summary>
	/// 解放
	/// </summary>
	void Finalize() override;

private: // メンバ変数
	ParticleManager* particleMan1_ = nullptr;
	Input* input_ = nullptr;
	DirectXCommon* dxCommon_ = nullptr;

	// スプライト
	Sprite* sprite_ = nullptr;
	Sprite* clear_ = nullptr;
	SpriteCommon spriteCommon_;

	DirectX::XMFLOAT3 clearPos_ = { 0,0,0 };
	DirectX::XMFLOAT2 clearScale_ = { 1280,720 };
	DirectX::XMFLOAT3 clearRot_ = { 0,0,0 };
};
