#pragma once

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"
#include "Model.h"
#include "Object3d.h"
#include "Camera.h"

struct BulletMove {
	DirectX::XMFLOAT3 transform = { 0,0,0 };
};

/// <summary>
/// 自キャラの弾
/// </summary>
class PlayerBullet
{
public:
	// コンストラクタ
	PlayerBullet();

	// デストラクタ
	~PlayerBullet();

public: // メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Camera* camera_, const Vector3& position, const Vector3& velocity);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(Camera* camera);
	
private: // 定数
	// カメラ
	static Camera* camera;

private:
	// モデル
	Model* bulletM = nullptr;
	// オブジェクト
	Object3d* bulletO3 = nullptr;
	// 自キャラの弾の処理
	BulletMove bulletMove;

	Vector3 pBulletPos = { 0,0,0 };
	Vector3 pBulletRot = { 0,0,0 };
	Vector3 pBulletSca = { 1,1,1 };
	Vector3 eye[5]{};
	Vector3 target[5]{};

};

