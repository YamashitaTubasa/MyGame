/**
 * @file WorldTransform.h
 * @brief 定数バッファクラス
 * @author Yamashita Tubasa
 */

#pragma once

#include "Matrix4.h"
#include "Vector3.h"

#include <DirectXMath.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>

class WorldTransform
{
private: // エイリアス
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	// 定数バッファデータ構造体
	struct ConstBufferData {
		DirectX::XMMATRIX mat_; // 3D変換行列
	};

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

	/// <summary>
	/// 度数からラジアンに変換
	/// </summary>
	/// <param name="angle">角度</param>
	float ToRadian(float angle) { return angle * (PI / 180); }

private:
	ComPtr<ID3D12Resource> constBuff_;
	ComPtr<ID3D12GraphicsCommandList> cmdList_;
	ComPtr<ID3D12Device> device_;

	ConstBufferData* constMap = nullptr;

	// スケール
	Vector3 scale_ = { 1.0f,1.0f,1.0f };

	// 回転角
	Vector3 rotation_ = { 0,0,0 };

	// 座標
	Vector3 position_ = { 0,0,0 };

	// ワールド行列変換
	Matrix4 matWorld_;
};