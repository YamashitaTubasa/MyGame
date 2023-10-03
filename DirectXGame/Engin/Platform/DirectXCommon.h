#pragma once 

#include "WinApp.h"
#include "FPSFixed.h"

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <d3dx12.h>
#include <chrono>
#include <thread>

#pragma warning(push)
// C4023の警告を見なかったことにする
#pragma warning(disable:4023)
#include <DirectXTex.h>
#pragma warning(pop)

/// <summary>
/// DirectX基盤
/// </summary>
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
	// DirectXCommonのインスタンス
	static DirectXCommon* instance;
	// インスタンスの取得
	static DirectXCommon* GetInstance();
	// インスタンスの解放
	void DeleteInstance();
	// リソースリークチェック
	static void ResourceLeakCheck();

private:
	// コンストラクタ
	DirectXCommon() = default;
	// デストラクタ
	~DirectXCommon() = default;
	// コピーコンストラクタの禁止
	DirectXCommon(const DirectXCommon&) = delete;
	// 代入演算子の禁止
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
	Microsoft::WRL::ComPtr<ID3D12Resource> depthBuff;
	//バックバッファ
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> backBuffers;
	// フェンスの生成
	Microsoft::WRL::ComPtr<ID3D12Fence> fence;

	UINT rtvHD;
	UINT64 fenceVal = 0;
};