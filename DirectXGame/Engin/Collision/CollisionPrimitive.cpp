/**
 * @file CollisionPrimitive.cpp
 * @brief 当たり判定プリミティブ
 * @author Yamashita Tubasa
 */

#include "CollisionPrimitive.h"

using namespace DirectX;

void Triangle::ComputeNormal()
{
	Vector3 p0_p1 = p1_ - p0_;
	Vector3 p0_p2 = p2_ - p0_;

	// 外積により、2辺に垂直なベクトルを算出する
	normal_ = p0_p1.Cross(p0_p2);
	normal_.Normalize();
}
