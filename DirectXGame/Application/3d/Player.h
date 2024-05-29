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
#include "Particle.h"
#include "ParticleManager.h"
#include "Sprite.h"
#include "Collider.h"

#include <DirectXMath.h>
#include <cmath>
#include <iostream>
#include <list>

class Player : public Object3d
{
private: // エイリアス
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;

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

	/// <summary>
	/// 回転
	/// </summary>
	void Spin();

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

public:
	// プレイヤーの座標の取得
	const DirectX::XMFLOAT3& GetPosition() const { return pPosition_; }

	// 当たった時
	bool GetIsHit()const { return isHit_; }
	void SetIsHit(bool isHit) { isHit_ = isHit; }

	// スタート演出フラグ
	bool GetIsStartStaging() const{ return isStartStaging_; }

	// クリア演出フラグ
	bool GetIsGameClearStaging() const { return isGameClearStaging_; }

	// ゲームオーバー演出フラグ
	bool GetIsGameOverStaging() const { return isGameOverStaging_; }
	void SetIsGameOverStaging(bool isGameOverStaging) { isGameOverStaging_ = isGameOverStaging; }

	// ボスフラグの取得
	bool GetIsBossDead() const { return isBossDead_; }
	void SetIsBossDead(bool isBossDead) { isBossDead_ = isBossDead; }
	bool GetIsBossStaging() const { return isBossStaging_; }

	bool GetIsMobEnemyAllive() const { return isMobEnemyAllive_; }
	void SetIsMobEnemyAllive(bool isMobEnemyAllive) { isMobEnemyAllive_ = isMobEnemyAllive; }

	// 自キャラ弾リストを取得
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() const { return pBullets_; }

	bool GetIsSpinEffect() const { return isSpinEffect_; }
	void SetIsSpinEffect(bool isSpinEffect) { isSpinEffect_ = isSpinEffect; }

	void SetIsRot(bool isRot) { isRot_ = isRot; }

private: // メンバ変数
	MyEngine::DirectXCommon* dxCommon_ = nullptr;

	// オブジェクト
	std::unique_ptr<Object3d> playerO3_ = nullptr;
	std::unique_ptr<Object3d> reticleO3_ = nullptr;
	std::unique_ptr<Object3d> reticle1O3_ = nullptr;

	// モデル
	std::unique_ptr<Model> playerM_;
	std::unique_ptr<Model> reticleM_;
	std::unique_ptr<Model> reticle1M_;

	// 入力
	MyEngine::Input* input_ = nullptr;
	// カメラ
	Camera* camera_ = nullptr;
	// 自キャラ弾リスト
	std::list<std::unique_ptr<PlayerBullet>> pBullets_;
	// パーティクル
	std::unique_ptr<MyEngine::Particle> particle_ = nullptr;
	std::unique_ptr<MyEngine::Particle> blackSmoke_ = nullptr;
	std::unique_ptr<MyEngine::Particle> circleMan_ = nullptr;
	std::unique_ptr<MyEngine::Particle> rotationParticle_ = nullptr;
	std::unique_ptr<MyEngine::ParticleManager> particleMan_ = nullptr;
	std::unique_ptr<MyEngine::ParticleManager> blackSmokeMan_ = nullptr;
	std::unique_ptr<MyEngine::ParticleManager> circle_ = nullptr;
	std::unique_ptr<MyEngine::ParticleManager> rotationParticleMan_ = nullptr;

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
	DirectX::XMFLOAT3 rPosition_ = { 0,-1,-125 };
	DirectX::XMFLOAT3 r1Position_ = { 0,-1.5f,-120 };
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
	const float playerSpeed_ = 0.5f;
	const float playerRotSpeed_ = 1.0f;
	const float playerSlopeMax_ = -20.0f;
	const float playerSlopeMaxPlus_ = 20.0f;
	const float playerSlopeMaxY_ = -10.0f;
	const float playerSlopeMin_ = 0.0f;

	// 移動フラグ
	bool isRightMove_ = false;
	bool isLeftMove_ = false;
	bool isUpMove_ = false;
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
	// 攻撃タイマー
	int time_ = 0;
	bool isTime_ = false;
	// レティクルフラグ
	bool isReticle_ = false;
	// スタート時の演出
	bool isStartStaging_ = true;
	int startCount_ = 0;
	const float startSpeed_ = 0.6f;
	// ゴール演出タイム
	const int endCount_ = 0;
	// 自機のフラグ
	int isBullet_ = 1;
	bool isDamageEffect_ = false;
	int effectTime_ = 10;
	bool isHit_ = false;
	bool isGameOverStaging_ = false;
	bool isGameClearStaging_ = false;
	// ゲームオーバーの演出
	const float playerFall_ = 0.05f;
	// ボスフラグ
	bool isBossDead_ = false;
	bool isBossStaging_ = false;

	// 爆発パーティクル
	const float particleBombPosX_ = 0.0f;
	const float particleBombPosY_ = 0.0f;
	const float particleBombPosZ_ = 0.0f;
	const int particleLife = 20;
	const float particleStartScale_ = 0.5f;
	const float particleEndScale_ = 0.0f;

	// 炎パーティクル
	const int fireParticleLife = 15;
	const float fireParticleStartScale_ = 0.4f;
	const float fireParticleEndScale_ = 0.0f;
	const int fireParticleMax_ = 100;

	// 回転パーティクル
	const int rotationParticleLife = 20;
	const float rotationParticleStartScale_ = 0.6f;
	const float rotationParticleEndScale_ = 0.0f;
	const int rotationParticleMax_ = 40;
	bool isRotationParticle_ = false;

	// ダメージ
	bool isSpinEffect_ = false;

	// シェイク
	const int shakeMin_ = -1;
	const int shakeMax_ = 1;
	const float shakeMdX_ = 2.5f;
	const float shakeMdMX_ = 5.0f;
	const float shakeMdY_ = 20.0f;
	const float shakeMdMY_ = 20.0f;
	int shakeTimer_ = 0;
	const int defaultShakeTimer_ = 0;
	const int shakeTimer10_ = 10;
	const int shakeTimer20_ = 20;
	float shakePosX_ = 0.0f;
	float shakePosX2_ = 0.0f;
	float shakePosY_ = 0.0f;
	float shakePosY2_ = 0.0f;
	float shakeDefaultPos_ = 0.0f;
	float shakeDefaultEye_ = 0.0f;

	bool isMobEnemyAllive_ = false;
};