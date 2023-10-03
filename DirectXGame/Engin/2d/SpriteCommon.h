#pragma once

#include <DirectXMath.h>

#include "DirectXCommon.h"

using namespace DirectX;

// ===================
// スプライト共通部
// ===================
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