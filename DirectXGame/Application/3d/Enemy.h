/**
 * @file Enemy.h
 * @brief 敵の挙動に関するクラス
 * @author Yamashita Tubasa
 */

#pragma once

#include "Object3d.h"
#include "Model.h"

#include <sstream>

class Enemy : public Object3d
{
public:
	// コンストラクタ
	Enemy();

	// デストラクタ
	~Enemy();

public: // メンバ関数
	/// <summary>
	/// 3Dオブジェクト生成
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns></returns>
	static Enemy* Create(Model* model = nullptr);

	/// <summary>
	/// 初期化
	/// </summary>
	bool Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 敵発生データの読み込み
	/// </summary>
	void LoadEnemyPopData();

	/// <summary>
	/// 敵発生コマンドの更新
	/// </summary>
	void UpdateEnemyPopCommands();

public: // Getter・Setter
	/// <summary>
	/// 座標の取得
	/// </summary>
	/// <returns>座標</returns>
	DirectX::XMFLOAT3 GetPosition() const { return position_[0]; }

	/// <summary>
	/// デスフラグの取得
	/// </summary>
	/// <returns>デスフラグ</returns>
	bool GetIsDead() const { return isDead_; }

	/// <summary>
	/// デスフラグのセット
	/// </summary>
	/// <param name="isDead_">デスフラグ</param>
	void SetIsDead(bool isDead) { isDead_ = isDead; }

	/// <summary>
	/// ダメージ
	/// </summary>
	/// <param name="damage"></param>
	void SetDamage(bool enemyDamage) { enemyDamage_ = enemyDamage; }
	void SetDamage01(bool enemyDamage01) { enemyDamage01_ = enemyDamage01; }
	void SetDamage02(bool enemyDamage02) { enemyDamage02_ = enemyDamage02; }
	void SetDamage03(bool enemyDamage03) { enemyDamage03_ = enemyDamage03; }
	bool GetDamage() const { return enemyDamage_; }
	bool GetDamage01() const { return enemyDamage01_; }
	bool GetDamage02() const { return enemyDamage02_; }
	bool GetDamage03() const { return enemyDamage03_; }

private:
	// オブジェクト
	Object3d* enemyO3_ = nullptr;
	Object3d* enemyO1_ = nullptr;
	Object3d* enemyO2_ = nullptr;
	Object3d* enemyO4_ = nullptr;
	// モデル
	Model* enemyM_ = nullptr;
	// 敵発生コマンド
	std::stringstream enemyPopCommands_;

	DirectX::XMFLOAT3 position_[5]{};
	DirectX::XMFLOAT3 rotation_[5]{};
	DirectX::XMFLOAT3 scale_[5]{};
	DirectX::XMFLOAT3 eye_[5]{};
	DirectX::XMFLOAT3 target_[5]{};

	// 待機フラグ
	bool isWait_ = false;
	// 待機タイマー
	int waitTimer_ = 0;
	// 敵のデスフラグ
	bool isDead_ = false;
	bool enemyDamage_ = false;
	bool enemyDamage01_ = false;
	bool enemyDamage02_ = false;
	bool enemyDamage03_ = false;
};

