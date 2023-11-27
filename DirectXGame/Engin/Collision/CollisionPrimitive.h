/**
 * @file CollisionPrimitive.h
 * @brief 当たり判定プリミティブ
 * @author Yamashita Tubasa
 */

#pragma once

#include "Vector3.h"
#include "Matrix4.h"

#include <DirectXMath.h>

/// <summary>
/// 球
/// </summary>
struct Sphere 
{
	// 抽選座標
	Vector3 center_ = { 0,0,0 };
	// 半径
	float radius_ = 1.0f;
	// 座標
	Vector3 position_ = { 0,0,0 };
};

/// <summary>
/// 平面
/// </summary>
struct Plane 
{
	// 法線ベクトル
	Vector3 normal_ = { 0,1,0 };
	// 原点(0,0,0)からの距離
	float distance_ = 0.0f;
};

/// <summary>
/// レイ（半直線）
/// </summary>
struct Ray
{
	// 始点座標
	Vector3 start_ = { 0,0,0 };
	// 方向
	Vector3 dir_ = { 1,0,0 };
};

/// <summary>
/// 法線付き三角形（時計回りが表面）
/// </summary>
class Triangle 
{
public:
	// 頂点座標3つ
	Vector3 p0_;
	Vector3 p1_;
	Vector3 p2_;
	// 法線ベクトル
	Vector3 normal_;

public:
	/// <summary>
	/// 法線の計算
	/// </summary>
	void ComputeNormal();
};