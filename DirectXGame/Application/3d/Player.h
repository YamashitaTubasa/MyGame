/**
 * @file Player.h
 * @brief プレイヤーの挙動に関するクラス
 * @author Yamashita Tubasa
 */

#pragma once

#include "Object3d.h"
#include "Model.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Matrix4.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "PlayerBullet.h"
#include "Camera.h"
#include "EasingManager.h"
#include "Input.h"
#include "Enemy.h"
#include "Particle.h"
#include "ParticleManager.h"

#include <DirectXMath.h>
#include <cmath>
#include <iostream>
#include <list>

class Player : public Object3d
{
public:
	// コンストラクタ
	Player();

	// デストラクタ
	~Player();

public: // 静的メンバ関数
	/// <summary>
	/// 3Dオブジェクト生成
	/// </summary>
	/// <returns>インスタンス</returns>
	static Player* Create(Model* model = nullptr);

public: // メンバ関数
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
	/// 攻撃
	/// </summary>
	void Attack();

	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	/// <param name="info">衝突情報</param>
	void OnCollision(const CollisionInfo& info) override;

	/// <summary>
	/// エフェクト
	/// </summary>
	void Effect();

public: // ゲッター
	/// <summary>
	/// プレイヤーの座標の取得
	/// </summary>
	/// <returns>座標</returns>
	const DirectX::XMFLOAT3& GetPositon() { return pPosition_; }

	/// <summary>
	/// HPの取得
	/// </summary>
	/// <returns>HP</returns>
	int GetHp() const { return hp_; }

	/// <summary>
	/// HPフラグの取得
	/// </summary>
	/// <returns>HPフラグ</returns>
	bool GetIsHp()const { return isHp_; }

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	// モデル
	Model* playerM_ = nullptr;
	Model* reticleM_ = nullptr;
	Model* reticle1M_ = nullptr;
	// オブジェクト
	Object3d* playerO3_ = nullptr;
	Object3d* reticleO3_ = nullptr;
	Object3d* reticle1O3_ = nullptr;
	// 入力
	Input* input_ = nullptr;
	// カメラ
	Camera* camera_ = nullptr;
	// 自キャラ弾
	std::list<PlayerBullet*> pBullets_;
	// 敵キャラ
	std::list<std::unique_ptr<Enemy>> enemys_;
	Enemy* enemy_ = nullptr;
	// パーティクル
	Particle* particle_ = nullptr;
	ParticleManager* particleMan_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	// プレイヤーの移動量
	DirectX::XMFLOAT3 move_ = { 0,0.25,0 };
	// プレイヤーの座標
	DirectX::XMFLOAT3 pPosition_ = { 0,0,0 };
	// プレイヤーの回転
	DirectX::XMFLOAT3 pRotation_ = { 0,0,0 };
	// プレイヤーのサイズ
	DirectX::XMFLOAT3 pScale_ = { 1,1,1 };
	// プレイヤーの弾の座標
	DirectX::XMFLOAT3 pBulletP_ = { 0,0,0 };

	// レティクルの移動量
	DirectX::XMFLOAT3 rmove_ = { 0,0.25,0 };
	DirectX::XMFLOAT3 r1move_ = { 0,0.25,0 };
	// レティクルの座標
	DirectX::XMFLOAT3 rPosition_ = { 0,0,-1 };
	DirectX::XMFLOAT3 r1Position_ = { 0,0,-1 };
	// レティクルの回転
	DirectX::XMFLOAT3 rRotation_ = { 0,0,0 };
	DirectX::XMFLOAT3 r1Rotation_ = { 0,0,0 };
	// レティクルのサイズ
	DirectX::XMFLOAT3 rScale_ = { 0.4f, 0.4f, 0.4f };
	DirectX::XMFLOAT3 r1Scale_ = { 0.2f, 0.2f, 0.2f };

	// 視点
	DirectX::XMFLOAT3 eye_ = { 0,-1,-155 };
	// 注視点
	DirectX::XMFLOAT3 target_ = { 0,0,0 };

	// 右への移動フラグ
	bool isRightMove_ = false;
	// 左への移動フラグ
	bool isLeftMove_ = false;
	// 上への移動フラグ
	bool isUpMove_ = false;
	// 下への移動フラグ
	bool isDownMove_ = false;
	// イージング変数
	bool isEaseFlag_ = false;
	float eTime_ = 0.0f;
	float eFrame_ = 0.0f;
	float eEndFrame_ = 80.0f;
	float eStart_ = 0.0f;
	float eEnd_ = 360.0f;
	// 回転のフラグ
	bool isRot_ = false;
	// HP
	int hp_ = 10;
	int time_ = 0;
	bool isTime_ = false;
	// レティクルフラグ
	bool isReticle_ = false;
	// スタート時の演出フラグ
	bool isStartStaging_ = true;
	// スタート演出タイム
	int startCount_ = 0;
	// ゴール時の演出フラグ
	bool isEndStaging_ = false;
	// ゴール演出タイム
	int endCount_ = 0;
	bool isPostE_ = false;
	// 自機のフラグ
	bool isPlayer_ = true;
	int isBullet_ = 1;
	bool isEffect_ = false;
	int effectTime_ = 0;
	bool isHp_ = false;
};

