#pragma once

#include "Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "Sprite.h"
#include "Model.h"
#include "Object3d.h"
#include "GameScene.h"
#include "Camera.h"
#include "ImGuiManager.h"
#include "PostEffect.h"

class TYFramework
{
public:
	// 実行
	void Run();

public: // メンバ関数
	// 初期化
	virtual void Initialize();

	// 終了
	virtual void Finalize();

	// 毎フレーム更新
	virtual void Update();

	// 描画
	virtual void Draw() = 0;

	// 終了チェック
	virtual bool IsEndRequst() { return endRequst_; }


public:
	virtual ~TYFramework() = default;

private:
	// ゲーム終了フラグ
	bool endRequst_ = false;

protected:
	// スプライト
	SpriteCommon spriteCommon;
	DirectXCommon* dXCommon = nullptr;
	WinApp* winApp = nullptr;
	Input* input = nullptr;
	ImGuiManager* imGuiManager = nullptr;
	Camera* camera = nullptr;
	PostEffect* postEffect = nullptr;
};

