/**
 * @file Skydome.h
 * @brief 天球に関するクラス
 * @author Yamashita Tubasa
 */

#pragma once

#include "Object3d.h"
#include "Model.h"

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

public:
	/// <summary>
	/// 天球の回転
	/// </summary>
	void RotateSky();

private:
	// オブジェクト
	Object3d* skydomeObj = nullptr;
	// モデル
	Model* skydomeModel = nullptr;

	// 天球の回転角
	DirectX::XMFLOAT3 rotate = { 0,0,0 };

	// フラグ
	bool isSkydome = false;
	// 天球の回転速度
	float skySpeed = 0.05f;
};

