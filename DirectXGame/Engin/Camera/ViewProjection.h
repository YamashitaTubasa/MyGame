/*
 * @file ViewProjection.h
 * @brief ビュープロジェクションクラス
 * @author Yamashita Tubasa
 */

#pragma once

#include "Vector3.h"

#include <wrl.h>
#include <d3d12.h>

class ViewProjection
{
private: //エイリアス
	// Microsoft::WRL::を省略
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	// 定数バッファ用データ構造体
	struct ConstBufferData
	{
		Vector3 ambient_;  // アンビエント係数
		float pad1_;       // パディング
		Vector3 diffuse_;  // ディフューズ係数
		float pad2_;       // パディング
		Vector3 specular_; // スペキュラー係数
		float alpha_;      // アルファ
	};

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 定数バッファ生成
	/// </summary>
	void CreateConstBuffer();

	/// <summary>
	/// マッピング
	/// </summary>
	void Map();

	/// <summary>
	/// 行列更新
	/// </summary>
	void UpdateMatrix();

private:
	// 定数バッファ(マテリアル)
	ComPtr<ID3D12Resource> constBuff_;
	ConstBufferData* constMap = nullptr;
};

