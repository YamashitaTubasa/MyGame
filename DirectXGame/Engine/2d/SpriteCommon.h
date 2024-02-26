/**
 * @file SpriteCommon.h
 * @brief スプライト共通部に関するクラス
 * @author Yamashita Tubasa
 */

#pragma once

#include <DirectXMath.h>

#include "DirectXCommon.h"

class SpriteCommon
{
public:
	

public: // メンバ関数
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

public: // getter
	/// <summary>
	/// DirectXCommonのインスタンスの取得
	/// </summary>
	/// <returns>DirectXCommonのインスタンス</returns>
	MyEngine::DirectXCommon* GetDirectXCommon() const{ return dxCommon_; }

private:
	MyEngine::DirectXCommon* dxCommon_ = nullptr;
};