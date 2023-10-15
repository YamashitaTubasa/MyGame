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
	const DirectX::XMFLOAT3& GetPositon() { return pPosition; }

	/// <summary>
	/// プレイヤーの弾の座標の取得
	/// </summary>
	/// <returns>弾の座標</returns>
	const DirectX::XMFLOAT3& GetpBulletP() { return pBulletP; }

	/// <summary>
	/// HPの取得
	/// </summary>
	/// <returns>HP</returns>
	int GetHp() const { return hp; }

	/// <summary>
	/// HPフラグの取得
	/// </summary>
	/// <returns>HPフラグ</returns>
	bool GetIsHp()const { return isHp; }

	/// <summary>
	/// ポストエフェクトのフラグの取得
	/// </summary>
	/// <returns>ポストエフェクトフラグ</returns>
	bool GetIsPostE() { return isPostE; }

	/// <summary>
	/// 弾のフラグの取得
	/// </summary>
	/// <returns>弾のフラグ</returns>
	int GetIsBullet() { return isBullet; }
	
public: // メンバ変数
	DirectXCommon* dxCommon = nullptr;
	// モデル
	Model* playerM = nullptr;
	Model* reticleM = nullptr;
	Model* reticle1M = nullptr;
	// オブジェクト
	Object3d* playerO3 = nullptr;
	Object3d* reticleO3 = nullptr;
	Object3d* reticle1O3 = nullptr;
	// 入力
	Input* input = nullptr;
	// カメラ
	Camera* camera_ = nullptr;
	// 自キャラ弾
	std::list<PlayerBullet*> pBullets;
	// 敵キャラ
	std::list<std::unique_ptr<Enemy>> enemys_;
	Enemy* enemy = nullptr;
	// パーティクル
	Particle* particle = nullptr;
	ParticleManager* particleMan = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	// プレイヤーの移動量
	DirectX::XMFLOAT3 move = { 0,0.25,0 };
	// プレイヤーの座標
	DirectX::XMFLOAT3 pPosition = { 0,0,0 };
	// プレイヤーの回転
	DirectX::XMFLOAT3 pRotation = { 0,0,0 };
	// プレイヤーのサイズ
	DirectX::XMFLOAT3 pScale = { 1,1,1 };
	// プレイヤーの弾の座標
	DirectX::XMFLOAT3 pBulletP = { 0,0,0 };

	// レティクルの移動量
	DirectX::XMFLOAT3 rmove = { 0,0.25,0 };
	DirectX::XMFLOAT3 r1move = { 0,0.25,0 };
	// レティクルの座標
	DirectX::XMFLOAT3 rPosition = { 0,0,-1 };
	DirectX::XMFLOAT3 r1Position = { 0,0,-1 };
	// レティクルの回転
	DirectX::XMFLOAT3 rRotation = { 0,0,0 };
	DirectX::XMFLOAT3 r1Rotation = { 0,0,0 };
	// レティクルのサイズ
	DirectX::XMFLOAT3 rScale = { 0.4f, 0.4f, 0.4f };
	DirectX::XMFLOAT3 r1Scale = { 0.2f, 0.2f, 0.2f };

	// 視点
	DirectX::XMFLOAT3 eye = { 0,30,-100 };
	// 注視点
	DirectX::XMFLOAT3 target = { 0,0,0 };

	// 右への移動フラグ
	bool isRightMove = false;
	// 左への移動フラグ
	bool isLeftMove = false;
	// 上への移動フラグ
	bool isUpMove = false;
	// 下への移動フラグ
	bool isDownMove = false;
	// イージング変数
	bool isEaseFlag = false;
	float eTime = 0.0f;
	float eFrame = 0.0f;
	float eEndFrame = 80.0f;
	float eStart = 0.0f;
	float eEnd = 360.0f;
	// 回転のフラグ
	bool isRot = false;
	// HP
	int hp = 10;
	int time = 0;
	bool isTime = false;
	// レティクルフラグ
	bool isReticle = false;
	// スタート時の演出フラグ
	bool isStartStaging = true;
	// スタート演出タイム
	int startCount = 0;
	// ゴール時の演出フラグ
	bool isEndStaging = false;
	// ゴール演出タイム
	int endCount = 0;
	bool isPostE = false;
	// 自機のフラグ
	bool isPlayer = true;
	int isBullet = 1;
	bool isEffect = false;
	int effectTime = 0;
	bool isHp = false;
};

