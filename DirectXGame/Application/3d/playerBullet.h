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
	static const int32_t kLifeTime_ = 60 * 5;

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

public: // ゲッター
	/// <summary>
	/// 座標の取得
	/// </summary>
	/// <returns>座標</returns>
	DirectX::XMFLOAT3 GetPosition() const { return pBulletPos_; }

	/// <summary>
	/// デスフラグの取得
	/// </summary>
	/// <returns>デスフラグ</returns>
	bool IsDead() const { return isDead_; }

private:
	// モデル
	Model* bulletM_ = nullptr;
	// オブジェクト
	Object3d* bulletO3_ = nullptr;
	// 自キャラの弾の処理
	BulletMove bulletMove_;

	// 自キャラの弾の座標
	DirectX::XMFLOAT3 pBulletPos_ = { 0,0,0 };
	// 自キャラの弾の回転
	DirectX::XMFLOAT3 pBulletRot_ = { 0,0,0 };
	// 自キャラの弾のサイズ
	DirectX::XMFLOAT3 pBulletSca_ = { 1,1,1 };
	// 自キャラの弾の速度
	DirectX::XMFLOAT3 velocity__ = { 0,0,0 };
	DirectX::XMFLOAT3 eye_[5]{};
	DirectX::XMFLOAT3 target_[5]{};

	// デスタイマー
	int32_t deathTimer_ = kLifeTime_;
	// デスフラグ
	bool isDead_ = false;
};

