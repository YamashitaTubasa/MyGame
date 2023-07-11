#pragma once

#include "Object3d.h"
#include "Model.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Matrix4.h"
#include "Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "PlayerBullet.h"
#include "Camera.h"

#include <cmath>
#include <iostream>
#include <list>

/// <summary>
/// 自キャラ
/// </summary>
class Player
{
public:
	// コンストラクタ
	Player();

	// デストラクタ
	~Player();

public:
	// イージング
	float easeInQuad(float x);

public: // メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Camera* camera);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

public: // ゲッター
	// プレイヤーの座標の取得
	const DirectX::XMFLOAT3& GetPositon() { return pPosition; }
	// プレイヤーの弾の座標の取得
	const DirectX::XMFLOAT3& GetpBulletP() { return pBulletP; }
	
public: // メンバ変数
	DirectXCommon* dxCommon = nullptr;
	// モデル
	Model* playerM = nullptr;
	// オブジェクト
	Object3d* playerO3 = nullptr;
	// 入力
	Input* input = nullptr;
	// カメラ
	Camera* camera_ = nullptr;
	// 自キャラ弾
	std::list<PlayerBullet*> pBullets;

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

	// 右への移動フラグ
	bool isRightMove = false;
	// 左への移動フラグ
	bool isLeftMove = false;
	// 上への移動フラグ
	bool isUpMove = false;
	// 下への移動フラグ
	bool isDownMove = false;
	// イージング変数
	float time = 0.0f;
	float frame = 0.0f;
	float endFrame = 2.0f;
	float start = 0.0f;
	float end = 2.0f;
};

