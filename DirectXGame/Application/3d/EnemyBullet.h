/**
 * @file EnemyBullet.h
 * @brief 敵の弾に関するクラス
 * @author Yamashita Tubasa
 */

#include "Object3d.h"
#include "Model.h"

#pragma once

class EnemyBullet {
private:
	static const int32_t lifeTime_ = 60 * 5;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& velocity);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

public:
	/// <summary>
	/// デスフラグの取得
	/// </summary>
	/// <param name="isDead"></param>
	void SetIsDead(bool isDead) { isDead_ = isDead; }
	bool GetIsDead() const { return isDead_; }

private:
	// オブジェクト
	std::unique_ptr<Object3d> bulletObject_;
	std::unique_ptr<Model> bulletModel_;

	// 座標
	DirectX::XMFLOAT3 position_ = { 0,0,0 };
	DirectX::XMFLOAT3 scale_ = { 1,1,1 };
	DirectX::XMFLOAT3 rotate_ = { 0,0,0 };
	DirectX::XMFLOAT3 velocity_ = { 0,0,0 };

	// デスタイマー
	int32_t dethTimer_ = lifeTime_;
	// デスフラグ
	bool isDead_ = false;
};