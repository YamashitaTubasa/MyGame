#pragma once

#include "Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "Sprite.h"
#include "SpriteCommon.h"
#include "Model.h"
#include "Object3d.h"
#include "GameSceneManager.h"
#include "Camera.h"
#include "ImGuiManager.h"
#include "ParticleManager.h"
#include "FbxLoader.h"
#include "PostEffect.h"
#include "AbstractSceneFactory.h"

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
	DirectXCommon* dXCommon = nullptr;
	WinApp* winApp = nullptr;
	Input* input = nullptr;
	ImGuiManager* imGuiManager = nullptr;
	Camera* camera = nullptr;
	GameSceneManager* sceneManager_ = nullptr;
	PostEffect* postEffect = nullptr;
	PostEffect* postEffect1 = nullptr;
	SpriteCommon* spriteCommon = nullptr;
	// シーンファクトリー
	AbstractSceneFactory* sceneFactory_ = nullptr;
	int a = 0;
};

