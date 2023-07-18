#include "Camera.h"
#include "WinApp.h"

using namespace DirectX;

/// <summary>
/// 静的メンバ変数の実体
/// </summary>
//ID3D12Device* Camera::device = nullptr;

Camera::~Camera()
{
}

void Camera::Initialize()
{
	eye = { 0, 0, -distance }; // 視点座標
	target = { 0, 0, 0 };      // 注視点座標
	up = { 0, 1, 0 };          // 上方向ベクトル

	UpdateViewMatrix();
	UpdateProjectionMatrix();
	CreateConstBuff();
	ConstBuffMap();
	UpdateConstBuff();

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
	matView.ViewMat(eye, target, up);
}

void Camera::UpdateProjectionMatrix()
{
	// 射影行列
	matProjection.ProjectionMat(ConvertToRadians(60.0f), (float)WinApp::window_width / WinApp::window_height, 0.1f, 1000.0f);
}

void Camera::CreateConstBuff()
{
	HRESULT result;

	// ヒープ設定
	CD3DX12_HEAP_PROPERTIES heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// リソース設定
	CD3DX12_RESOURCE_DESC resDesc = CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataTransform) + 0xff) & ~0xff);

	// 定数バッファの生成
	result = DirectXCommon::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffTransform));
}

void Camera::ConstBuffMap()
{
	HRESULT result = constBuffTransform->Map(0, nullptr, (void**)&constMap);
	assert(SUCCEEDED(result));
}

void Camera::UpdateConstBuff()
{
	constMap->viewproj = matView;
	constMap->world = matProjection;
	constMap->cameraPos = eye;
}
