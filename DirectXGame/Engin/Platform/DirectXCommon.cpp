#include "DirectXCommon.h"
#include <vector>
#include <cassert>
#include <vector>
#include <string>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

using namespace Microsoft::WRL;

DirectXCommon* DirectXCommon::GetInstance()
{
	static DirectXCommon instance;

	return &instance;
}

void DirectXCommon::Initialize(WinApp* winApp) 
{
	// NULL検出
	assert(winApp);

	// メンバ変数に記録
	this->winApp = winApp;

	// FPS固定初期化
	fpsFixed = new FPSFixed();
	fpsFixed->InitializeFixFPS();
	// デバイスの生成
	InitializeDevice();
	// コマンド関連の初期化
	InitializeCommand();
	// スワップチェーンの初期化
	InitializeSwapchain();
	// レンダーターゲットビューの初期化
	InitializeRenderTargetView();
	// 深度バッファの初期化
	InitializeDepthBuffer();
	// フェンスの初期化
	InitializeFence();
}

void DirectXCommon::InitializeDevice()
{

#ifdef _DEBUG
	// デバックレイヤーをオンに
	ID3D12Debug1* debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
		debugController->EnableDebugLayer();
		debugController->SetEnableGPUBasedValidation(TRUE);
	}

#endif

	// DXGIファクトリーの生成
	result = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));
	assert(SUCCEEDED(result));

	// アダプターの列挙用
	std::vector<IDXGIAdapter4*> adapters;
	// ここに特定の名前を持つアダプターオブジェクトが入る
	IDXGIAdapter4* tmpAdapter = nullptr;

	// パフォーマンスが高いものから順に、全てのアダプターを列挙する
	for (UINT i = 0;
		dxgiFactory->EnumAdapterByGpuPreference(i,
			DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
			IID_PPV_ARGS(&tmpAdapter)) != DXGI_ERROR_NOT_FOUND;
		i++) {
		// 動的配列に追加する
		adapters.push_back(tmpAdapter);
	}

	// 妥当なアダプターを選別する
	for (size_t i = 0; i < adapters.size(); i++) {
		DXGI_ADAPTER_DESC3 adapterDesc;
		// アダプターの情報を取得する
		adapters[i]->GetDesc3(&adapterDesc);

		// ソフトウェアデバイスを回避
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)) {
			// デバイスを採用してループを抜ける
			tmpAdapter = adapters[i];
			break;
		}
	}

	// 対応レベルの配列
	D3D_FEATURE_LEVEL levels[] = {
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};

	D3D_FEATURE_LEVEL featureLevel;

	for (size_t i = 0; i < _countof(levels); i++) {
		// 採用したアダプターでデバイスを生成
		result = D3D12CreateDevice(tmpAdapter, levels[i],
			IID_PPV_ARGS(&device));
		if (result == S_OK) {
			// デバイスを生成できた時点でループを抜ける
			featureLevel = levels[i];
			break;
		}
	}

#ifdef _DEBUG
	ComPtr<ID3D12InfoQueue> infoQueue;
	if (SUCCEEDED(device->QueryInterface(IID_PPV_ARGS(&infoQueue)))) {
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true); // やばいエラー時に止まる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);      // エラー時に止まる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);    // 警告時に止まる

		// 抑制するエラー
		D3D12_MESSAGE_ID denyIds[] = {
			/*
			* Windows11でのDXGIデバックレイヤーとDX12デバックレイヤーの相互作用バグによるエラーメッセージ
			* https://stackoverflow.com/questions/69805245/directx-12-application-is-crashing-in-windows-11
			*/
			D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE
		};
		// 抑制する表示レベル
		D3D12_MESSAGE_SEVERITY severities[] = { D3D12_MESSAGE_SEVERITY_INFO };
		D3D12_INFO_QUEUE_FILTER filter{};
		filter.DenyList.NumIDs = _countof(denyIds);
		filter.DenyList.pIDList = denyIds;
		filter.DenyList.NumSeverities = _countof(severities);
		filter.DenyList.pSeverityList = severities;
		// 指定したエラーの表示を抑制
		infoQueue->PushStorageFilter(&filter);

		//infoQueue->Release();
	}
#endif
}

void DirectXCommon::InitializeCommand()
{
	// コマンドアロケーターを生成
	result = device->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&commandAllocator));
	assert(SUCCEEDED(result));

	// コマンドリストを生成
	result = device->CreateCommandList(0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		commandAllocator.Get(), nullptr,
		IID_PPV_ARGS(&commandList));
	assert(SUCCEEDED(result));

	// コマンドキューの設定
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};

	// コマンドキューを生成
	result = device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&commandQueue));
	assert(SUCCEEDED(result));
}

void DirectXCommon::InitializeSwapchain()
{
	// スワップチェーンの設定
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	swapChainDesc.Width = 1280;
	swapChainDesc.Height = 720;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;           // 色情報の書式
	swapChainDesc.SampleDesc.Count = 1;                          // マルチサンプルしない
	swapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;          // バックバッファ用
	swapChainDesc.BufferCount = 2;                               // バッファ数を2つに設定
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;    // フリップ用は破棄
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// IDXGISwapChain1のComptrを用意
	ComPtr<IDXGISwapChain1> swapChain1;

	// スワップチェーンの生成
	result = dxgiFactory->CreateSwapChainForHwnd(
		commandQueue.Get(), winApp->GetHwnd(), &swapChainDesc, nullptr, nullptr,
		&swapChain1
	);
	// 生成したIDXGISwapChain1のオブジェクトをIDXGISwapChain4に変換する
	swapChain1.As(&swapChain);
	assert(SUCCEEDED(result));
}

void DirectXCommon::InitializeRenderTargetView()
{
	DXGI_SWAP_CHAIN_DESC swcDesc = {};
    result = swapChain->GetDesc(&swcDesc);
    assert(SUCCEEDED(result));

	// スワップチェーンの設定
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	swapChainDesc.Width = 1280;
	swapChainDesc.Height = 720;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;            // 色情報の書式
	swapChainDesc.SampleDesc.Count = 1;                           // マルチサンプルしない
	swapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;           // バックバッファ用
	swapChainDesc.BufferCount = 2;                                // バッファ数を2つに設定
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;     // フリップ用は破棄
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;      // レンゲーターゲットレビュー
	rtvHeapDesc.NumDescriptors = swapChainDesc.BufferCount; // 裏表の2つ

	rtvHD = device->GetDescriptorHandleIncrementSize(rtvHeapDesc.Type);

	// デスクリプタヒープの生成
	device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&rtvHeap));

	// バックバッファ
	backBuffers.resize(swapChainDesc.BufferCount);

	// 裏表の2つ分
	for (int i = 0; i < 2; i++) {
		// スワップチェーンの全てのバッファについて処理する
		for (size_t i = 0; i < backBuffers.size(); i++) {
			// スワップチェーンからバッファを取得
			swapChain->GetBuffer((UINT)i, IID_PPV_ARGS(&backBuffers[i]));
			// デクリプタヒープからバッファを取得
			D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
			// 裏か表かでアドレスがずれる
			rtvHandle.ptr += i * device->GetDescriptorHandleIncrementSize(rtvHeapDesc.Type);
			// レンダーターゲットレビューの設定
			D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
			// シェーダーの計算結果をSRGBに変換して書き込む
			rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
			rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
			// レンダーターゲットレビューの生成
			device->CreateRenderTargetView(backBuffers[i].Get(), &rtvDesc, rtvHandle);
		}
	}
}

void DirectXCommon::InitializeDepthBuffer()
{
	// リソース設定
	D3D12_RESOURCE_DESC depthResourceDesc{};
	depthResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthResourceDesc.Width = WinApp::window_width;                    // レンダーターゲットに合わせる
	depthResourceDesc.Height = WinApp::window_height;                  // レンダーターゲットに合わせる
	depthResourceDesc.DepthOrArraySize = 1;
	depthResourceDesc.Format = DXGI_FORMAT_D32_FLOAT;                  // 深度フォーマット
	depthResourceDesc.SampleDesc.Count = 1;
	depthResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL; // テプスステンシル

	// 深度値用ヒーププロパティ
	D3D12_HEAP_PROPERTIES depthHeapProp{};
	depthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
	// 深度値のクリア設定
	D3D12_CLEAR_VALUE depthClearValue{};            // 深度値1.0f(最大値)でクリア
	depthClearValue.Format = DXGI_FORMAT_D32_FLOAT; // 深度値フォーマット
	depthClearValue.DepthStencil.Depth = 1.0f;

	// リソース設定
	ID3D12Resource* depthBuff = nullptr;
	result = device->CreateCommittedResource(
		&depthHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&depthResourceDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&depthClearValue,
		IID_PPV_ARGS(&depthBuff));

	// 深度ビュー用デスクリプタヒープ作成
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1;// 深度ビューは1つ
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	/*ID3D12DescriptorHeap* dsvHeap = nullptr;*/
	result = device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap));

	// 深度ビュー作成
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	device->CreateDepthStencilView(
		depthBuff,
		&dsvDesc,
		dsvHeap->GetCPUDescriptorHandleForHeapStart());
}

void DirectXCommon::InitializeFence()
{
	result = device->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
}

void DirectXCommon::PreDraw()
{
	// バックバッファの番号取得(２つなので0番か1番)
	UINT bbIndex = swapChain->GetCurrentBackBufferIndex();

	// 1.リソースバリアで書き込み可能に変更
	D3D12_RESOURCE_BARRIER barrierDesc{};
	barrierDesc.Transition.pResource = backBuffers[bbIndex].Get();// バックバッファを指定
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;// 表示状態から
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;// 描画状態へ
	commandList->ResourceBarrier(1, &barrierDesc);
	//commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(backBuffers[bbIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

	// 2.描画先の変更
	// レンダーターゲットビューのハンドルを取得
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle =
		CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeap->GetCPUDescriptorHandleForHeapStart(), bbIndex, rtvHD);
	/*rtvHandle.ptr += bbIndex * rtvH;*/
	// 深度ステンシルビュー用デスクリプタヒープのハンドルを取得
	CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle = 
		CD3DX12_CPU_DESCRIPTOR_HANDLE(dsvHeap->GetCPUDescriptorHandleForHeapStart());
	// 描画先指定
	commandList->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);

	// 3.描画クリア　　　　　
	// 画面を塗るつぶす色    R    G      B     A
	FLOAT clearcolor[] = { 0.1f, 0.25f, 0.5f, 0.0f };// 青っぽい色
	// レンダーターゲットクリア
	commandList->ClearRenderTargetView(rtvHandle, clearcolor, 0, nullptr);
	// 震度バッファクリア
	commandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	//if (input->Pushkey(DIK_SPACE)) // スペースキーが押されていたら
	//{
	//	clearcolor[0] = { 0.7f }; // 青っぽい色
	//	clearcolor[1] = { 0.5f };
	//	clearcolor[2] = { 0.3f };
	//	clearcolor[3] = { 0.0f };
	//}

	/*bool キーを押した状態か(uint8_t キー番号);
	bool キーを離した状態か(uint8_t キー番号);
	bool キーを押した瞬間か(uint8_t キー番号);
	bool キーを離した瞬間か(uint8_t キー番号);*/

	// 4.描画コマンドはここから
	// ビューポート設定コマンド
	D3D12_VIEWPORT viewport{};
	viewport.Width = WinApp::window_width;
	viewport.Height = WinApp::window_height;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	// ビューポート設定コマンドを、コマンドリストに積む
	commandList->RSSetViewports(1, &viewport);
	//commandList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, WinApp::window_width, WinApp::window_height));

	// シザー矩形
	D3D12_RECT scissorRect{};
	scissorRect.left = 0;// 切り抜き座標左
	scissorRect.right = scissorRect.left + WinApp::window_width;// 切り抜き座標右
	scissorRect.top = 0;// 切り抜き座標上
	scissorRect.bottom = scissorRect.top + WinApp::window_height;// 切り抜き座標下
	// シザー矩形設定コマンドを、コマンドリストに積む
	commandList->RSSetScissorRects(1, &scissorRect);
	//commandList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, WinApp::window_width, WinApp::window_height));
}

void DirectXCommon::PostDraw()
{
	// バックバッファの番号取得(２つなので0番か1番)
	UINT bbIndex = swapChain->GetCurrentBackBufferIndex();

	// 5.リソースバリアを戻す
	D3D12_RESOURCE_BARRIER barrierDesc{};
	barrierDesc.Transition.pResource = backBuffers[bbIndex].Get();           // バックバッファを指定
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET; // 表示状態から
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;        // 描画状態へ
	commandList->ResourceBarrier(1, &barrierDesc);

	// 命令のクローズ
	result = commandList->Close();
	assert(SUCCEEDED(result));

	// コマンドリストの実行
	ID3D12CommandList* commandLists[] = { commandList.Get()};
	commandQueue->ExecuteCommandLists(1, commandLists);

	// 画面に表示するバッファをクリップ(裏表の入れ替え)
	result = swapChain->Present(1, 0);
	assert(SUCCEEDED(result));

	// コマンドの実行完了を持つ
	commandQueue->Signal(fence.Get(), ++fenceVal);
	if (fence->GetCompletedValue() != fenceVal) {
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		fence->SetEventOnCompletion(fenceVal, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}

	// FPS固定
	fpsFixed->UpdateFixFPS();

	// キューをクリア
	result = commandAllocator->Reset();
	assert(SUCCEEDED(result));

	// 再びコマンドリストを貯める準備
	result = commandList->Reset(commandAllocator.Get(), nullptr);
	assert(SUCCEEDED(result));
}

void DirectXCommon::fpsFixedFinalize(){
	safe_delete(fpsFixed);
}
