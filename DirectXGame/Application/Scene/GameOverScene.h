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

public:
	// スプライトの番号
	enum GameOverSpriteNum
	{
		blackNum,
		overNum,
		spaceNum,
		fBlackNum
	};

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

	/// <summary>
	/// フェードの処理
	/// </summary>
	void Fade();

	/// <summary>
	/// シーンの切り替え処理
	/// </summary>
	void SceneChange();

	/// <summary>
	/// スプライトの読み込み
	/// </summary>
	void LoadSprite();

private: // メンバ変数
	Input* input_ = nullptr;
	DirectXCommon* dxCommon_ = nullptr;

	// スプライト
	std::unique_ptr<Sprite> sprite_ = nullptr;
	std::unique_ptr<Sprite> over_ = nullptr;
	std::unique_ptr<Sprite> space_ = nullptr;
	std::unique_ptr<Sprite> black_ = nullptr;
	std::unique_ptr<Sprite> fBlack_ = nullptr;
	SpriteCommon spriteCommon_;

	// 天球
	std::unique_ptr<Skydome> skydome_ = nullptr;

	// ゲームオーバーの設定
	DirectX::XMFLOAT4 overColor_ = { 1,1,1,1 };
	DirectX::XMFLOAT3 overPos_ = { 150,100,0 };
	DirectX::XMFLOAT2 overScale_ = { 639.0f * 1.5f, 150.0f * 1.5f };
	DirectX::XMFLOAT3 overRot_ = { 0,0,0 };
	DirectX::XMFLOAT2 overAnchorpoint_ = { 0.0f,0.0f };

	// スペースの設定
	DirectX::XMFLOAT4 spaceColor_ = { 1,1,1,1 };
	DirectX::XMFLOAT3 spacePos_ = { 430,520,0 };
	DirectX::XMFLOAT2 spaceScale_ { 415, 96 };
	DirectX::XMFLOAT2 spaceAnchorpoint_ = { 0.0f,0.0f };
	float spaceRot_ = 0.0f;
	float sAlpha_ = 1.0f;
	const float sAlphaMin_ = 0.0f;
	const float sAlphaMax_ = 1.0f;
	const float sAlphaMove_ = 0.05f;
	int spaceTimer_ = 0;
	const int spaceTimerMin_ = 0;
	const int spaceTimerOne_ = 50;
	const int spaceTimerTwo_ = 100;
	const int spaceTimerMax_ = 150;
	bool isSpace_ = true;

	// Blackの設定
	DirectX::XMFLOAT4 blackColor_ = { 1,1,1,1 };
	DirectX::XMFLOAT3 blackPos_ = { 0,0,0 };
	DirectX::XMFLOAT2 blackScale_ = { 1280,720 };
	DirectX::XMFLOAT2 blackAnchorpoint_ = { 0.0f,0.0f };

	// FBlackの設定
	DirectX::XMFLOAT4 fBlackColor_ = { 1,1,1,1 };
	DirectX::XMFLOAT3 fBlackPos_ = { 0,0,0 };
	DirectX::XMFLOAT2 fBlackScale_ = { 1280,720 };
	DirectX::XMFLOAT2 fBlackAnchorpoint_ = { 0.0f,0.0f };

	// タイマー
	int timer_ = 0;
	const int changeSceneTimer_ = 200;

	// アルファ値
	float fBlackAlpha_ = 1.0f;

	// フェードアウト・インのフラグ
	bool isFadeIn_ = false;
	bool isFadeOut_ = true;
	const float fBlackFadeMax_ = 1.0f;
	const float fBlackFadeMin_ = 0.0f;
	const float fBlackFadeMove_ = 0.01f;

	// シーンフラグ
	bool isScene_ = false;
};

