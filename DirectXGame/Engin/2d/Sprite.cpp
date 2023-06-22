#include "Sprite.h"

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace std;

Sprite::Sprite(UINT texNumber, XMFLOAT2 pos, XMFLOAT2 size, XMFLOAT4 color, XMFLOAT2 anchorpoint, bool isFlipX, bool isFlipY) {

}

Sprite::~Sprite() {

}

PipelineSet Sprite::SpriteCreateGraphicsPipeline(ID3D12Device* device) 
{
	HRESULT result;
	ComPtr<ID3DBlob> vsBlob; // 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> psBlob; // ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob; // エラーオブジェクト

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/Shaders/SpriteVS.hlsl", // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "vs_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&vsBlob, &errorBlob);

	// エラーなら
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string error;
		error.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(error.c_str());
		assert(0);
	}

	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/Shaders/SpritePS.hlsl",   // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "ps_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&psBlob, &errorBlob);

	// エラーなら
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string error;
		error.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(error.c_str());
		assert(0);
	}

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ // xyz座標(1行で書いたほうが見やすい)
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},

		{ // uv座標(1行で書いたほうが見やすい)
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

	// デスクリプタレンジの設定
	CD3DX12_DESCRIPTOR_RANGE descriptorRange{};
	descriptorRange.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

	CD3DX12_ROOT_PARAMETER rootParams[2];
	rootParams[0].InitAsConstantBufferView(0);
	rootParams[1].InitAsDescriptorTable(1, &descriptorRange, D3D12_SHADER_VISIBILITY_ALL);

	// テクスチャサンプラーの設定
	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP; //横繰り返し（タイリング）
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP; //縦繰り返し（タイリング）
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP; //奥行繰り返し（タイリング）
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK; //ボーダーの時は黒
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR; //全てリニア補間
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX; //ミップマップ最大値
	samplerDesc.MinLOD = 0.0f; //ミップマップ最小値
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; //ピクセルシェーダからのみ使用可能

	// グラフィックスパイプライン設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	// シェーダーの設定
	pipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
	pipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();

	// サンプルマスクの設定
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定

	// ラスタライザの設定
	pipelineDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;  // カリングしない
	//pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // ポリゴン内塗りつぶし
	//pipelineDesc.RasterizerState.DepthClipEnable = true; // 深度クリッピングを有効に

	// レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL; // RBGA全てのチャンネルを描画
	blenddesc.BlendEnable = true;                   // ブレンドを有効にする
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;    // 加算
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;      // ソースの値を100% 使う
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;    // デストの値を  0% 使う

	// 半透明合成
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;             // 加算
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;         // ソースのアルファ値
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;    // 1.0f-ソースのアルファ値

	// 頂点レイアウトの設定
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc.InputLayout.NumElements = _countof(inputLayout);

	// 図形の形状設定
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	// その他の設定
	pipelineDesc.NumRenderTargets = 1; // 描画対象は1つ
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0～255指定のRGBA
	pipelineDesc.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	// デプスステンシルステートの設定
	pipelineDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS; // 常に上書きルール
	pipelineDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	pipelineDesc.DepthStencilState.DepthEnable = false;

	// ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams; // ルートパラメータの先頭アドレス
	rootSignatureDesc.NumParameters = _countof(rootParams); // ルートパラメータ数
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	// パイプラインとルートシグネチャのセット
	PipelineSet pipelineSet;

	ComPtr<ID3DBlob> rootSigBlob;
	// バージョン自動判定でのシリアライズ
	result = D3D12SerializeRootSignature(
		&rootSignatureDesc, 
		D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, 
		&errorBlob);
	assert(SUCCEEDED(result));
	// ルートシグネチャの生成
	result = device->CreateRootSignature(
		0, 
		rootSigBlob->GetBufferPointer(), 
		rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&pipelineSet.rootsignature));
	assert(SUCCEEDED(result));

	// パイプラインにルートシグネチャをセット
	pipelineDesc.pRootSignature = pipelineSet.rootsignature.Get();

	// パイプランステートの生成
	result = device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineSet.pipelinestate));
	assert(SUCCEEDED(result));

	// デスクリプタヒープを生成	
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダから見えるように
	descHeapDesc.NumDescriptors = 1; // シェーダーリソースビュー1つ
	result = device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descHeap));//生成
	if (FAILED(result)) {
		assert(0);
	}

	// パイプラインとルートシグネチャを返す
	return pipelineSet;
}

void Sprite::SpriteCreate(ID3D12Device* dev, float window_width, float window_height, 
	UINT texNumber, const SpriteCommon& spriteCommon, XMFLOAT2 anchorpoint, bool isFlipX, bool isFlipY) {

	HRESULT result = S_FALSE;

	//頂点データ
	VertexPosUv vertices[] = {
		{{	0.0f, 100.0f,	0.0f},{0.0f,1.0f}},
		{{	0.0f,	0.0f,	0.0f},{0.0f,0.0f}},
		{{100.0f, 100.0f,	0.0f},{1.0f,1.0f}},
		{{100.0f,	0.0f,	0.0f},{1.0f,0.0f}},
	};

	UINT sizeVB = static_cast<UINT>(sizeof(VertexPosUv) * _countof(vertices));

	// ヒーププロパティ
	CD3DX12_HEAP_PROPERTIES heapPropsVertexBuffer = 
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// リソース設定
	CD3DX12_RESOURCE_DESC resourceDescVertexBuffer =
		CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices));

	// テクスチャ番号をコピー
	this->texNumber = texNumber;

	//頂点バッファ生成
	result = dev->CreateCommittedResource(
		&heapPropsVertexBuffer, // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resourceDescVertexBuffer, // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

	// 指定番号の画像が読み込み済みなら
	if (spriteCommon.texBuff[this->texNumber]) {
		// テクスチャ情報取得
		D3D12_RESOURCE_DESC resDesc = spriteCommon.texBuff[this->texNumber]->GetDesc();
		// スプライトの大きさを画像の解像度に合わせる
		scale = { (float)resDesc.Width,(float)resDesc.Height };
		// テクスチャ情報取得
	}

	texSize_ = { (float)resDesc.Width, (float)resDesc.Height };

	// アンカーポイントをコピー
	this->anchorpoint = anchorpoint;

	// 反転フラグをコピー
	this->isFlipX_ = isFlipX;
	this->isFlipY_ = isFlipY;

	//頂点バッファへのデータ転送
	VertexPosUv* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	memcpy(vertMap, vertices, sizeof(vertices));
	vertBuff->Unmap(0, nullptr);

	// 頂点バッファビューの作成
	// GPU仮想アドレス
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	// 頂点バッファのサイズ
	vbView.SizeInBytes = sizeof(vertices);
	// 頂点1つ分のデータサイズ
	vbView.StrideInBytes = sizeof(vertices[0]);

	// ヒーププロパティ
	CD3DX12_HEAP_PROPERTIES heapPropsConstantBuffer = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// リソース設定
	CD3DX12_RESOURCE_DESC resourceDescConstantBuffer =
		CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff);

	// 定数バッファの生成
	result = dev->CreateCommittedResource(
		&heapPropsConstantBuffer, // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resourceDescConstantBuffer, // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));
	assert(SUCCEEDED(result));

	// 定数バッファにデータ転送
	ConstBufferData* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap); // マッピング
	constMap->color = color_;
	assert(SUCCEEDED(result));

	//平行投影行列
	constMap->mat = XMMatrixOrthographicOffCenterLH(0.0f, window_width, window_height, 0.0f, 0.0f, 1.0f);
	constBuff->Unmap(0, nullptr);
}

void Sprite::PreDraw(ID3D12GraphicsCommandList* cmdList, const SpriteCommon& spriteCommon) {

	// パイプラインステートとルートシグネチャの設定
	cmdList->SetPipelineState(spriteCommon.pipelineSet.pipelinestate.Get());
	cmdList->SetGraphicsRootSignature(spriteCommon.pipelineSet.rootsignature.Get());

	// プリミティブ形状の設定
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP); // 三角形リスト

	//テクスチャ用でスクリプタヒープの設定
	ID3D12DescriptorHeap* ppHeaps[] = { spriteCommon.descHeap.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
}

void Sprite::PostDraw()
{
}

void Sprite::SpriteDraw(ID3D12GraphicsCommandList* cmdList_, const SpriteCommon& spriteCommon, 
	ID3D12Device* dev,D3D12_VERTEX_BUFFER_VIEW& vbView) 
{
	// 非表示フラグがtrueなら
	if (isInvisible) {
		// 描画せずに抜ける
		return;
	}

	this->cmdList = cmdList_;

	// 頂点バッファをセット
	cmdList->IASetVertexBuffers(0, 1, &vbView);

	// 定数バッファ(CBV)をセット
	cmdList->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());

	//シェーダーリソースビューをセット
	cmdList->SetGraphicsRootDescriptorTable(
		1, CD3DX12_GPU_DESCRIPTOR_HANDLE(
			spriteCommon.descHeap->GetGPUDescriptorHandleForHeapStart(),
			texNumber, 
			dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));

	//ポリゴンの描画(4頂点で四角形)
	cmdList->DrawInstanced(4, 1, 0, 0);
}

SpriteCommon Sprite::SpriteCommonCreate(ID3D12Device* dev, int window_width, int window_height) 
{
	HRESULT result = S_FALSE;

	// 新たなスプライト共通データを生成
	SpriteCommon spriteCommon{};

	// スプライト用パイプライン生成
	spriteCommon.pipelineSet = SpriteCreateGraphicsPipeline(dev);

	// 平行投影行列生成
	spriteCommon.matProjection = XMMatrixOrthographicOffCenterLH(
		0.0f, (float)window_width, (float)window_height, 0.0f, 0.0f, 1.0f);

	// デスクリプタヒープを生成
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NumDescriptors = SpriteCommon::kMaxSRVCount;
	result = dev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&spriteCommon.descHeap));

	// 生成したスプライトを返す
	return spriteCommon;
}

void Sprite::SpriteUpdate(Sprite* sprite, const SpriteCommon& spriteCommon) 
{
	// ワールド行列の更新
	sprite->matWorld = XMMatrixIdentity();
	// Z軸回転
	sprite->matWorld *= XMMatrixRotationZ(XMConvertToRadians(sprite->rotation));
	// 平行移動
	sprite->matWorld *= XMMatrixTranslation(sprite->position.x, sprite->position.y, 0.0f);

	// 定数バッファの転送
	ConstBufferData* constMap = nullptr;
	HRESULT result = sprite->constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->mat = sprite->matWorld * spriteCommon.matProjection;
	sprite->constBuff->Unmap(0, nullptr);
}

void Sprite::LoadTexture(SpriteCommon& spriteCommon, UINT texnumber, const wchar_t* filename, ID3D12Device* dev) {

	assert(texnumber <= SpriteCommon::kMaxSRVCount - 1);

	HRESULT result;
	TexMetadata metadata{};
	ScratchImage scratchImg{};

	// WICテクスチャのロード
	result = LoadFromWICFile(filename, WIC_FLAGS_NONE, &metadata, scratchImg);
	assert(SUCCEEDED(result));

	ScratchImage mipChain{};
	// ミップマップ生成
	result = GenerateMipMaps(
		scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mipChain);
	if (SUCCEEDED(result)) {
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}

	// 読み込んだディフューズテクスチャをSRGBとして扱う
	metadata.format = MakeSRGB(metadata.format);

	// リソース設定
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format, metadata.width, (UINT)metadata.height, (UINT16)metadata.arraySize,
		(UINT16)metadata.mipLevels);

	// ヒーププロパティ
	CD3DX12_HEAP_PROPERTIES heapProps =
		CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);

	// テクスチャ用バッファの生成
	result = dev->CreateCommittedResource(
		&heapProps, D3D12_HEAP_FLAG_NONE, &texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, // テクスチャ用指定
		nullptr, IID_PPV_ARGS(&spriteCommon.texBuff[texnumber]));
	assert(SUCCEEDED(result));

	// テクスチャバッファにデータ転送
	for (size_t i = 0; i < metadata.mipLevels; i++) {
		const Image* img = scratchImg.GetImage(i, 0, 0); // 生データ抽出
		result = spriteCommon.texBuff[texnumber]->WriteToSubresource(
			(UINT)i,
			nullptr, // 全領域へコピー
			img->pixels, // 元データアドレス
			(UINT)img->rowPitch, // 1ラインサイズ
			(UINT)img->slicePitch // 1枚サイズ
		);
		assert(SUCCEEDED(result));
	}

	// シェーダリソースビュー作成
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // 設定構造体
	D3D12_RESOURCE_DESC resDesc = spriteCommon.texBuff[texnumber]->GetDesc();

	srvDesc.Format = resDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;// 2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;

	dev->CreateShaderResourceView(spriteCommon.texBuff[texnumber].Get(), // ビューと関連付けるバッファ
		&srvDesc, // テクスチャ設定情報
		CD3DX12_CPU_DESCRIPTOR_HANDLE(spriteCommon.descHeap->GetCPUDescriptorHandleForHeapStart(), 
		texnumber, 
		dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV))
	);
}

void Sprite::SpriteTransferVertexBuffer(const Sprite* sprite, const SpriteCommon& spriteCommon, uint32_t texIndex)
{
	HRESULT result = S_FALSE;

	this->texIndex_ = texIndex;

	// 頂点データ
	VertexPosUv vertices[] = {
		//    u    v
		{{},{0.0f,1.0f}}, // 左下
		{{},{0.0f,0.0f}}, // 左上
		{{},{1.0f,1.0f}}, // 右下
		{{},{1.0f,0.0f}}, // 右上
	};

	//  左下、左上、右下、右上
	enum { LB, LT, RB, RT };

	// 左右上下端の座標計算
	float left = (0.0f - anchorpoint.x) * scale.x;
	float right = (1.0f - anchorpoint.x) * scale.x;
	float top = (0.0f - anchorpoint.y) * scale.y;
	float bottom = (1.0f - anchorpoint.y) * scale.y;

	if (isFlipX_ = false) 
	{// 左右入れ替え
		left = -left;
		right = -right;
	}

	if (isFlipY_ = false)
	{// 左右入れ替え
		top = -top;
		bottom = -bottom;
	}

	// 頂点データ配列に座標セット
	vertices[LB].pos = { left,  bottom, 0.0f }; // 左下
	vertices[LT].pos = { left,     top, 0.0f }; // 左上
	vertices[RB].pos = { right, bottom, 0.0f }; // 右下
	vertices[RT].pos = { right,    top, 0.0f }; // 右上

	// UV計算
	// 指定番号の画像が読み込み済みなら
	if (spriteCommon.texBuff[sprite->texIndex_]) {
		// テクスチャ情報取得
		resDesc = spriteCommon.texBuff[sprite->texIndex_]->GetDesc();

		float tex_left = sprite->texLeftTop_.x / resDesc.Width;
		float tex_right = (sprite->texLeftTop_.x + sprite->texSize_.x) / resDesc.Width;
		float tex_top = sprite->texLeftTop_.y / resDesc.Height;
		float tex_bottom = (sprite->texLeftTop_.y + sprite->texSize_.y) / resDesc.Height;

		vertices[LB].uv = { tex_left,  tex_bottom }; // 左下
		vertices[LT].uv = { tex_left,     tex_top }; // 左上
		vertices[RB].uv = { tex_right, tex_bottom }; // 右下
		vertices[RT].uv = { tex_right,    tex_top }; // 右上
	}

	// 頂点バッファへのデータ転送
	VertexPosUv* vertMap = nullptr;
	result = sprite->vertBuff->Map(0, nullptr, (void**)&vertMap);
	memcpy(vertMap, vertices, sizeof(vertices));
	sprite->vertBuff->Unmap(0, nullptr);
}

void Sprite::Finalize()
{
}
