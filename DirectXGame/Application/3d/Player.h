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
#include "EasingManager.h"

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
	/// <summary>
	/// イージング
	/// </summary>
	/// <param name="t">時間</param>
	/// <param name="b">開始位置</param>
	/// <param name="c">開始位置-終了位置の差</param>
	/// <param name="d">合計時間</param>
	/// <returns></returns>
	float EaseOutQuintP(float t, float b, float c, float d);
	float EaseOutQuint(float x);

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

	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	/// <param name="info">衝突情報</param>
	void OnCollision(const CollisionInfo& info) override;

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

	Vector3 move = { 0,0.3f,0 };
	Vector3 pPosition = { 0,0,0 };
	Vector3 pRotation = { 0,0,0 };
	Vector3 pScale = { 1,1,1 };
	Vector3 position[5]{};
	Vector3 rotation[5]{};
	Vector3 scale[5]{};
	Vector3 eye[5]{};
	Vector3 target[5]{};
	
	float speed = 0.25f;
	float rotSpeed = 1.0f;
	float pRotMax = 20.0f;
	float pRotMin = 0.0f;

	bool isEaseFlag = false;
	float eTime = 0.0f;
	float eFrame = 0.0f;
	float eEndFrame = 80.0f;
	float eStart = 0.0f;
	float eEnd = 360.0f;
	// 回転のフラグ
	bool isRot = false;

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

