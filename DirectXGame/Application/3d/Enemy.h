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

public: // ゲッター
	DirectX::XMFLOAT3 GetPosition() const { return position[0]; }
	// デスフラグの取得
	bool GetIsDead() const { return isDead; }
	// デスフラグのセット
	void SetIsDead(bool isDead_) { isDead = isDead_; }

private:
	// オブジェクト
	Object3d* enemyO3 = nullptr;
	Object3d* enemyO1 = nullptr;
	Object3d* enemyO2 = nullptr;
	Object3d* enemyO4 = nullptr;
	// モデル
	Model* enemyM = nullptr;
	// 敵発生コマンド
	std::stringstream enemyPopCommands;

	DirectX::XMFLOAT3 position[5]{};
	DirectX::XMFLOAT3 rotation[5]{};
	DirectX::XMFLOAT3 scale[5]{};
	DirectX::XMFLOAT3 eye[5]{};
	DirectX::XMFLOAT3 target[5]{};

	// 待機フラグ
	bool isWait = false;
	// 待機タイマー
	int waitTimer = 0;
	// 敵のデスフラグ
	bool isDead = false;
};

