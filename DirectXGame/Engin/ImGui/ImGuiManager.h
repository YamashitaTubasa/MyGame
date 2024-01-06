/**
 * @file ImGuiManager.h
 * @brief ImGuiの管理クラス
 * @author Yamashita Tubasa
 */

#pragma once

#include "WinApp.h"
#include "DirectXCommon.h"

#pragma warning(push)
#pragma warning(disable:4023)
#include <imgui_impl_win32.h>
#include <imgui_impl_dx12.h>
#pragma warning(pop)

namespace MyEngine
{
	class ImGuiManager
	{
	public:
		// コンストラクタ
		ImGuiManager();
		// デストラクタ
		~ImGuiManager();

	public:
		/// <summary>
		/// 初期化
		/// </summary>
		/// <param name="dXCommon"></param>
		void Initialize(DirectXCommon* dXCommon, WinApp* winApp);

		/// <summary>
		/// 終了
		/// </summary>
		void Finalize();

		/// <summary>
		/// ImGui受付開始
		/// </summary>
		void Begin();

		/// <summary>
		/// ImGui受付終了
		/// </summary>
		void End();

		/// <summary>
		/// 画面への描画
		/// </summary>
		void Draw(DirectXCommon* dXCommon);

		/// <summary>
		/// セーブ
		/// </summary>
		void MySaveFunction();

	private:
		WinApp* winApp_ = nullptr;
		DirectXCommon* dXCommon_ = nullptr;

		// SRV用デスクリプタヒープ
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap_;
	};
};