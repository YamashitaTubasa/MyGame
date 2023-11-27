/**
 * @file CollisionInfo.h
 * @brief 衝突情報に関するクラス
 * @author Yamashita Tubasa
 */

#pragma once

#include <DirectXMath.h>
#include "Vector3.h"

class Object3d;
class BaseCollider;

struct CollisionInfo
{
public:
	CollisionInfo(Object3d* object, BaseCollider* collider, const Vector3& inter) {
		this->object_ = object;
		this->collider_ = collider;
		this->inter_ = inter;
	}

	// 衝突相手のオブジェクト
	Object3d* object_ = nullptr;
	// 衝突相手のコライダー
	BaseCollider* collider_ = nullptr;
	// 衝突点
	Vector3 inter_;
};