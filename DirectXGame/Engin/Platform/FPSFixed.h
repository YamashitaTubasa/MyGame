/**
 * @file FPSFixed.h
 * @brief FPSに関するクラス
 * @author Yamashita Tubasa
 */

#pragma once

#pragma warning(push)
#pragma warning(disable:4365)
#include <chrono>
#pragma warning(pop)

#include <thread>

class FPSFixed 
{
public: // メンバ関数
	/// <summary>
	/// FPS固定初期化
	/// </summary>
	void InitializeFixFPS();

	/// <summary>
	/// FPS固定更新
	/// </summary>
	void UpdateFixFPS();

	// 記録時間(FPS固定用)
	std::chrono::steady_clock::time_point reference_;
};