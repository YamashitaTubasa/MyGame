/**
 * @file SphereCollider.cpp
 * @brief 球の衝突判定オブジェクト
 * @author Yamashita Tubasa
 */

#pragma once

#include "BaseCollider.h"
#include "CollisionPrimitive.h"

#include <DirectXMath.h>

class SphereCollider : public BaseCollider, public Sphere
{
private: // エイリアス
	// DirectX::を省略
	using XMVECTOR = DirectX::XMVECTOR;

public:
	SphereCollider(XMVECTOR offset = { 0,0,0,0 }, float radius = 1.0f) : offset_(offset), radius_(radius) {
		// 球形状をセット
		shapeType = COLLISIONSHAPE_SPHERE;
	}

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	inline void SetRadius(float s_radius) { this->radius_ = s_radius; }

private:
	// オブジェクト中心からのオフセット
	XMVECTOR offset_;
	// 半径
	float radius_;
};