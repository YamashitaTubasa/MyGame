#pragma once

#pragma warning(push)
#pragma warning(disable:4265)
#pragma warning(disable:4365)
#pragma warning(disable:4514)
#pragma warning(disable:4625)
#pragma warning(disable:4626)
#pragma warning(disable:4668)
#pragma warning(disable:4820)
#pragma warning(disable:5039)
#pragma warning(disable:5204)
#pragma warning(disable:5220)
#include <DirectXMath.h>
#include <wrl.h>
#pragma warning(pop)

//========//
// カメラ //
//========//
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
	[[maybe_unused]] const XMMATRIX& GetMatView() { return matView_; }
	[[maybe_unused]] const XMMATRIX& GetMatProjection() { return matProjection_; }
	[[maybe_unused]] const XMFLOAT3& GetEye() { return eye_; }
	[[maybe_unused]] const XMFLOAT3& GetTarget() { return target_; }
	[[maybe_unused]] const XMFLOAT3& GetUp() { return up_; }
	[[maybe_unused]] const float& GetDistance() { return distance_; }
	[[maybe_unused]] const XMMATRIX& GetViewProjectionMatrix() { return viewProjection_; }

	[[maybe_unused]] void SetMatView(const XMMATRIX& matView) { this->matView_ = matView; }
	[[maybe_unused]] void SetMatProjection(const XMMATRIX& matProjection) { this->matProjection_ = matProjection; }
	[[maybe_unused]] void SetEye(const XMFLOAT3& eye) { this->eye_ = eye; }
	[[maybe_unused]] void SetTarget(const XMFLOAT3& target) { this->target_ = target; }
	[[maybe_unused]] void SetUp(const XMFLOAT3& up) { this->up_ = up; }
	[[maybe_unused]] void SetDistance(float distance) { this->distance_ = distance; }

private:
	XMMATRIX matView_;
	XMMATRIX matProjection_;
	XMFLOAT3 eye_;
	XMFLOAT3 target_;
	XMFLOAT3 up_;
	XMMATRIX viewProjection_;
	float distance_ = 50.0f;
};