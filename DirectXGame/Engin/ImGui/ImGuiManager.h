#pragma once

#include "WinApp.h"
#include "DirectXCommon.h"

#pragma warning(push)
// C4023の警告を見なかったことにする
#pragma warning(disable:4023)
#include <imgui_impl_win32.h>
#include <imgui_impl_dx12.h>
#pragma warning(pop)

//==============
// Imguiの管理
//==============
class ImGuiManager 
{
public:
	ImGuiManager();
	~ImGuiManager();
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="dXCommon"></param>
	void Initialize(DirectXCommon* dXCommon_, WinApp* winApp_);

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
