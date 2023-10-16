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
#include "GameBaseScene.h"
#include "GameSceneManager.h"
#include "GamePlayScene.h"

#include <DirectXMath.h>

class GameTitleScene : public GameBaseScene
{
public: // エイリアス
	using XMFLOAT2 = DirectX::XMFLOAT2;

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

private: // メンバ変数
	Input* input_ = nullptr;
	DirectXCommon* dxCommon_ = nullptr;
	
	// スプライト
	Sprite* sprite_ = nullptr;
	Sprite* title_ = nullptr;
	Sprite* space_ = nullptr;
	Sprite* inBlack_ = nullptr;
	Sprite* outBlack_ = nullptr;

	// 天球
	Skydome* skydome = nullptr;

	// オブジェクト
	Object3d* object3d = nullptr;
	Model* model = nullptr;

	// タイトルの設定
	DirectX::XMFLOAT4 titleColor_ = { 1,1,1,1 };
	DirectX::XMFLOAT3 titlePos_ = { 200,100,0 };
	DirectX::XMFLOAT2 titleScale_ = { 1000,300 };
	float titleRot_ = 0.0f;

	// スペースの設定
	DirectX::XMFLOAT4 spaceColor_ = { 1,1,1,1 };
	DirectX::XMFLOAT3 spacePos_ = { 480,500,0 };
	DirectX::XMFLOAT2 spaceScale_ {330, 43};
	float spaceRot_ = 0.0f;

	DirectX::XMFLOAT3 pPos = { 0,-5,-30 };
	DirectX::XMFLOAT3 eye = { 0,0,-50 };

	// タイマー
	int startTimer = 0;
	int spaceTimer = 0;

	// フラグ
	bool start = false;
	bool isSpace = true;
	bool isPos = false;

	// スプライトのアルファ値
	float bInAlpha_ = 1.0f;
	float bOutAlpha_ = 1.0f;

	// フェードイン・アウトフラグ
	bool isFadeIn = false;
	bool isFadeOut = true;
};
