#pragma once

#include "Object3d.h"
#include "Model.h"

/// <summary>
/// 天球
/// </summary>
class Skydome
{
public:
	Skydome();
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

private:
	// オブジェクト
	Object3d* titleObj = nullptr;
	// モデル
	Model* titleModel = nullptr;
};

