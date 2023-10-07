#pragma once

#include <DirectXMath.h>
#include <wrl.h>

// カメラ
class Camera
{
public:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	// 初期化
	void Initialize();

	// 更新
	void Update();

	// ビュー行列
	void UpdateViewMatrix();

	// 透視投影
	void UpdateProjectionMatrix();

public:
	const XMMATRIX& GetMatView() { return matView_; }
	const XMMATRIX& GetMatProjection() { return matProjection_; }
	const XMFLOAT3& GetEye() { return eye_; }
	const XMFLOAT3& GetTarget() { return target_; }
	const XMFLOAT3& GetUp() { return up_; }
	const float& GetDistance() { return distance_; }
	const XMMATRIX& GetViewProjectionMatrix() { return viewProjection_; }

	void SetMatView(const XMMATRIX& matView) { this->matView_ = matView; }
	void SetMatProjection(const XMMATRIX& matProjection) { this->matProjection_ = matProjection; }
	void SetEye(const XMFLOAT3& eye) { this->eye_ = eye; }
	void SetTarget(const XMFLOAT3& target) { this->target_ = target; }
	void SetUp(const XMFLOAT3& up) { this->up_ = up; }
	void SetDistance(float distance) { this->distance_ = distance; }

private:
	XMMATRIX matView_;
	XMMATRIX matProjection_;
	XMFLOAT3 eye_;
	XMFLOAT3 target_;
	XMFLOAT3 up_;
	XMMATRIX viewProjection_;
	float distance_ = 50.0f;
};