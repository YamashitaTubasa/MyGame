/**
 * @file ImGuiManager.cpp
 * @brief ImGuiの管理クラス
 * @author Yamashita Tubasa
 */

#include "ImGuiManager.h"

MyEngine::ImGuiManager::ImGuiManager()
{
}

MyEngine::ImGuiManager::~ImGuiManager()
{
}

void MyEngine::ImGuiManager::Initialize(DirectXCommon* dXCommon, WinApp* winApp) {

	HRESULT result;

	// ImGuiのコンテキストを生成
	ImGui::CreateContext();
	// ImGuiのスタイルを設定
	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(winApp->GetHwnd());

	// デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	desc.NumDescriptors = 1;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	// デスクリプタヒープ生成
	result = dXCommon->GetDevice()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&srvHeap_));
	assert(SUCCEEDED(result));
	srvHeap_->SetName(L"ImGuiManager[srvHeap_]");
	ImGui_ImplDX12_Init(
		dXCommon->GetDevice(),
		static_cast<int>(dXCommon->GetBackBufferCount()),
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, srvHeap_.Get(),
		srvHeap_->GetCPUDescriptorHandleForHeapStart(),
		srvHeap_->GetGPUDescriptorHandleForHeapStart()
	);

	ImGuiIO& io = ImGui::GetIO();
	// 標準フォントを追加する
	io.Fonts->AddFontDefault();
}

void MyEngine::ImGuiManager::Finalize()
{
	// 後始末
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	// デスクリプタヒープの解放
	srvHeap_.Reset();
}

void MyEngine::ImGuiManager::Begin()
{
	// ImGuiフレーム開始
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void MyEngine::ImGuiManager::End()
{
	// 描画準備
	ImGui::Render();
}

void MyEngine::ImGuiManager::Draw(DirectXCommon* dXCommon)
{
	ID3D12GraphicsCommandList* commandList = dXCommon->GetCommandList();

	// デスクリプタヒープの配列をセットするコマンド
	ID3D12DescriptorHeap* ppHeaps[] = { srvHeap_.Get() };
	commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	// 描画コマンドを発行
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList);
}

void MyEngine::ImGuiManager::MySaveFunction()
{
}
