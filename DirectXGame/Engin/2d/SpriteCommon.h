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
	// 初期化
	void Initialize();
	// 更新
	void Update();
	// 描画
	void Draw();

public: // getter
	DirectXCommon* GetDirectXCommon() const{ return directXCommon; }

private:
	DirectXCommon* directXCommon = nullptr;
};