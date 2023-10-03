#pragma once

#include "TYFramework.h"

/// <summary>
/// ゲーム全体
/// </summary>
class MyGame : public TYFramework
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
