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

public:
	enum SpriteNum
	{
		titleNum = 1,
		spaceNum,
		blackNum
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
	Sprite* sprite_ = nullptr;
	Sprite* clear_ = nullptr;
	Sprite* space_ = nullptr;
	Sprite* black_ = nullptr;
	SpriteCommon spriteCommon_;

	// 天球
	Skydome* skydome_ = nullptr;

	// クリアの設定
	DirectX::XMFLOAT4 clearColor_ = { 1,1,1,1 };
	DirectX::XMFLOAT3 clearPos_ = { 150, 100, 0 };
	DirectX::XMFLOAT2 clearScale_ = { 695.0f * 1.5f, 152.0f * 1.5f };
	DirectX::XMFLOAT3 clearRot_ = { 0,0,0 };

	// スペースの設定
	DirectX::XMFLOAT4 spaceColor_ = { 1,1,1,1 };
	DirectX::XMFLOAT3 spacePos_ = { 430,520,0 };
	DirectX::XMFLOAT2 spaceScale_ { 415.0f, 94.0f };
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

	// blackの設定
	DirectX::XMFLOAT4 blackColor_ = { 1,1,1,1 };
	DirectX::XMFLOAT3 blackPos_ = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT2 blackScale_ { 1280.0f,720.0f };

	// タイマー
	int timer_ = 0;
	const int changeSceneTimer_ = 200;

	// アルファ値
	float bAlpha_ = 1.0f;

	// フェードアウト・イン
	bool isFadeIn_ = false;
	bool isFadeOut_ = true;
	const float blackFadeMax_ = 1.0f;
	const float blackFadeMin_ = 0.0f;
	const float blackFadeMove_ = 0.01f;

	// シーンフラグ
	bool isScene_ = false;

	// アンカーポイント
	const DirectX::XMFLOAT2 defaultAnchorpoint_ = { 0.0f,0.0f };
};
