#pragma once

#include "DirectXCommon.h"
#include "Input.h"
#include "Sprite.h"
#include "Object3d.h"
#include "Model.h"
#include "Skydome.h"
#include "GameBaseScene.h"
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

	// 天球
	Skydome* skydome = nullptr;

	// オブジェクト
	Object3d* object3d = nullptr;
	Model* model = nullptr;

	DirectX::XMFLOAT4 titleColor_ = { 1,1,1,1 };
	DirectX::XMFLOAT3 titlePos_ = { 200,100,0 };
	DirectX::XMFLOAT2 titleScale_ = { 1000,300 };
	DirectX::XMFLOAT4 spaceColor_ = { 1,1,1,1 };
	DirectX::XMFLOAT3 spacePos_ = { 480,500,0 };
	DirectX::XMFLOAT2 spaceScale_ {330, 41};

	DirectX::XMFLOAT3 pPos = { 0,-5,-30 };
	DirectX::XMFLOAT3 eye = { 0,0,-50 };

	float titleRot_ = 0.0f;
	float spaceRot_ = 0.0f;
	bool start = false;
	int startTimer = 0;
	bool isSpace = true;
	int spaceTimer = 0;
};
