#pragma once

#include "Object3d.h"
#include "Model.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"
#include "Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "PlayerBullet.h"
#include "Camera.h"

#include <cmath>
#include <iostream>
#include <list>

/// <summary>
/// 天球
/// </summary>
class Skydome
{
public:
	// コンストラクタ
	Skydome();

	// デストラクタ
	~Skydome();

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ変数
	// モデル
	Model* skydomeM = nullptr;
	// オブジェクト
	Object3d* skydomeO3 = nullptr;
};

