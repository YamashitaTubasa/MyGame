#pragma once

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"
#include "DirectXCommon.h"

#include <DirectXMath.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>

// カメラ
class Camera
{
public: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private: // サブクラス
	// 定数バッファ用データ構造体
	struct ConstBufferDataTransform 
	{
		Matrix4 viewproj;  // ビュープロジェクション行列
		Matrix4 world;     // ワールド行列
		Vector3 cameraPos; // カメラ座標（ワールド座標）
	};

public: // メンバ関数
	// デストラクタ
	~Camera();

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
	/// ビュー行列
	/// </summary>
	void UpdateViewMatrix();

	/// <summary>
	/// 透視投影
	/// </summary>
	void UpdateProjectionMatrix();

	/// <summary>
	/// 定数バッファ生成
	/// </summary>
	void CreateConstBuff();

	/// <summary>
	/// 定数バッファマッピング
	/// </summary>
	void ConstBuffMap();

	/// <summary>
	/// 定数バッファ転送
	/// </summary>
	void UpdateConstBuff();

public:
	const Matrix4& GetMatView() { return matView; }
	const Matrix4& GetMatProjection() { return matProjection; }
	const Vector3& GetEye() { return eye; }
	const Vector3& GetTarget() { return target; }
	const Vector3& GetUp() { return up; }
	const float& GetDistance() { return distance; }
	const Matrix4& GetViewProjectionMatrix() { return viewProjection; }
	ID3D12Resource* GetConstBuff() { return constBuffTransform.Get(); }

	void SetMatView(const Matrix4& matView) { this->matView = matView; }
	void SetMatProjection(const Matrix4& matProjection) { this->matProjection = matProjection; }
	void SetEye(const Vector3& eye) { this->eye = eye; }
	void SetTarget(const Vector3& target) { this->target = target; }
	void SetUp(const Vector3& up_) { this->up = up_; }
	void SetDistance(float distance_) { this->distance = distance_; }

private: // 静的メンバ変数
	//static ID3D12Device* device;

private: // メンバ変数
	Matrix4 matView;
	Matrix4 matProjection;
	Vector3 eye;
	Vector3 target;
	Vector3 up;
	Matrix4 viewProjection;

	// 定数バッファ用データ構造体
	ComPtr<ID3D12Resource> constBuffTransform;
	ConstBufferDataTransform* constMap = nullptr;

	float distance = 50.0f;
};