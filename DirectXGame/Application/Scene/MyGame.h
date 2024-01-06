/**
 * @file MyGame.h
 * @brief ゲーム全体を司るクラス
 * @author Yamashita Tubasa
 */

#pragma once

#include "TYFramework.h"

class MyGame : public MyEngine::TYFramework
{
public: // メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;
};
