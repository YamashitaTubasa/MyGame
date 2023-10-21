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
		this->object3d_ = object;
	}

	/// <summary>
	/// オブジェクトの取得
	/// </summary>
	/// <returns>オブジェクト</returns>
	inline Object3d* GetObject3d() {
		return object3d_;
	}

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 形状タイプ取得
	/// </summary>
	/// <returns></returns>
	inline CollisionShapeType GetShapeType() { return shapeType_; }

	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	/// <param name="info">衝突情報</param>
	inline void OnCollision(const CollisionInfo& info) {
		object3d_->OnCollision(info);
	}

protected:
	Object3d* object3d_ = nullptr;
	// 形状タイプ
	CollisionShapeType shapeType_ = SHAPE_UNKNOWN;
};