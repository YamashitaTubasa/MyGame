#pragma once 
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <d3dx12.h>
#include <chrono>
#include <thread>

#include <DirectXTex.h>

#include "WinApp.h"
#include "FPSFixed.h"

// DirectX基盤
class DirectXCommon final
{
public: // メンバ関数
	// 初期化
	void Initialize(WinApp* winApp);
	// デバイスの初期化
	void InitializeDevice();
	// コマンド関連の初期化
	void InitializeCommand();
	// スワップチェーンの初期化
	void InitializeSwapchain();
	// レンダーターゲットビューの初期化
	void InitializeRenderTargetView();
	// 深度バッファの初期化
	void InitializeDepthBuffer();
	// フェンスの初期化
	void InitializeFence();
	// 描画前処理
	void PreDraw();
	// 描画後処理
	void PostDraw();
	// 終了処理
	void fpsFixedFinalize();

public:
	static DirectXCommon* GetInstance();

private:
	DirectXCommon() = default;
	~DirectXCommon() = default;
	DirectXCommon(const DirectXCommon&) = delete;
	DirectXCommon& operator=(const DirectXCommon&) = delete;

private: // メンバ関数
	template <class T>
	inline void safe_delete(T*& p) {
		delete p;
		p = nullptr;
	}
	
public: // Getter
	// デバイスの取得
	ID3D12Device* GetDevice() const { return device.Get(); }
	// コマンドリスト取得
	ID3D12GraphicsCommandList* GetCommandList() const { return commandList.Get(); }
	// バックバッファの数を取得
	size_t GetBackBufferCount() const { return backBuffers.size(); }

private: 
	// WindowsAPI
	WinApp* winApp = nullptr;
	// FPS
	FPSFixed* fpsFixed = nullptr;

	HRESULT result;
	// DirectX12デバイス
	Microsoft::WRL::ComPtr<ID3D12Device> device;
	// DXGIファクトリ
	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory;
	// スワップチェイン
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain;
	// コマンドアロケータ
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator;
	// コマンドリスト
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList;
	// コマンドキュー
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvHeap;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvHeap;

	//// スワップチェーンの設定
	//DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	//// コマンドキューの設定
	//D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
	//D3D12_RESOURCE_BARRIER barrierDesc{};
	//D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
	//バックバッファ
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> backBuffers;
	// フェンスの生成
	Microsoft::WRL::ComPtr<ID3D12Fence> fence;

	UINT rtvHD;
	UINT64 fenceVal = 0;
};