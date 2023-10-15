/**
 * @file GameClearScene.h
 * @brief クリアシーンのクラス
 * @author Yamashita Tubasa
 */

#pragma once

#include "DirectXCommon.h"
#include "Input.h"
#include "Sprite.h"
#include "Skydome.h"
#include "GameBaseScene.h"
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
	Input* input_ = nullptr;
	DirectXCommon* dxCommon_ = nullptr;

	// スプライト
	Sprite* sprite_ = nullptr;
	Sprite* clear_ = nullptr;
	Sprite* space_ = nullptr;
	SpriteCommon spriteCommon_;

	// 天球
	Skydome* skydome_ = nullptr;

	// クリアの設定
	DirectX::XMFLOAT3 clearPos_ = { 190,150,0 };
	DirectX::XMFLOAT2 clearScale_ = { 950,230 };
	DirectX::XMFLOAT3 clearRot_ = { 0,0,0 };

	// スペースの設定
	DirectX::XMFLOAT4 spaceColor_ = { 1,1,1,1 };
	DirectX::XMFLOAT3 spacePos_ = { 480,500,0 };
	DirectX::XMFLOAT2 spaceScale_ {330, 43};

	// フラグ
	bool isSpace = true;

	// タイマー
	int spaceTimer = 0;
};
