/**
 * @file WorldTransform.cpp
 * @brief 定数バッファクラス
 * @author Yamashita Tubasa
 */

#include "WorldTransform.h"

void WorldTransform::Initialize()
{
	HRESULT result;

	// ヒーププロパティ
	CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// リソース設定
	CD3DX12_RESOURCE_DESC resourceDesc =
		CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff);

	// 定数バッファの生成
	result = device_->CreateCommittedResource(
		&heapProps, // アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff_));
	assert(SUCCEEDED(result));
}

void WorldTransform::Update()
{
	HRESULT result;

	Matrix4 matScale, matRot, matTrans;
	Matrix4 m, matRotX, matRotY, matRotZ;

	// スケール、回転、平行移動行列の計算
	matScale = m.Scale(scale_);
	matRot = Matrix4::Identity();
	matRot *= matRotX.RotateZ(ToRadian(rotation_.z));
	matRot *= matRotY.RotateX(ToRadian(rotation_.x));
	matRot *= matRotZ.RotateY(ToRadian(rotation_.y));
	matTrans = m.Translate(position_);

	// ワールド行列の合成
	matWorld_ = Matrix4::Identity(); // 変形をリセット
	matWorld_ *= matScale;           // ワールド行列にスケーリングを反映
	matWorld_ *= matRot;             // ワールド行列に回転を反映
	matWorld_ *= matTrans;           // ワールド行列に平行移動を反映

	// 定数バッファへデータ転送
	result = constBuff_->Map(0, nullptr, (void**)&constMap);
	constMap->mat_ = matWorld_;	// 行列の合成
	constBuff_->Unmap(0, nullptr);
}

void WorldTransform::Draw()
{
	// 定数バッファビューをセット
	cmdList_->SetGraphicsRootConstantBufferView(0, constBuff_->GetGPUVirtualAddress());
}
