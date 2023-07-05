#include "Camera.h"
#include "WinApp.h"

using namespace DirectX;

void Camera::Initialize() 
{
	eye = { 0, 0, -distance }; // 視点座標
	target = { 0, 0, 0 };      // 注視点座標
	up = { 0, 1, 0 };          // 上方向ベクトル

	UpdateViewMatrix();
	UpdateProjectionMatrix();

	viewProjection = matView * matProjection;
}

void Camera::Update()
{
	UpdateViewMatrix();
	UpdateProjectionMatrix();

	viewProjection = matView * matProjection;
}

void Camera::UpdateViewMatrix()
{
	// ビュー行列の計算
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
}

void Camera::UpdateProjectionMatrix()
{
	// 透視投影
	matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(60.0f),
		(float)WinApp::window_width / WinApp::window_height, 0.1f, 1000.0f
	);

	matProjection = Transform()
}