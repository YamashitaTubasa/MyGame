/**
 * @file DirectXCommon.h
 * @brief DirectXの基盤クラス
 * @author Yamashita Tubasa
 */

#pragma once 

#include "WinApp.h"
#include "FPSFixed.h"

#pragma warning(push)
#pragma warning(disable:4023)
#pragma warning(disable:4061)
#pragma warning(disable:4820)
#pragma warning(disable:4265)
#pragma warning(disable:4365)
#pragma warning(disable:4514)
#pragma warning(disable:4625)
#pragma warning(disable:4626)
#pragma warning(disable:4668)
#pragma warning(disable:5204)
#pragma warning(disable:5220)
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <d3dx12.h>
#include <chrono>
#include <thread>
#include <DirectXTex.h>
#pragma warning(pop)

namespace MyEngine 
{
	class DirectXCommon final
	{
	private: // エイリアス
		template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	public: // メンバ関数
		/// <summary>
		/// 初期化
		/// </summary>
		/// <param name="winApp"></param>
		void Initialize(WinApp* winApp);

		/// <summary>
		/// デバイスの初期化
		/// </summary>
		void InitializeDevice();

		/// <summary>
		/// コマンド関連の初期化
		/// </summary>
		void InitializeCommand();

		/// <summary>
		/// スワップチェーンの初期化
		/// </summary>
		void InitializeSwapchain();

		/// <summary>
		/// レンダーターゲットビューの初期化
		/// </summary>
		void InitializeRenderTargetView();

		/// <summary>
		/// 深度バッファの初期化
		/// </summary>
		void InitializeDepthBuffer();

		/// <summary>
		/// フェンスの初期化
		/// </summary>
		void InitializeFence();

		/// <summary>
		/// 描画前処理
		/// </summary>
		void PreDraw();

		/// <summary>
		/// 描画後処理
		/// </summary>
		void PostDraw();

		/// <summary>
		/// 終了処理
		/// </summary>
		void fpsFixedFinalize();

	public:
		// DirectXCommonのインスタンス
		static DirectXCommon* instance;

		/// <summary>
		/// インスタンスの取得
		/// </summary>
		/// <returns></returns>
		static DirectXCommon* GetInstance();

		/// <summary>
		/// インスタンスの解放
		/// </summary>
		void DeleteInstance();

		/// <summary>
		/// リソースリークチェック
		/// </summary>
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
		/// <summary>
		/// デバイスの取得
		/// </summary>
		/// <returns></returns>
		[[maybe_unused]] ID3D12Device* GetDevice() const { return device_.Get(); }

		/// <summary>
		/// コマンドリスト取得
		/// </summary>
		/// <returns></returns>
		[[maybe_unused]] ID3D12GraphicsCommandList* GetCommandList() const { return commandList_.Get(); }

		/// <summary>
		/// バックバッファの数を取得
		/// </summary>
		/// <returns></returns>
		[[maybe_unused]] size_t GetBackBufferCount() const { return backBuffers_.size(); }

	private:
		// WindowsAPI
		WinApp* winApp_ = nullptr;
		// FPS
		FPSFixed* fpsFixed_ = nullptr;

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
};