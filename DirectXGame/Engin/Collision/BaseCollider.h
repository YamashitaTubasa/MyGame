/**
 * @file BaseCollider.h
 * @brief コライダーの基底クラス
 * @author Yamashita Tubasa
 */

#pragma once

#include "CollisionTypes.h"
#include "Object3d.h"
#include "CollisionInfo.h"

class BaseCollider
{
public:
	// コンストラクタ
	BaseCollider() = default;
	// 仮想デストラクタ
	virtual ~BaseCollider() = default;

public:
	/// <summary>
	/// オブジェクトのセット
	/// </summary>
	/// <param name="object">オブジェクト</param>
	inline void SetObject(Object3d* object) {
		this->object3d = object;
	}

	/// <summary>
	/// オブジェクトの取得
	/// </summary>
	/// <returns>オブジェクト</returns>
	inline Object3d* GetObject3d() {
		return object3d;
	}

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

	// 形状タイプ取得
	inline CollisionShapeType GetShapeType() { return shapeType; }

	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	/// <param name="info">衝突情報</param>
	inline void OnCollision(const CollisionInfo& info) {
		object3d->OnCollision(info);
	}

protected:
	Object3d* object3d = nullptr;
	// 形状タイプ
	CollisionShapeType shapeType = SHAPE_UNKNOWN;
};