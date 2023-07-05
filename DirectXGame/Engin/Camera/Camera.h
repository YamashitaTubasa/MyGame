#pragma once

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"

#include <DirectXMath.h>
#include <wrl.h>

// カメラ
class Camera
{
public:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

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
	const Matrix4& GetMatView() { return matView; }
	const Matrix4& GetMatProjection() { return matProjection; }
	const Vector3& GetEye() { return eye; }
	const Vector3& GetTarget() { return target; }
	const Vector3& GetUp() { return up; }
	const float& GetDistance() { return distance; }
	const Matrix4& GetViewProjectionMatrix() { return viewProjection; }

	void SetMatView(const Matrix4& matView) { this->matView = matView; }
	void SetMatProjection(const Matrix4& matProjection) { this->matProjection = matProjection; }
	void SetEye(const Vector3& eye) { this->eye = eye; }
	void SetTarget(const Vector3& target) { this->target = target; }
	void SetUp(const Vector3& up_) { this->up = up_; }
	void SetDistance(float distance_) { this->distance = distance_; }

private:
	Matrix4 matView;
	Matrix4 matProjection;
	Vector3 eye;
	Vector3 target;
	Vector3 up;
	Matrix4 viewProjection;
	float distance = 50.0f;
};