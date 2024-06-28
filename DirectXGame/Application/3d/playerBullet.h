/**
 * @file PlayerBullet.h
 * @brief プレイヤーの弾の挙動に関するクラス
 * @author Yamashita Tubasa
 */

#pragma once

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"
#include "Model.h"
#include "Object3d.h"
#include "Collider.h"

struct BulletMove {
	DirectX::XMFLOAT3 transform_ = { 0,0,0 };
};

class PlayerBullet
{
public:
	// コンストラクタ
	PlayerBullet();

	// デストラクタ
	~PlayerBullet();

private: // 定数
	// 寿命
	static const int32_t kLifeTime_ = 50;

public: // メンバ関数
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

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

public: // ゲッター
	/// <summary>
	/// 座標の取得
	/// </summary>
	/// <returns>座標</returns>
	const DirectX::XMFLOAT3& GetPosition() const { return position_; }

	/// <summary>
	/// デスフラグの取得
	/// </summary>
	/// <returns>デスフラグ</returns>
	bool GetIsDead() const { return isDead_; }
	void SetIsDead(bool isDead) { isDead_ = isDead; }

private:
	// オブジェクト
	std::unique_ptr<Object3d> bulletO3_ = nullptr;

	// モデル
	std::unique_ptr<Model> bulletM_;

	// 自キャラの弾の処理
	BulletMove bulletMove_;

	// 自キャラの弾の座標
	DirectX::XMFLOAT3 position_ = { 0,0,0 };
	// 自キャラの弾の回転
	DirectX::XMFLOAT3 rotation_ = { 0,180,0 };
	// 自キャラの弾のサイズ
	DirectX::XMFLOAT3 scale_ = { 0.5f,0.5f,0.5f };
	// 自キャラの弾の速度
	DirectX::XMFLOAT3 velocity__ = { 0,0,0 };
	DirectX::XMFLOAT3 eye_[5]{};
	DirectX::XMFLOAT3 target_[5]{};

	// デスタイマー
	int32_t deathTimer_ = kLifeTime_;
	// デスフラグ
	bool isDead_ = false;
};

