/**
 * @file BossEnemy.h
 * @brief ボス敵クラス
 * @author Yamashita Tubasa
 */

#pragma once

#include "Object3d.h"
#include "Model.h"

class BossEnemy
{
public:
	enum Move {
		RightMove,              // 右移動
		LeftMove,               // 左移動
		MoveInitializeTime = 0, // 初期時間
		MoveMaxTime = 50        // 最大時間
	};

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

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

	/// <summary>
	/// 移動
	/// </summary>
	void Move();

	// ボスフラグの取得
	bool GetIsBoss() const { return isBoss_; }
	bool GetIsBossStaging() const { return isBossStaging_; }
	bool GetIsBossHp() const { return isBossHp_; }
	bool GetIsBd() const { return isBd_; }
	//bool GetIsBossDamage() const { return isBossDamage_; }

	// ボス座標の取得
	const DirectX::XMFLOAT3& GetPosition() const { return position_; }
	void SetPosition(const DirectX::XMFLOAT3& position) { position_ = position; }

	bool GetIsDead() const { return isDead_; }
	void SetIsDead(bool isDead) { isDead_ = isDead; }

private:
	std::unique_ptr<Object3d> bossObject_;
	std::unique_ptr<Model> bossModel_;

	// ボスの移動量
	DirectX::XMFLOAT3 move_ = { 0,0.25,0 };
	// ボスの座標
	DirectX::XMFLOAT3 position_ = { 0,-1,250 };
	// ボスの回転
	DirectX::XMFLOAT3 rotation_ = { 0,180,0 };
	// ボスのサイズ
	DirectX::XMFLOAT3 scale_ = { 25,25,25 };
	float speed_ = 0.8f; // ボスの速さ
	float xSpeed_ = 0.2f;
	float screenMax_ = 10.0f;
	int isMove_ = RightMove;
	int isMoveTime_ = 0;
	bool isBossHp_ = false;
	int bossHp_ = 0;
	int bossTimer_ = 0;
	bool isBH_ = false;
	bool isBd_ = false;
	// ボスフラグ
	bool isBoss_ = false;
	bool isBossStaging_ = false;
	bool isDead_ = false;
};

