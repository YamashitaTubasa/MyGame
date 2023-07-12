#pragma once

#include "Object3d.h"
#include "Model.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
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
	void Draw(Camera* camera);

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

public: // ゲッター
	Vector3& GetPositon(){ return pPosition; }

public: // セッター
	static void SetCamera(Camera* camera) { Player::camera_ = camera; }

public: // 静的メンバ変数
	// カメラ
	static Camera* camera_;

public: // メンバ変数
	DirectXCommon* dxCommon = nullptr;
	// モデル
	Model* playerM = nullptr;
	// オブジェクト
	Object3d* playerO3 = nullptr;
	// 入力
	Input* input = nullptr;

	// 自キャラ弾
	std::list<PlayerBullet*> pBullets;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	Vector3 move = { 0,0.25,0 };
	Vector3 pPosition = { 0,0,0 };
	Vector3 pRotation = { 0,0,0 };
	Vector3 pScale = { 1,1,1 };
	Vector3 position[5]{};
	Vector3 rotation[5]{};
	Vector3 scale[5]{};
	Vector3 eye[5]{};
	Vector3 target[5]{};

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

