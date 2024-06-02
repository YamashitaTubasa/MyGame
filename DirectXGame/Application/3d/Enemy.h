/**
 * @file Enemy.h
 * @brief 敵の挙動に関するクラス
 * @author Yamashita Tubasa
 */

#pragma once

#include "Object3d.h"
#include "Model.h"
#include "Collider.h"

#include <sstream>

class GamePlayScene;

class Enemy : public Object3d
{
public:
	// コンストラクタ
	Enemy();

	// デストラクタ
	~Enemy();

public:
	// 敵の行動フェーズ
	enum class Phase {
		Access, // 接近する
		Fire    // 弾の発射
	};

public: // メンバ関数
	/// <summary>
	/// 3Dオブジェクト生成
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns></returns>
	//static std::unique_ptr<Enemy> Create(Model* model = nullptr);

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const DirectX::XMFLOAT3& position);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 弾の発射
	/// </summary>
	void Fire();

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

public: // Getter・Setter
	/// <summary>
	/// 座標の取得
	/// </summary>
	/// <returns>座標</returns>
	const DirectX::XMFLOAT3& GetPosition() const { return position_; }

	// デスフラグの取得
	bool GetIsDead() const { return isDead_; }
	void SetIsDead(bool isDead) { isDead_ = isDead; }

	/// <summary>
	/// ダメージ
	/// </summary>
	/// <param name="damage"></param>
	void SetDamage(bool isDamage) { isDamage_ = isDamage; }
	bool GetDamage() const { return isDamage_; }
	bool GetIsMobEnemyAllive() { return isMobEnemyAllive_; }
	void SetIsMobEnemyAllive(bool isMobEnemyAllive) { isMobEnemyAllive_ = isMobEnemyAllive; }

	/// <summary>
	/// ゲームプレイシーンのセット
	/// </summary>
	/// <param name="gamePlayScene"></param>
	void SetGamePlayScene(GamePlayScene* gamePlayScene) { gamePlayScene_ = gamePlayScene; }

private:
	// オブジェクト
	std::unique_ptr<Object3d> enemyObject_ = nullptr;
	// モデル
	std::unique_ptr<Model> enemyModel_;
	// ゲームシーン
	GamePlayScene* gamePlayScene_ = nullptr;

	// 座標
	DirectX::XMFLOAT3 position_ = { 0,0,0 };
	DirectX::XMFLOAT3 oldPosition_ = { 0,0,0 };
	DirectX::XMFLOAT3 rotation_ = { 0,180,0 };
	DirectX::XMFLOAT3 scale_ = { 5,5,5 };
	DirectX::XMFLOAT3 eye_;
	DirectX::XMFLOAT3 target_;

	// 敵のデスフラグ
	bool isDead_ = false;
	bool isDamage_ = false;
	bool isMobEnemyAllive_;
	bool isMove_ = false;
	int moveTime_ = 0;

	// フェーズ
	Phase phase_ = Phase::Access;

	// 弾の発射間隔
	static const int bulletInterval_ = 50;
	static const int fireInterval_ = 100;
	int bulletTime_ = bulletInterval_;
	int fireTime_ = fireInterval_;
};

