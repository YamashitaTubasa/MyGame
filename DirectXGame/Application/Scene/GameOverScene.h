/**
 * @file GameOverScene.h
 * @brief ゲームオーバーシーンのクラス
 * @author Yamashita Tubasa
 */

#pragma once

#include "Input.h"
#include "DirectXCommon.h"
#include "Sprite.h"
#include "Skydome.h"
#include "GameBaseScene.h"
#include "GameSceneManager.h"
#include "GameTitleScene.h"

#include <DirectXMath.h>

class GameOverScene : public GameBaseScene
{
public: // メンバ関数
	// コンストラクタ
	GameOverScene();
	// デストラクタ
	~GameOverScene();

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
	Sprite* over_ = nullptr;
	Sprite* space_ = nullptr;
	Sprite* black_ = nullptr;
	Sprite* fBlack_ = nullptr;
	SpriteCommon spriteCommon_;

	// 天球
	Skydome* skydome_ = nullptr;

	// ゲームオーバーの設定
	DirectX::XMFLOAT3 overPos_ = { 190,150,0 };
	DirectX::XMFLOAT2 overScale_ = { 950,230 };
	DirectX::XMFLOAT3 overRot_ = { 0,0,0 };

	// スペースの設定
	DirectX::XMFLOAT4 spaceColor_ = { 1,1,1,1 };
	DirectX::XMFLOAT3 spacePos_ = { 480,500,0 };
	DirectX::XMFLOAT2 spaceScale_ {330, 41};

	// フラグ
	bool isSpace = true;

	// タイマー
	int spaceTimer = 0;
	int timer = 0;

	// アルファ値
	float bAlpha_ = 1.0f;

	// フェードアウト・インのフラグ
	bool isFadeIn = false;
	bool isFadeOut = true;

	// シーンフラグ
	bool isScene = false;
};

