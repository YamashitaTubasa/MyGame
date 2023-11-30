/**
 * @file SphereCollider.cpp
 * @brief 球の衝突判定オブジェクト
 * @author Yamashita Tubasa
 */

#include "SphereCollider.h"

using namespace DirectX;

void SphereCollider::Update()
{
	// ワールド行列から座標を抽出
	const Matrix4& matWorld = object3d_->GetWorldTransform().GetMatWorld();

	const Vector3& position = { matWorld.m[3][0], matWorld.m[3][1], matWorld.m[3][2] };

	// 球のメンバ変数の更新
	Sphere::center_ = position + offset_;
	Sphere::radius_ = radius_;
}