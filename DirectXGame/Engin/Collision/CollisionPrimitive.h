/**
 * @file CollisionPrimitive.h
 * @brief 当たり判定プリミティブ
 * @author Yamashita Tubasa
 */

#pragma once

#include "Vector3.h"

#include <DirectXMath.h>

/// <summary>
/// 球
/// </summary>
struct Sphere 
{
	// 抽選座標
	DirectX::XMVECTOR center_ = { 0,0,0 };
	// 半径
	float radius_ = 1.0;
};

/// <summary>
/// 平面
/// </summary>
struct Plane 
{
	// 法線ベクトル
	DirectX::XMVECTOR normal_ = { 0,1,0,0 };
	// 原点(0,0,0)からの距離
	float distance_ = 0.0f;
};

/// <summary>
/// レイ（半直線）
/// </summary>
struct Ray
{
	// 始点座標
	DirectX::XMVECTOR start_ = { 0,0,0,1 };
	// 方向
	DirectX::XMVECTOR dir_ = { 1,0,0,0 };
};

/// <summary>
/// 法線付き三角形（時計回りが表面）
/// </summary>
class Triangle 
{
public:
	// 頂点座標3つ
	DirectX::XMVECTOR p0_;
	DirectX::XMVECTOR p1_;
	DirectX::XMVECTOR p2_;
	// 法線ベクトル
	DirectX::XMVECTOR normal_;

public:
	/// <summary>
	/// 法線の計算
	/// </summary>
	void ComputeNormal();
};