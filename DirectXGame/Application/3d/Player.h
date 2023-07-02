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

public: // メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

public: // メンバ変数
	DirectXCommon* dxCommon = nullptr;
	// モデル
	Model* playerM = nullptr;
	// オブジェクト
	Object3d* playerO3 = nullptr;
	// 入力
	Input* input = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	DirectX::XMFLOAT3 position[5]{};
	DirectX::XMFLOAT3 rotation[5]{};
	DirectX::XMFLOAT3 scale[5]{};
	DirectX::XMFLOAT3 eye[5]{};
	DirectX::XMFLOAT3 target[5]{};
};

