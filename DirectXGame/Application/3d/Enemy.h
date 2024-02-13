/**
 * @file Enemy.h
 * @brief 敵の挙動に関するクラス
 * @author Yamashita Tubasa
 */

#pragma once

#include "Object3d.h"
#include "Model.h"

#include <sstream>

class GamePlayScene;
class Enemy;

class BaseEnemyState
{
public:
	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update(Enemy* enemy) = 0;
};

class EnemyStateAccess : public BaseEnemyState
{
public:
	/// <summary>
	/// 更新
	/// </summary>
	void Update(Enemy* enemy) override;

private:
	DirectX::XMFLOAT3 position_;
};

class EnemyStateFire : public BaseEnemyState
{
public:
	/// <summary>
	/// 更新
	/// </summary>
	void Update(Enemy* enemy) override;

private:
	DirectX::XMFLOAT3 position_;
};

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

	/// <summary>
	/// 切り替え処理
	/// </summary>
	/// <param name="newState"></param>
	void ChangeState(BaseEnemyState* newState);

public: // Getter・Setter
	/// <summary>
	/// 座標の取得
	/// </summary>
	/// <returns>座標</returns>
	DirectX::XMFLOAT3 GetPosition() const { return position_; }

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
	
	BaseEnemyState* state_;

	// 座標
	DirectX::XMFLOAT3 position_ = { 0,0,0 };
	DirectX::XMFLOAT3 rotation_ = { 0,180,0 };
	DirectX::XMFLOAT3 scale_ = { 5,5,5 };
	DirectX::XMFLOAT3 eye_;
	DirectX::XMFLOAT3 target_;

	// 敵のデスフラグ
	bool isDead_ = false;
	bool isDamage_ = false;
	bool isMobEnemyAllive_;

	// フェーズ
	Phase phase_ = Phase::Access;
};