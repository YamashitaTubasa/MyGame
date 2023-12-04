/**
 * @file WorldTransform.h
 * @brief ワールドトランスフォームクラス
 * @author Yamashita Tubasa
 */

#pragma once

#include "Matrix4.h"
#include "Vector3.h"
#include "DirectXCommon.h"

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
		Matrix4 mat_; // 3D変換行列
	};

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 定数バッファの生成
	/// </summary>
	void CreateConstBuffer();

	/// <summary>
	/// 定数バッファとのデータリンク
	/// </summary>
	void Map();

	/// <summary>
	/// 行列計算
	/// </summary>
	void UpdateMatrix();

	/// <summary>
	/// 度数からラジアンに変換
	/// </summary>
	/// <param name="angle">角度</param>
	float ToRadian(float angle) { return (float)(angle * (PI / 180)); }

	/// <summary>
	/// 定数バッファデータ構造体の取得
	/// </summary>
	/// <returns>定数バッファデータ構造体</returns>
	ID3D12Resource* GetConstBuffer() { return constBuffer_.Get(); }

	/// <summary>
	/// ワールド行列の取得
	/// </summary>
	/// <returns></returns>
	Matrix4 GetMatWorld() { return matWorld_; }

private:
	ComPtr<ID3D12Resource> constBuffer_;
	ComPtr<ID3D12GraphicsCommandList> cmdList_;
	ComPtr<ID3D12Device> device_;

	ConstBufferData* constMap_ = nullptr;

	// 親となるワールド行列への変換
	WorldTransform* parent_ = nullptr;

public:
	// スケール
	Vector3 scale_ = { 1.0f,1.0f,1.0f };

	// 回転角
	Vector3 rotation_ = { 0,0,0 };

	// 座標
	Vector3 position_ = { 0,0,0 };

	// ワールド行列変換
	Matrix4 matWorld_;
};