/**
 * @file Collision.h
 * @brief 当たり判定に関するクラス
 * @author Yamashita Tubasa
 */

#pragma once

#include "CollisionPrimitive.h"

#include "Vector3.h"

class Collision
{
public:
	/// <summary>
	/// 球と球の当たり判定
	/// </summary>
	/// <param name="sphere">球</param>
	/// <param name="sphere1">球</param>
	/// <param name="inter">交点（球上の最近接点）</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckSphere2Sphere(const Sphere& sphereA, const Sphere& sphereB, Vector3* inter = nullptr, Vector3* r = nullptr);

	/// <summary>
	/// 球と平面の当たり判定
	/// </summary>
	/// <param name="sphre">球</param>
	/// <param name="plane">平面</param>
	/// <param name="inter">交点（平面上の最近接点）</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckSphere2Plane(const Sphere& sphere, const Plane& plane, Vector3* inter = nullptr);

	/// <summary>
	/// 点と三角形の最近接点を求める
	/// </summary>
	/// <param name="point">点</param>
	/// <param name="trignale">三角形</param>
	/// <param name="closest">最近接点（出力用）</param>
	static void ClosestPtPoint2Triangle(const Vector3& point, const Triangle& trignale, Vector3* closest);

	/// <summary>
	/// 球と法線付き三角形の当たりチェック
	/// </summary>
	/// <param name="sphere">球</param>
	/// <param name="triangle">三角形</param>
	/// <param name="inter">交点（三角形上の最近接点）</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckSphere2Triangle(const Sphere& sphere, const Triangle& triangle, Vector3* inter = nullptr);

	/// <summary>
	/// レイと平面の当たり判定
	/// </summary>
	/// <param name="ray">レイ</param>
	/// <param name="plane">平面</param>
	/// <param name="distance">距離（出力用）</param>
	/// <param name="inter">交点（出力用）</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckRay2Plane(const Ray& ray, const Plane& plane, float* distance = nullptr, Vector3* inter = nullptr);

	/// <summary>
	/// レイと三角形の当たり判定
	/// </summary>
	/// <param name="ray">レイ</param>
	/// <param name="triangle">三角形</param>
	/// <param name="distance">距離（出力用）</param>
	/// <param name="inter">交点（出力用）</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckRay2Triangle(const Ray& ray, const Triangle& triangle, float* distance = nullptr, Vector3* inter = nullptr);
	
	/// <summary>
	/// レイと球の当たり判定
	/// </summary>
	/// <param name="ray">レイ</param>
	/// <param name="sphere">球</param>
	/// <param name="distance">距離（出力用）</param>
	/// <param name="inter">交点（出力用）</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckRay2Sphere(const Ray& ray, const Sphere&sphere, float* distance = nullptr, Vector3* inter = nullptr);
};