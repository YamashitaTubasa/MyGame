#pragma once

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"
#include "Model.h"
#include "Object3d.h"

struct BulletMove {
	DirectX::XMFLOAT3 transform = { 0,0,0 };
};

/// <summary>
/// 自キャラの弾
/// </summary>
class PlayerBullet
{
public:

public:
	// コンストラクタ
	PlayerBullet();

	// デストラクタ
	~PlayerBullet();

public: // メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& velocity);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

public: // ゲッター
	DirectX::XMFLOAT3 GetPosition() const { return pBulletPos; }

private:
	// モデル
	Model* bulletM = nullptr;
	// オブジェクト
	Object3d* bulletO3 = nullptr;
	// 自キャラの弾の処理
	BulletMove bulletMove;

	DirectX::XMFLOAT3 pBulletPos = { 0,0,0 };
	DirectX::XMFLOAT3 pBulletRot = { 0,0,0 };
	DirectX::XMFLOAT3 pBulletSca = { 1,1,1 };
	DirectX::XMFLOAT3 eye[5]{};
	DirectX::XMFLOAT3 target[5]{};
	DirectX::XMFLOAT3 velocity_ = { 0,0,0 };
};

