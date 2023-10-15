/**
 * @file CollisionInfo.h
 * @brief 衝突情報に関するクラス
 * @author Yamashita Tubasa
 */

#pragma once

#include <DirectXMath.h>

class Object3d;
class BaseCollider;

struct CollisionInfo
{
public:
	CollisionInfo(Object3d* object, BaseCollider* collider, const DirectX::XMVECTOR& inter) {
		this->object = object;
		this->collider = collider;
		this->inter = inter;
	}

	// 衝突相手のオブジェクト
	Object3d* object = nullptr;
	// 衝突相手のコライダー
	BaseCollider* collider = nullptr;
	// 衝突点
	DirectX::XMVECTOR inter;
};