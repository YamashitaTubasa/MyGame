/**
 * @file GameTitleScene.h
 * @brief ゲームタイトルシーンのクラス
 * @author Yamashita Tubasa
 */

#pragma once

#include "DirectXCommon.h"
#include "Input.h"
#include "Sprite.h"
#include "Skydome.h"
#include "PostEffect.h"
#include "GameBaseScene.h"
#include "GameSceneManager.h"
#include "GamePlayScene.h"

#include <DirectXMath.h>

class GameTitleScene : public GameBaseScene
{
public: // エイリアス
	using XMFLOAT2 = DirectX::XMFLOAT2;

public:
	// スプライトの番号
	enum SpriteNum
	{
		titleNum = 1,
		spaceNum,
		inBlackNum,
		nowLoadingNum,
		dotZeroNum,
		dotOneNum,
		dotTwoNum
	};

public: // メンバ関数
	// コンストラクタ
	GameTitleScene();

	// デストラクタ
	~GameTitleScene();

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
	/// スプライトの読み込み
	/// </summary>
	void LoadSprite();

	/// <summary>
	/// フェードイン・アウトの処理
	/// </summary>
	void Fade();

	/// <summary>
	/// SPACEの点滅処理
	/// </summary>
	void FlashSpace();

	/// <summary>
	/// ロード時の処理
	/// </summary>
	void Load();

	/// <summary>
	/// 自機の回転処理
	/// </summary>
	void PlayerRotation();

	/// <summary>
	/// シーンの切り替え処理
	/// </summary>
	void SceneChange();

public:
	/// <summary>
	/// ブラーの取得
	/// </summary>
	/// <returns></returns>
	bool GetIsBlur() const { return isBlur_; }

private: // メンバ変数
	MyEngine::Input* input_ = nullptr;
	MyEngine::DirectXCommon* dxCommon_ = nullptr;
	
	// スプライト
	std::unique_ptr<MyEngine::Sprite> sprite_ = nullptr;
	std::unique_ptr<MyEngine::Sprite> title_ = nullptr;
	std::unique_ptr<MyEngine::Sprite> space_ = nullptr;
	std::unique_ptr<MyEngine::Sprite> inBlack_ = nullptr;
	std::unique_ptr<MyEngine::Sprite> outBlack_ = nullptr;
	std::unique_ptr<MyEngine::Sprite> nowLoading_ = nullptr;
	std::unique_ptr<MyEngine::Sprite> dot_[5]{};

	// 天球
	std::unique_ptr<Skydome> skydome_ = nullptr;

	// オブジェクト
	std::unique_ptr<Object3d> player_ = nullptr;
	// モデル
	std::unique_ptr<Model> playerModel_ = nullptr;

	std::unique_ptr<MyEngine::PostEffect> postEffect_ = nullptr;

	// タイトルの設定
	DirectX::XMFLOAT4 titleColor_ = { 1,1,1,1 };
	DirectX::XMFLOAT3 titlePos_ = { 120, 80, 0 };
	DirectX::XMFLOAT2 titleScale_ = { 988.0f * 1.1f, 237.0f * 1.1f };
	float titleRot_ = 0.0f;

	// スペースの設定
	DirectX::XMFLOAT4 spaceColor_ = { 1,1,1,1 };
	DirectX::XMFLOAT3 spacePos_ = { 430,520,0 };
	DirectX::XMFLOAT2 spaceScale_ = { 415.0f, 94.0f };
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

	// ロードの設定
	DirectX::XMFLOAT4 nowLoadingColor_ = { 1,1,1,1 };
	DirectX::XMFLOAT3 nowLoadingPos_ = { 700, 590, 0 };
	DirectX::XMFLOAT2 nowLoadingScale_ = { 442.0f * 1.1f, 110 * 1.1f };
	bool isLoding_ = false;
	float lodingAlpha_ = 1.0f;

	// 点の設定
	std::vector<DirectX::XMFLOAT4> dotColor_ { {1, 1, 1, 1}, { 1,1,1,1 }, { 1,1,1,1 } };
	std::vector<DirectX::XMFLOAT3> dotPos_ = { { 1170, 650, 0 }, { 1190, 650, 0 }, { 1210, 650, 0 } };
	std::vector<DirectX::XMFLOAT2> dotScale_ = { { 30, 30 },{ 30,30 },{ 30,30 } };
	std::vector<float> dotAlpha_ = { {1.0f},{1.0f},{1.0f} };
	bool isDot_[5] = { false, false, false, false, false };
	int dotTimer_ = 0;
	const int dotTimerMin_ = 0;
	const int dotTimerOne_ = 40;
	const int dotTimerTwo_ = 70;
	const int dotTimerThree_ = 100;
	const int dotTimerMax_ = 140;
	const int defaultDotTimer_ = 0;
	const int dotMax_ = 3;

	// フェード
	DirectX::XMFLOAT4 inBlackColor_ = { 1,1,1,1 };
	DirectX::XMFLOAT3 inBlackPosition_ = { 0,0,0 };
	DirectX::XMFLOAT2 inBlackScale_ = { 1280,720 };

	DirectX::XMFLOAT3 pPos_ = { 0,-5,-30 };
	DirectX::XMFLOAT3 pRot_ = { 0,0,0 };
	DirectX::XMFLOAT3 pScale_ = { 5,5,5 };
	DirectX::XMFLOAT3 eye_ = { 0,0,-50 };

	// タイマー
	int startTimer_ = 0;
	const int defaultStartTimer_ = 0;
	const int MaxStartTimer_ = 200;

	// フラグ
	bool start_ = false;
	bool isPos_ = false;

	// フェードイン・アウト時ののアルファ値
	float bInAlpha_ = 1.0f;
	float bInAlphaMax_ = 1.0f;

	// フェードイン・アウトフラグ
	bool isFadeIn_ = false;
	bool isFadeOut_ = true;
	const float bFadeMax_ = 1.0f;
	const float bFadeMin_ = 0.0f;
	const float bFadeMove_ = 0.01f;

	// アンカーポイント
	const DirectX::XMFLOAT2 defaultAnchorpoint_ = { 0.0f,0.0f };
	const float defaultAnchorpointX_ = 0.0f;
	const float defaultAnchorpointY_ = 0.0f;

	// イージング変数
	bool isEaseFlag_ = false;
	float eTime_ = 0.0f;
	float eFrame_ = 0.0f;
	const float defaultFrame_ = 0.0f;
	const float eEndFrame_ = 80.0f;
	const float eStart_ = 0.0f;
	const float eEnd_ = 360.0f;
	const float eStartPos_ = 0.0f;
	const float eEndPos_ = 360.0f;
	const float eTotalTime_ = 60.0f;
	// 回転のフラグ
	bool isRot_ = false;

	// プレイヤー
	const float playerMaxRotation_ = 360.0f;
	const float playerMinRotation_ = 0.0f;

	// 自機の上下移動
	const float playerPosYMin_ = -4.0f;
	const float playerPosYMax_ = -7.0f;
	const float playerPosYMove_ = 0.03f;

	// スタート
	const float sPlayerPosMoveY_ = 0.25f;
	const float sPlayerPosMoveZ_ = 1.8f;
	const float sPlayerEyeMove_ = 0.06f;

	// ポストエフェクト（ブラー）
	bool isBlur_ = false;
};
