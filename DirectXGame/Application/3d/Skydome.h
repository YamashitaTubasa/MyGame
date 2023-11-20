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
	std::unique_ptr<Object3d> skydomeObj_ = nullptr;
	// モデル
	std::unique_ptr<Model> skydomeModel_ = nullptr;

	// 天球の回転角
	DirectX::XMFLOAT3 rotate_ = { 0,0,0 };

	// フラグ
	bool isSkydome_ = false;
	// 天球の回転速度
	float skySpeed_ = 0.05f;
};

