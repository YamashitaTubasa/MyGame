/**
 * @file Camera.cpp
 * @brief カメラに関するクラス
 * @author Yamashita Tubasa
 */

#include "Camera.h"
#include "WinApp.h"

using namespace DirectX;

void Camera::Initialize() 
{
	eye_ = { 0, 0, -distance_ }; // 視点座標
	target_ = { 0, 0, 0 };      // 注視点座標
	up_ = { 0, 1, 0 };          // 上方向ベクトル

	UpdateViewMatrix();
	UpdateProjectionMatrix();

	viewProjection_ = matView_ * matProjection_;
}

void Camera::Update()
{
	UpdateViewMatrix();
	UpdateProjectionMatrix();

	viewProjection_ = matView_ * matProjection_;
}

void Camera::UpdateViewMatrix()
{
	// ビュー行列の計算
	matView_ = XMMatrixLookAtLH(XMLoadFloat3(&eye_), XMLoadFloat3(&target_), XMLoadFloat3(&up_));
}

void Camera::UpdateProjectionMatrix()
{
	// 透視投影
	matProjection_ = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(60.0f),
		(float)MyEngine::WinApp::window_width / MyEngine::WinApp::window_height, 0.1f, 1000.0f
	);
}