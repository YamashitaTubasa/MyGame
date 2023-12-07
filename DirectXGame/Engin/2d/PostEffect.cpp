/**
 * @file PostEffect.cpp
 * @brief ポストエフェクトに関するクラス
 * @author Yamashita Tubasa
 */

#include "PostEffect.h"

#include <d3dx12.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;

// 静的メンバ変数の実体
//                                        Red  Green Blue  Alpha
//const float PostEffect::clearColor[4] = { 0.25, 0.5, 0.1f, 0.0f }; // 緑っぽい色
const float PostEffect::clearColor_[4] = { 0.1f, 0.25f, 0.5f, 0.0f };// 青っぽい色

PostEffect::PostEffect()
	: Sprite(
		100, // テクスチャ番号
		{ 0,0,0 }, // 座標
		{ 500.0f,500.0f }, // サイズ
		{ 1,1,1,1 }, // 色
		{ 0.0f,0.0f }, // アンカーポイント
		false, // 左右反転フラグ
		false) // 上下反転フラグ
{
}

PostEffect::~PostEffect()
{
}

void PostEffect::Initialize(const wchar_t* filename)
{
	dxCommon_ = DirectXCommon::GetInstance();

	HRESULT result;

	this->device_ = dxCommon_->GetDevice();

	// パイプライン生成
	CreateGraphicsPipelineState(filename);

	// テクスチャリソース設定
	CD3DX12_RESOURCE_DESC texresDesc =
		CD3DX12_RESOURCE_DESC::Tex2D(
			DXGI_FORMAT_R8G8B8A8_UNORM,
			WinApp::window_width,
			(UINT)WinApp::window_height,
			1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
		);

	CD3DX12_HEAP_PROPERTIES heapProperties =
		CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);
	CD3DX12_CLEAR_VALUE clearValue =
		CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, clearColor_);

	// テクスチャバッファの生成
	result = device_->CreateCommittedResource(
		&heapProperties,
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		&clearValue,
		IID_PPV_ARGS(&texBuff_));
	assert(SUCCEEDED(result));

	{// テクスチャを赤クリア
		// 画素数(1280 x 720 = 921600ピクセル)
		const UINT pixelCount = WinApp::window_width * WinApp::window_height;
		// 画像1桁分のデータサイズ
		const UINT rowPitch = sizeof(UINT) * WinApp::window_width;
		// 画像全体node-tasaizu
		const UINT depthPitch = rowPitch * WinApp::window_height;
		// 画像イメージ
		UINT* img = new UINT[pixelCount];
		for (int i = 0; i < pixelCount; i++) { img[i] = 0xff0000ff; }

		// テクスチャバッファにデータ転送
		result = texBuff_->WriteToSubresource(
			0, nullptr, img, rowPitch, depthPitch);
		assert(SUCCEEDED(result));
		delete[] img;
	}

	// SRV用デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC srvDescHeapDesc = {};
	srvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvDescHeapDesc.NumDescriptors = 1;
	// SRV用デスクリプタヒープを生成
	result = device_->CreateDescriptorHeap(&srvDescHeapDesc, IID_PPV_ARGS(&descHeapSRV_));
	assert(SUCCEEDED(result));
	descHeapSRV_->SetName(L"PostEffect[descHeapSRV]");
	// SRV設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // 設定構造体
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; // 2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;

	// デスクリプタヒープにSRV作成
	device_->CreateShaderResourceView(texBuff_.Get(), // ビューと関連付けるバッファ
		&srvDesc,
		descHeapSRV_->GetCPUDescriptorHandleForHeapStart()
	);

	// RTV用デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc{};
	rtvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDescHeapDesc.NumDescriptors = 1;
	// RTV用デスクリプタヒープを生成
	result = device_->CreateDescriptorHeap(&rtvDescHeapDesc, IID_PPV_ARGS(&descHeapRTV_));
	assert(SUCCEEDED(result));

	// レンダーターゲットレビューの設定
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	// シェーダーの計算結果をSRGBに変換して書き込む
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	// デスクリプタヒープにRTV作成
	device_->CreateRenderTargetView(texBuff_.Get(),
		&rtvDesc,
		descHeapRTV_->GetCPUDescriptorHandleForHeapStart()
	);
	descHeapRTV_->SetName(L"PostEffect[descHeapRTV]");
	// 深度バッファリソース設定
	CD3DX12_RESOURCE_DESC depthResDesc =
		CD3DX12_RESOURCE_DESC::Tex2D(
			DXGI_FORMAT_D32_FLOAT,
			WinApp::window_width,
			WinApp::window_height,
			1, 0,
			1, 0,
			D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
		);

	CD3DX12_HEAP_PROPERTIES heapPropertiesBuffer =
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	CD3DX12_CLEAR_VALUE clearValueBuffer =
		CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0);

	// 深度バッファの生成
	result = device_->CreateCommittedResource(
		&heapPropertiesBuffer,
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&clearValueBuffer,
		IID_PPV_ARGS(&depthBuff_));
	assert(SUCCEEDED(result));
	depthBuff_->SetName(L"PostEffect[depthBuff]");
	// DSV用デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC DescHeapDesc{};
	DescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	DescHeapDesc.NumDescriptors = 1;
	// DSV用デスクリプタヒープを作成
	result = device_->CreateDescriptorHeap(&DescHeapDesc, IID_PPV_ARGS(&descHeapDSV_));
	assert(SUCCEEDED(result));

	// デスクリプタヒープにDSV作成
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT; // 深度値フォーマット
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	device_->CreateDepthStencilView(depthBuff_.Get(),
		&dsvDesc,
		descHeapDSV_->GetCPUDescriptorHandleForHeapStart());
	descHeapDSV_->SetName(L"PostEffect[descHeapDSV]");
	CD3DX12_HEAP_PROPERTIES heapPropertiess =
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	CD3DX12_RESOURCE_DESC resourceDesc =
		CD3DX12_RESOURCE_DESC::Buffer(sizeof(VertexPosUv) * vertNum_);

	// 頂点バッファ生成
	result = device_->CreateCommittedResource(
		&heapPropertiess,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff_));
	assert(SUCCEEDED(result));

	// 頂点データ
	VertexPosUv vertices[vertNum_] = {
		{{-1.0f,-1.0f,0.0f},{0.0f,1.0f}},
		{{-1.0f,+1.0f,0.0f},{0.0f,0.0f}},
		{{+1.0f,-1.0f,0.0f},{1.0f,1.0f}},
		{{+1.0f,+1.0f,0.0f},{1.0f,0.0f}},
	};

	// 頂点バッファへのデータ転送
	VertexPosUv* vertMap = nullptr;
	result = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		memcpy(vertMap, vertices, sizeof(vertices));
		vertBuff_->Unmap(0, nullptr);
	}

	// 頂点バッファビューの作成
	// GPU仮想アドレス
	vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	// 頂点バッファのサイズ
	vbView_.SizeInBytes = sizeof(VertexPosUv) * 4;
	// 頂点1つ分のデータサイズ
	vbView_.StrideInBytes = sizeof(VertexPosUv);

	CD3DX12_HEAP_PROPERTIES heapPropertiesss =
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	CD3DX12_RESOURCE_DESC resourceDescss =
		CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) +
			0xff) & ~0xff);

	// 定数バッファの生成
	result = device_->CreateCommittedResource(
		&heapPropertiesss,
		D3D12_HEAP_FLAG_NONE,
		&resourceDescss,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff_));
	assert(SUCCEEDED(result));
	constBuff_->SetName(L"PostEffect[constBuff]");

	// 定数バッファにデータ転送
	result = constBuff_->Map(0, nullptr, (void**)&constMap_); // マッピング
	constMap_->color_ = color_;
	constMap_->isPostE_ = isPostE_;
	constMap_->alpha_ = alpha_;
	constMap_->mat_ = XMMatrixIdentity();
	assert(SUCCEEDED(result));
}

void PostEffect::Draw(ID3D12GraphicsCommandList* cmdList)
{
	// 非表示フラグがtrueなら
	if (isInvisible_) {
		// 描画せずに抜ける
		return;
	}

	this->cmdList_ = cmdList;

	// パイプラインステートとルートシグネチャの設定
	cmdList->SetPipelineState(pipelineState_.Get());
	cmdList->SetGraphicsRootSignature(rootSignature_.Get());
	rootSignature_->SetName(L"PostEffectRootSignature");

	// プリミティブ形状の設定
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP); // 三角形リスト

	// 頂点バッファをセット
	cmdList->IASetVertexBuffers(0, 1, &vbView_);

	// 定数バッファ(CBV)をセット
	cmdList->SetGraphicsRootConstantBufferView(0, constBuff_->GetGPUVirtualAddress());

	ID3D12DescriptorHeap* ppHeap[] = { descHeapSRV_.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeap), ppHeap);

	//シェーダーリソースビューをセット
	cmdList->SetGraphicsRootDescriptorTable(1, descHeapSRV_->GetGPUDescriptorHandleForHeapStart());

	//ポリゴンの描画(4頂点で四角形)
	cmdList->DrawInstanced(4, 1, 0, 0);
}

void PostEffect::PreDraw(ID3D12GraphicsCommandList* cmdList)
{
	CD3DX12_RESOURCE_BARRIER resourceBarrier =
		CD3DX12_RESOURCE_BARRIER::Transition(texBuff_.Get(),
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			D3D12_RESOURCE_STATE_RENDER_TARGET);

	// リソースバリアを変更(シェーダーリソース→描画可能)
	cmdList->ResourceBarrier(1, &resourceBarrier);

	// レンダーターゲットビュー用ディスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE rtvH =
		descHeapRTV_->GetCPUDescriptorHandleForHeapStart();
	// 深度ステンシルビュー用デスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH =
		descHeapDSV_->GetCPUDescriptorHandleForHeapStart();
	// レンダーターゲットをセット
	cmdList->OMSetRenderTargets(1, &rtvH, false, &dsvH);

	CD3DX12_VIEWPORT viewport =
		CD3DX12_VIEWPORT(0.0f, 0.0f, WinApp::window_width, WinApp::window_height);
	// ビューポートの設定
	cmdList->RSSetViewports(1, &viewport);

	CD3DX12_RECT rect =
		CD3DX12_RECT(0, 0, WinApp::window_width, WinApp::window_height);
	// シザリング矩形の設定
	cmdList->RSSetScissorRects(1, &rect);

	// 全画面クリア
	cmdList->ClearRenderTargetView(rtvH, clearColor_, 0, nullptr);

	// 深度バッファのクリア
	cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0,
		nullptr);
}

void PostEffect::PostDraw(ID3D12GraphicsCommandList* cmdList)
{
	CD3DX12_RESOURCE_BARRIER resourceBarriers =
		CD3DX12_RESOURCE_BARRIER::Transition(texBuff_.Get(),
			D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	// リソースバリアを変更(描画可能→シェーダーリソース)
	cmdList->ResourceBarrier(1, &resourceBarriers);
}

void PostEffect::CreateGraphicsPipelineState(const wchar_t* filename)
{
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob; // 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> psBlob; // ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob; // エラーオブジェクト

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/PostEffectVS.hlsl", // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "vs_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバック用指定
		0,
		&vsBlob, &errorBlob);

	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		assert(0);
	}

	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		filename, // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "ps_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバック用設定
		0,
		&psBlob, &errorBlob);

	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		assert(0);
	}


	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ // xy座標
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // uv座標
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

	// グラフィクスパイプラインの流れを設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	// サンプルマスク
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	// ラスタライザステート
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	// デプスステンシルステート
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS; // 常に上書きルール

	// レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL; // RGBA全てのチャンネルを描画
	blenddesc.BlendEnable = true;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// ブレンドステートの設定
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	// 深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// 頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// 図形の形状設定(三角形)
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 1; // 描画対象は1つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0～255指定のRGBA
	gpipeline.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	// デスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 レジスタ

	// ルートパラメータ
	CD3DX12_ROOT_PARAMETER rootparams[2];
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);

	// スタティックサンプラー
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0, D3D12_FILTER_MIN_MAG_MIP_POINT);

	// ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	// バージョン自動判定のシリアライズ
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result));

	// ルートシグネチャの生成
	result = dxCommon_->GetDevice()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature_));
	assert(SUCCEEDED(result));

	gpipeline.pRootSignature = rootSignature_.Get();
	rootSignature_->SetName(L"PostEffectRootSignature");

	// グラフィックスパイプラインの生成
	result = dxCommon_->GetDevice()->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelineState_));
	assert(SUCCEEDED(result));
}

void PostEffect::SetColor(const DirectX::XMFLOAT4& color)
{
	HRESULT result;

	// 定数バッファにデータ転送
	result = constBuff_->Map(0, nullptr, (void**)&constMap_); // マッピング
	constMap_->color_ = color;
	assert(SUCCEEDED(result));
}

void PostEffect::SetIsPostE(const bool& isPostE)
{
	HRESULT result;

	// 定数バッファにデータ転送
	result = constBuff_->Map(0, nullptr, (void**)&constMap_); // マッピング
	constMap_->isPostE_ = isPostE;
	assert(SUCCEEDED(result));
}

void PostEffect::SetAlpha(const float& alpha)
{
	HRESULT result;

	// 定数バッファにデータ転送
	result = constBuff_->Map(0, nullptr, (void**)&constMap_);
	constMap_->alpha_ = alpha;
	assert(SUCCEEDED(result));
}