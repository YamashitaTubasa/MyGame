/**
 * @file Skydome.h
 * @brief 天球に関するクラス
 * @author Yamashita Tubasa
 */

#pragma once

#include "Object3d.h"
#include "Model.h"
#include "DirectXCommon.h"
#include "Input.h"

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

	/// <summary>
	/// 移動
	/// </summary>
	void Move();
	
private:
	// モデル
	std::unique_ptr<Model> skydomeModel_ = nullptr;
	// オブジェクト
	std::unique_ptr<Object3d> skydomeObj_ = nullptr;
	
	// 天球の回転角
	DirectX::XMFLOAT3 rotate_ = { 0,0,0 };
	// 天球の移動
	DirectX::XMFLOAT3 move_ = { 0,0,0 };

	// フラグ
	bool isSkydome_ = false;
	// 天球の回転速度
	float skyRotSpeed_ = 0.025f;
	// 天球の移動速度
	float skySpeed_ = 1.0f;
};

