/**
 * @file CollisionManager.h
 * @brief 当たり判定を管理するクラス
 * @author Yamashita Tubasa
 */

#pragma once

#include <forward_list>

class BaseCollider;

class CollisionManager
{
public: // 静的メンバ関数
	static CollisionManager* GetInstance();

public: // メンバ関数
	inline void AddCollider(BaseCollider* collider) {
		colliders.push_front(collider);
	}

	inline void RemoveCollider(BaseCollider* collider) {
		colliders.remove(collider);
	}

	/// <summary>
	/// 全ての衝突チェック
	/// </summary>
	void CheckAllCollisions();

private:
	// コンストラクタ
	CollisionManager() = default;
	// デストラクタ
	~CollisionManager() = default;
	// コピーコンストラクタの禁止
	CollisionManager(const CollisionManager&) = delete;
	// コピー代入演算子の禁止
	CollisionManager& operator=(const CollisionManager&) = delete;

private:
	// コライダーのリスト
	std::forward_list<BaseCollider*> colliders;
};