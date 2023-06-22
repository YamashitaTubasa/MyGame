#include "PostEffect.h"

#include <d3dx12.h>

using namespace DirectX;

PostEffect::PostEffect()
	: Sprite(
		100, // テクスチャ番号
		{ 0,0 }, // 座標
		{ 500.0f,500.0f }, // サイズ
		{ 1,1,1,1 }, // 色
		{ 0.0f,0.0f }, // アンカーポイント
		false, // 左右反転フラグ
		false) // 上下反転フラグ
{
}

void PostEffect::Draw(ID3D12GraphicsCommandList* cmdList)
{
	//this->cmdList = cmdList_;

	//// 頂点バッファをセット
	//cmdList->IASetVertexBuffers(0, 1, &vbView);

	//// 定数バッファ(CBV)をセット
	//cmdList->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());

	////シェーダーリソースビューをセット
	//cmdList->SetGraphicsRootDescriptorTable(
	//	1, CD3DX12_GPU_DESCRIPTOR_HANDLE(
	//		spriteCommon.descHeap->GetGPUDescriptorHandleForHeapStart(),
	//		texNumber,
	//		dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));

	////ポリゴンの描画(4頂点で四角形)
	//cmdList->DrawInstanced(4, 1, 0, 0);
}
