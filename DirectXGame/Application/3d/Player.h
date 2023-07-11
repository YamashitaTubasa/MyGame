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

	DirectX::XMFLOAT3 move = { 0,0.25,0 };
	DirectX::XMFLOAT3 pPosition = { 0,0,0 };
	DirectX::XMFLOAT3 pRotation = { 0,0,0 };
	DirectX::XMFLOAT3 pScale = { 1,1,1 };
	DirectX::XMFLOAT3 position[5]{};
	DirectX::XMFLOAT3 rotation[5]{};
	DirectX::XMFLOAT3 scale[5]{};
	DirectX::XMFLOAT3 eye[5]{};
	DirectX::XMFLOAT3 target[5]{};
	DirectX::XMFLOAT3 pBulletP = { 0,0,0 };

	bool isRightMove = false;
	bool isLeftMove = false;
	bool isUpMove = false;
	bool isDownMove = false;
	float time = 0.0f;
	float frame = 0.0f;
	float endFrame = 2.0f;
	float start = 0.0f;
	float end = 2.0f;
};

