#pragma once

#include "ParticleManager.h"
#include "Input.h"
#include "DirectXCommon.h"
#include "GameBaseScene.h"
#include "Sprite.h"
#include "GameSceneManager.h"
#include "GamePlayScene.h"

#include <DirectXMath.h>

class GameTitleScene : public GameBaseScene
{
public: // メンバ関数
	// コンストラクタ
	GameTitleScene();
	// デストラクタ
	~GameTitleScene();

public: // メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(SpriteCommon& spriteCommon) override;

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
	Sprite* title_ = nullptr;
	SpriteCommon spriteCommon_;

	DirectX::XMFLOAT3 titlePos_ = { 0,0,0 };
	DirectX::XMFLOAT2 titleScale_ = { 1280,720 };
	DirectX::XMFLOAT3 titleRot_ = { 0,0,0 };
};
