/**
 * @file TYFramework.h
 * @brief フレームワーククラス
 * @author Yamashita Tubasa
 */

#pragma once

#include "Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "Sprite.h"
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
	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize();

	/// <summary>
	/// 終了
	/// </summary>
	virtual void Finalize();

	/// <summary>
	/// 毎フレーム更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() = 0;

	/// <summary>
	/// 終了チェック
	/// </summary>
	/// <returns></returns>
	virtual bool IsEndRequst() { return endRequst_; }


public:
	// デストラクタ
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
	// シーンファクトリー
	AbstractSceneFactory* sceneFactory_ = nullptr;
	int a = 0;
};

