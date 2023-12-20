#include "ViewProjection.h"

#include "DirectXCommon.h"
#include <d3dx12.h>

void ViewProjection::Initialize()
{
	CreateConstBuffer();
	Map();
	UpdateMatrix();
}

void ViewProjection::CreateConstBuffer()
{
	// ヒーププロパティ
	CD3DX12_HEAP_PROPERTIES heapProps = 
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

	// リソース設定
	CD3DX12_RESOURCE_DESC resourceDesc =
		CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff);

	// 定数バッファの生成
	HRESULT result = DirectXCommon::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProps,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff_)
	);
	assert(SUCCEEDED(result));
}

void ViewProjection::Map()
{
	HRESULT result = constBuff_->Map(0, nullptr, (void**)&constMap);
	assert(SUCCEEDED(result));
}

void ViewProjection::UpdateMatrix()
{
	// 定数バッファへデータ転送
	if (SUCCEEDED(result)) {
		constMap->ambient_ = material_.ambient_;
		constMap->diffuse_ = material_.diffuse_;
		constMap->specular_ = material_.specular_;
		constMap->alpha_ = material_.alpha_;
	}
}