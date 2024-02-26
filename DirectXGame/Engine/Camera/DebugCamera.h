#pragma once

#include <DirectXMath.h>

/// <summary>
/// デバックカメラ
/// </summary>
class DebugCamera
{
	// X,Y,Z軸のローカル回転角
	DirectX::XMFLOAT3 rotation_ = { 0,0,0 };

	// ローカル座標
	DirectX::XMFLOAT3 translation_ = { 0,0,-50 };
};


