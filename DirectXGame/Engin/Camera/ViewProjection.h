/*
 * @file ViewProjection.h
 * @brief ビュープロジェクションクラス
 * @author Yamashita Tubasa
 */

#pragma once

class ViewProjection
{
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
};

