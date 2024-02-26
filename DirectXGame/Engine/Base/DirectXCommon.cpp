/**
 * @file DirectXCommon.cpp
 * @brief DirectXの基盤クラス
 * @author Yamashita Tubasa
 */

#include "DirectXCommon.h"

#pragma warning(push)
#pragma warning(disable:4514)
#pragma warning(disable:4820)
#include <vector>
#include <cassert>
#include <vector>
#include <string>
#include <dxgidebug.h>
#pragma warning(pop)

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

/// <summary>
/// 静的メンバ変数の実体
/// </summary>
MyEngine::DirectXCommon* MyEngine::DirectXCommon::instance = nullptr;

MyEngine::DirectXCommon* MyEngine::DirectXCommon::GetInstance()
{
	if (instance == nullptr) 
	{
		instance = new DirectXCommon();
	}
	return instance;
}

void MyEngine::DirectXCommon::DeleteInstance()
{
	delete instance;

	instance = nullptr;
}

void MyEngine::DirectXCommon::Initialize(WinApp* winApp)
{
	// NULL検出
	assert(winApp);

	// メンバ変数に記録
	this->winApp_ = winApp;
	
	// FPS固定初期化
	fpsFixed_ = new FPSFixed();
	fpsFixed_->InitializeFixFPS();
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

void MyEngine::DirectXCommon::InitializeDevice()
{
	HRESULT result_;
#ifdef _DEBUG
	// デバックレイヤーをオンに
	ID3D12Debug1* debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
		debugController->EnableDebugLayer();
		debugController->SetEnableGPUBasedValidation(TRUE);
	}

#endif

	// DXGIファクトリーの生成
	result_ = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory_));
	assert(SUCCEEDED(result_));

	// アダプターの列挙用
	std::vector<IDXGIAdapter4*> adapters;
	// ここに特定の名前を持つアダプターオブジェクトが入る
	IDXGIAdapter4* tmpAdapter = nullptr;

	// パフォーマンスが高いものから順に、全てのアダプターを列挙する
	for (UINT i = 0;
		dxgiFactory_->EnumAdapterByGpuPreference(i,
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
		result_ = D3D12CreateDevice(tmpAdapter, levels[i],
			IID_PPV_ARGS(&device_));
		if (result_ == S_OK) {
			// デバイスを生成できた時点でループを抜ける
			featureLevel = levels[i];
			break;
		}
	}

#ifdef _DEBUG
	ComPtr<ID3D12InfoQueue> infoQueue;
	if (SUCCEEDED(device_->QueryInterface(IID_PPV_ARGS(&infoQueue)))) {
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true); // やばいエラー時に止まる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);      // エラー時に止まる
		//infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);    // 警告時に止まる

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

void MyEngine::DirectXCommon::InitializeCommand()
{
	HRESULT result_;

	// コマンドアロケーターを生成
	result_ = device_->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&commandAllocator_));
	assert(SUCCEEDED(result_));

	// コマンドリストを生成
	result_ = device_->CreateCommandList(0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		commandAllocator_.Get(), nullptr,
		IID_PPV_ARGS(&commandList_));
	assert(SUCCEEDED(result_));

	// コマンドキューの設定
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};

	// コマンドキューを生成
	result_ = device_->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&commandQueue_));
	assert(SUCCEEDED(result_));
}

void MyEngine::DirectXCommon::InitializeSwapchain()
{
	HRESULT result_;

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
	result_ = dxgiFactory_->CreateSwapChainForHwnd(
		commandQueue_.Get(), winApp_->GetHwnd(), &swapChainDesc, nullptr, nullptr,
		&swapChain1
	);
	// 生成したIDXGISwapChain1のオブジェクトをIDXGISwapChain4に変換する
	swapChain1.As(&swapChain_);
	assert(SUCCEEDED(result_));
}

void MyEngine::DirectXCommon::InitializeRenderTargetView()
{
	HRESULT result_;

	DXGI_SWAP_CHAIN_DESC swcDesc = {};
    result_ = swapChain_->GetDesc(&swcDesc);
    assert(SUCCEEDED(result_));

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

	rtvHD_ = device_->GetDescriptorHandleIncrementSize(rtvHeapDesc.Type);

	// デスクリプタヒープの生成
	device_->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&rtvHeap_));

	// バックバッファ
	backBuffers_.resize(swapChainDesc.BufferCount);

	// 裏表の2つ分
	for (int i = 0; i < 2; i++) {
		// スワップチェーンの全てのバッファについて処理する
		for (size_t j = 0; j < backBuffers_.size(); j++) {
			// スワップチェーンからバッファを取得
			swapChain_->GetBuffer((UINT)j, IID_PPV_ARGS(&backBuffers_[j]));
			// デクリプタヒープからバッファを取得
			D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap_->GetCPUDescriptorHandleForHeapStart();
			// 裏か表かでアドレスがずれる
			rtvHandle.ptr += j * device_->GetDescriptorHandleIncrementSize(rtvHeapDesc.Type);
			// レンダーターゲットレビューの設定
			D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
			// シェーダーの計算結果をSRGBに変換して書き込む
			rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
			rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
			// レンダーターゲットレビューの生成
			device_->CreateRenderTargetView(backBuffers_[j].Get(), &rtvDesc, rtvHandle);
		}
	}
}

void MyEngine::DirectXCommon::InitializeDepthBuffer()
{
	HRESULT result_;

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
	result_ = device_->CreateCommittedResource(
		&depthHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&depthResourceDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&depthClearValue,
		IID_PPV_ARGS(&depthBuff_));

	// 深度ビュー用デスクリプタヒープ作成
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1;// 深度ビューは1つ
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	result_ = device_->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap_));

	// 深度ビュー作成
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	device_->CreateDepthStencilView(
		depthBuff_.Get(),
		&dsvDesc,
		dsvHeap_->GetCPUDescriptorHandleForHeapStart());
}

void MyEngine::DirectXCommon::InitializeFence()
{
	HRESULT result_;

	result_ = device_->CreateFence(fenceVal_, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_));
}

void MyEngine::DirectXCommon::PreDraw()
{
	// バックバッファの番号取得(２つなので0番か1番)
	UINT bbIndex = swapChain_->GetCurrentBackBufferIndex();

	// 1.リソースバリアで書き込み可能に変更
	D3D12_RESOURCE_BARRIER barrierDesc{};
	barrierDesc.Transition.pResource = backBuffers_[bbIndex].Get();// バックバッファを指定
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;// 表示状態から
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;// 描画状態へ
	commandList_->ResourceBarrier(1, &barrierDesc);
	//commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(backBuffers[bbIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

	// 2.描画先の変更
	// レンダーターゲットビューのハンドルを取得
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle =
		CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeap_->GetCPUDescriptorHandleForHeapStart(), (int)bbIndex, rtvHD_);
	/*rtvHandle.ptr += bbIndex * rtvH;*/
	// 深度ステンシルビュー用デスクリプタヒープのハンドルを取得
	CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle = 
		CD3DX12_CPU_DESCRIPTOR_HANDLE(dsvHeap_->GetCPUDescriptorHandleForHeapStart());
	// 描画先指定
	commandList_->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);

	// 3.描画クリア　　　　　
	// 画面を塗るつぶす色    R    G      B     A
	FLOAT clearcolor[] = { 0.1f, 0.25f, 0.5f, 0.0f };// 青っぽい色
	// レンダーターゲットクリア
	commandList_->ClearRenderTargetView(rtvHandle, clearcolor, 0, nullptr);
	// 震度バッファクリア
	commandList_->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

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
	commandList_->RSSetViewports(1, &viewport);
	//commandList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, WinApp::window_width, WinApp::window_height));

	// シザー矩形
	D3D12_RECT scissorRect{};
	scissorRect.left = 0;// 切り抜き座標左
	scissorRect.right = scissorRect.left + WinApp::window_width;// 切り抜き座標右
	scissorRect.top = 0;// 切り抜き座標上
	scissorRect.bottom = scissorRect.top + WinApp::window_height;// 切り抜き座標下
	// シザー矩形設定コマンドを、コマンドリストに積む
	commandList_->RSSetScissorRects(1, &scissorRect);
	//commandList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, WinApp::window_width, WinApp::window_height));
}

void MyEngine::DirectXCommon::PostDraw()
{
	HRESULT result_;

	// バックバッファの番号取得(２つなので0番か1番)
	UINT bbIndex = swapChain_->GetCurrentBackBufferIndex();

	// 5.リソースバリアを戻す
	D3D12_RESOURCE_BARRIER barrierDesc{};
	barrierDesc.Transition.pResource = backBuffers_[bbIndex].Get();           // バックバッファを指定
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET; // 表示状態から
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;        // 描画状態へ
	commandList_->ResourceBarrier(1, &barrierDesc);

	// 命令のクローズ
	result_ = commandList_->Close();
	assert(SUCCEEDED(result_));

	// コマンドリストの実行
	ID3D12CommandList* commandLists[] = { commandList_.Get()};
	commandQueue_->ExecuteCommandLists(1, commandLists);

	// 画面に表示するバッファをクリップ(裏表の入れ替え)
	result_ = swapChain_->Present(1, 0);
	assert(SUCCEEDED(result_));

	// コマンドの実行完了を持つ
	commandQueue_->Signal(fence_.Get(), ++fenceVal_);
	if (fence_->GetCompletedValue() != fenceVal_) {
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		fence_->SetEventOnCompletion(fenceVal_, event);
		if (event != NULL) {
			WaitForSingleObject(event, INFINITE);
			CloseHandle(event);
		}
	}

	// FPS固定
	fpsFixed_->UpdateFixFPS();

	// キューをクリア
	result_ = commandAllocator_->Reset();
	assert(SUCCEEDED(result_));

	// 再びコマンドリストを貯める準備
	result_ = commandList_->Reset(commandAllocator_.Get(), nullptr);
	assert(SUCCEEDED(result_));
}

void MyEngine::DirectXCommon::fpsFixedFinalize(){
	safe_delete(fpsFixed_);
}

void MyEngine::DirectXCommon::ResourceLeakCheck()
{
#ifdef _DEBUG
	// リソースリークチェック
	IDXGIDebug1* debug;
	if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&debug)))) {
		debug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
		debug->ReportLiveObjects(DXGI_DEBUG_APP, DXGI_DEBUG_RLO_ALL);
		debug->ReportLiveObjects(DXGI_DEBUG_D3D12, DXGI_DEBUG_RLO_ALL);
		debug->Release();
	}
#endif
}