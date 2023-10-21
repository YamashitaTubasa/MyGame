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

private:
	/// <summary>
	/// SPACEの点滅処理
	/// </summary>
	void FlashSpace();

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
	DirectX::XMFLOAT3 overPos_ = { 150,100,0 };
	DirectX::XMFLOAT2 overScale_ = { 639.0f * 1.5f, 150.0f * 1.5f };
	DirectX::XMFLOAT3 overRot_ = { 0,0,0 };

	// スペースの設定
	DirectX::XMFLOAT4 spaceColor_ = { 1,1,1,1 };
	DirectX::XMFLOAT3 spacePos_ = { 420,520,0 };
	DirectX::XMFLOAT2 spaceScale_ { 505, 93 };
	float spaceRot_ = 0.0f;
	float sAlpha_ = 1.0f;
	int spaceTimer_ = 0;
	bool isSpace_ = true;

	// タイマー
	int timer_ = 0;

	// アルファ値
	float bAlpha_ = 1.0f;

	// フェードアウト・インのフラグ
	bool isFadeIn_ = false;
	bool isFadeOut_ = true;

	// シーンフラグ
	bool isScene_ = false;
};

