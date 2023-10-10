#pragma once 

#include "WinApp.h"
#include "FPSFixed.h"

#include <dxgi1_6.h>
#include <wrl.h>
#include <d3dx12.h>
#include <chrono>
#include <thread>

#pragma warning(push)
#pragma warning(disable:4820)
#include <d3d12.h>
#pragma warning(pop)

#pragma warning(push)
#pragma warning(disable:4023)
#include <DirectXTex.h>
#pragma warning(pop)

//=============//
// DirectX基盤 //
//=============//
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
	ID3D12Device* GetDevice() const { return device_.Get(); }
	// コマンドリスト取得
	ID3D12GraphicsCommandList* GetCommandList() const { return commandList_.Get(); }
	// バックバッファの数を取得
	size_t GetBackBufferCount() const { return backBuffers_.size(); }

private: 
	// WindowsAPI
	WinApp* winApp_ = nullptr;
	// FPS
	FPSFixed* fpsFixed_ = nullptr;

	HRESULT result_;
	// DirectX12デバイス
	Microsoft::WRL::ComPtr<ID3D12Device> device_;
	// DXGIファクトリ
	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory_;
	// スワップチェイン
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain_;
	// コマンドアロケータ
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator_;
	// コマンドリスト
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList_;
	// コマンドキュー
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue_;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvHeap_;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvHeap_;
	Microsoft::WRL::ComPtr<ID3D12Resource> depthBuff_;
	//バックバッファ
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> backBuffers_;
	// フェンスの生成
	Microsoft::WRL::ComPtr<ID3D12Fence> fence_;

	UINT rtvHD_;
	UINT64 fenceVal_ = 0;
};