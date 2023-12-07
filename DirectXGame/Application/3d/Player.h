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
	static std::unique_ptr<Player> Create(Model* model = nullptr);

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

	/// <summary>
	/// シェイク
	/// </summary>
	void Shake();

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

	/// <summary>
	/// スタート演出フラグ
	/// </summary>
	/// <returns></returns>
	bool GetIsStartStaging() const{ return isStartStaging_; }

	/// <summary>
	/// クリア演出フラグ
	/// </summary>
	/// <returns></returns>
	bool GetIsGameClearStaging() const { return isGameClearStaging_; }

	/// <summary>
	/// ゲームオーバー演出フラグ
	/// </summary>
	/// <returns></returns>
	bool GetIsGameOverStaging() const { return isGameOverStaging_; }

	/// <summary>
	/// ダメージ
	/// </summary>
	/// <returns></returns>
	bool GetDamage() const { return damage_; }
	bool GetDamage01() const { return damage01_; }
	bool GetDamage02() const { return damage02_; }
	bool GetDamage03() const { return damage03_; }

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	// モデル
	std::unique_ptr<Model> playerM_ = nullptr;
	std::unique_ptr<Model> reticleM_ = nullptr;
	std::unique_ptr<Model> reticle1M_ = nullptr;
	// オブジェクト
	std::unique_ptr<Object3d> playerO3_ = nullptr;
	std::unique_ptr<Object3d> reticleO3_ = nullptr;
	std::unique_ptr<Object3d> reticle1O3_ = nullptr;
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
	std::unique_ptr<Particle> particle_ = nullptr;
	std::unique_ptr<ParticleManager> particleMan_ = nullptr;
	std::unique_ptr<Particle> blackSmoke_ = nullptr;
	std::unique_ptr<ParticleManager> blackSmokeMan_ = nullptr;
	std::unique_ptr<Particle> circleMan_ = nullptr;
	std::unique_ptr<ParticleManager> circle_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	// プレイヤーの移動量
	DirectX::XMFLOAT3 move_ = { 0,0.25,0 };
	// プレイヤーの座標
	DirectX::XMFLOAT3 pPosition_ = { 0,-1,-140 };
	// プレイヤーの回転
	DirectX::XMFLOAT3 pRotation_ = { 0,0,0 };
	// プレイヤーのサイズ
	DirectX::XMFLOAT3 pScale_ = { 5,5,5 };
	// プレイヤーの弾の座標
	DirectX::XMFLOAT3 pBulletP_ = { 0,0,0 };

	// レティクルの移動量
	DirectX::XMFLOAT3 rmove_ = { 0,0.25,0 };
	DirectX::XMFLOAT3 r1move_ = { 0,0.25,0 };
	// レティクルの座標
	DirectX::XMFLOAT3 rPosition_ = { 0,-2,-45 };
	DirectX::XMFLOAT3 r1Position_ = { 0,-2.5f,-40 };
	// レティクルの回転
	DirectX::XMFLOAT3 rRotation_ = { 0,90,0 };
	DirectX::XMFLOAT3 r1Rotation_ = { 0,90,0 };
	// レティクルのサイズ
	DirectX::XMFLOAT3 rScale_ = { 0.4f, 0.4f, 0.4f };
	DirectX::XMFLOAT3 r1Scale_ = { 0.2f, 0.2f, 0.2f };

	// 視点
	DirectX::XMFLOAT3 eye_ = { 0, 0, -120 };
	// 注視点
	DirectX::XMFLOAT3 target_ = { 0,0,0 };

	// プレイヤー
	const float playerSpeed_ = 0.3f;
	const float playerRotSpeed_ = 1.0f;
	const float playerSlopeMax_ = -20.0f;
	const float playerSlopeMaxPlus_ = 20.0f;
	const float playerSlopeMaxY_ = -10.0f;
	const float playerSlopeMin_ = 0.0f;

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
	const float eEndFrame_ = 80.0f;
	const float eStart_ = 0.0f;
	const float eEnd_ = 360.0f;
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
	const float startSpeed_ = 0.4f;
	// ゴール時の演出フラグ
	bool isEndStaging_ = false;
	// ゴール演出タイム
	const int endCount_ = 0;
	bool isPostE_ = false;
	// 自機のフラグ
	bool isPlayer_ = true;
	int isBullet_ = 1;
	bool isEffect_ = false;
	int effectTime_ = 0;
	bool isHp_ = false;
	bool isGameOverStaging_ = false;
	bool isGameClearStaging_ = false;
	// ゲームオーバーの演出
	const float playerFall_ = 0.05f;

	// 爆発パーティクル
	const float particleBombPosX_ = 0.0f;
	const float particleBombPosY_ = 0.0f;
	const float particleBombPosZ_ = 0.0f;
	const int particleLife = 20;
	const float particleStartScale_ = 0.6f;
	const float particleEndScale_ = 0.0f;

	// 炎パーティクル
	const int fireParticleLife = 15;
	const float fireParticleStartScale_ = 0.4f;
	const float fireParticleEndScale_ = 0.0f;
	const int fireParticleMax_ = 100;

	// ダメージ
	bool damage_ = false;
	bool damage01_ = false;
	bool damage02_ = false;
	bool damage03_ = false;

	// シェイク
	const int shakeMin_ = -1;
	const int shakeMax_ = 1;
	const float shakeMd_ = 5.0f;
	const float shakeMdM_ = 10.0f;
	int shakeTimer_ = 0;
	const int defaultShakeTimer_ = 0;
	const int shakeTimer10_ = 10;
	const int shakeTimer20_ = 20;
	float shakePos_ = 0.0f;
	float shakeEye_ = 0.0f;
};

