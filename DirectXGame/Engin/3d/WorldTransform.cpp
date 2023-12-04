/**
 * @file WorldTransform.cpp
 * @brief ワールドトランスフォームクラス
 * @author Yamashita Tubasa
 */

#include "WorldTransform.h"

void WorldTransform::Initialize()
{
	CreateConstBuffer();
	Map();
	UpdateMatrix();
}

void WorldTransform::CreateConstBuffer()
{
	HRESULT result;

	// ヒーププロパティ
	CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// リソース設定
	CD3DX12_RESOURCE_DESC resourceDesc =
		CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff);

	// 定数バッファの生成
	result = DirectXCommon::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProps, // アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffer_));
	assert(SUCCEEDED(result));
}

void WorldTransform::Map()
{
	// 定数バッファとのデータリンク
	HRESULT result;
	result = constBuffer_->Map(0, nullptr, (void**)&constMap_);
	assert(SUCCEEDED(result));
}

void WorldTransform::UpdateMatrix()
{
	Matrix4 matScale, matRot, matTrans;
	Matrix4 m, matRotX, matRotY, matRotZ;

	// スケール、回転、平行移動行列の計算
	matScale = Matrix4::Identity();
	matScale.Scale(scale_);
	matRot = Matrix4::Identity();
	matRot *= matRotZ.RotateZ(ToRadian(rotation_.z));
	matRot *= matRotX.RotateX(ToRadian(rotation_.x));
	matRot *= matRotY.RotateY(ToRadian(rotation_.y));
	matTrans.Translate(position_);

	// ワールド行列の合成
	matWorld_ = Matrix4::Identity(); // 変形をリセット
	matWorld_ *= matScale;           // ワールド行列にスケーリングを反映
	matWorld_ *= matRot;             // ワールド行列に回転を反映
	matWorld_ *= matTrans;           // ワールド行列に平行移動を反映

	// 親オブジェクトがあれば
	if (parent_ != nullptr) {
		// 親オブジェクトのワールド行列を掛ける
		matWorld_ *= parent_->matWorld_;
	}

	// 定数バッファへデータ転送
	constMap_->mat_ = matWorld_;	 // 行列の合成
}