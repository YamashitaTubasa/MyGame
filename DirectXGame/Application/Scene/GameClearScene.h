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
	Sprite* clear_ = nullptr;
	Sprite* space_ = nullptr;
	Sprite* black_ = nullptr;
	SpriteCommon spriteCommon_;

	// 天球
	Skydome* skydome_ = nullptr;

	// クリアの設定
	DirectX::XMFLOAT3 clearPos_ = { 150, 100, 0 };
	DirectX::XMFLOAT2 clearScale_ = { 695.0f * 1.5f, 152.0f * 1.5f };
	DirectX::XMFLOAT3 clearRot_ = { 0,0,0 };

	// スペースの設定
	DirectX::XMFLOAT4 spaceColor_ = { 1,1,1,1 };
	DirectX::XMFLOAT3 spacePos_ = { 420,520,0 };
	DirectX::XMFLOAT2 spaceScale_ { 443.0f, 86.0f };
	float spaceRot_ = 0.0f;
	float sAlpha_ = 1.0f;
	int spaceTimer = 0;
	bool isSpace = true;

	// タイマー
	int timer = 0;

	// アルファ値
	float bAlpha_ = 1.0f;

	// フェードアウト・インのフラグ
	bool isFadeIn = false;
	bool isFadeOut = true;

	// シーンフラグ
	bool isScene = false;
};
