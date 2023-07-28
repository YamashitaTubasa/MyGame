#pragma once

#include "Object3d.h"
#include "Model.h"
#include "Camera.h"

/// <summary>
/// 敵クラス
/// </summary>
class Enemy
{
public:
	// コンストラクタ
	Enemy();

	// デストラクタ
	~Enemy();

public: // メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Camera* camera_);

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
	// オブジェクト
	Object3d* enemyO3 = nullptr;
	// モデル
	Model* enemyM = nullptr;

	Vector3 position[5]{};
	Vector3 rotation[5]{};
	Vector3 scale[5]{};
	Vector3 eye[5]{};
	Vector3 target[5]{};

	bool isMove = true;
	float moveSpeed = 1.1f;
};

