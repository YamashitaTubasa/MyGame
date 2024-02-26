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
	const XMMATRIX& matWorld = object3d_->GetMatWorld();

	// 球のメンバ変数の更新
	Sphere::center_ = matWorld.r[3] + offset_;
	Sphere::radius_ = radius_;
}