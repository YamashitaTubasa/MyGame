#pragma once

#include "Sprite.h"
#include "WinApp.h"

class PostEffect : public Sprite
{
private:
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private:
	struct ConstBufferData {
		DirectX::XMFLOAT4 color; // 色 (RGBA)
		DirectX::XMMATRIX mat; // 座標
		bool isPostE; // ブラーフラグ
		float alpha; // アルファ値
	};

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	PostEffect();
	~PostEffect();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const wchar_t* filename);

	/// <summary>
	/// 描画コマンド
	/// </summary>
	/// <param name="cmdList">コマンドリスト</param>
	void Draw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// シーン描画前処理
	/// </summary>
	/// <param name="cmdList">コマンドリスト</param>
	void PreDraw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// シーン描画後処理
	/// </summary>
	/// <param name="cmdList">コマンドリスト</param>
	void PostDraw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// パイプライン生成
	/// </summary>
	void CreateGraphicsPipelineState(const wchar_t* filename);

public:
	void SetColor(const DirectX::XMFLOAT4& color);
	void SetIsPostE(const bool& isPostE);
	void SetAlpha(const float& alpha);

	DirectX::XMFLOAT4 GetColor() const { return color_; }

private:
	// テクスチャバッファ
	ComPtr<ID3D12Resource> texBuff;
	// SRV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;
	// 震度バッファ
	ComPtr<ID3D12Resource> depthBuff;
	// RTV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapRTV;
	// DSV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapDSV;
	// グラフィクスパイプライン
	ComPtr<ID3D12PipelineState> pipelineState;
	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> rootSignature;
	ComPtr<ID3D12Device> device;
	ComPtr<ID3D12GraphicsCommandList> cmdList;
	ConstBufferData* constMap = nullptr;

protected:
	// 画面クリアカラー
	static const float clearColor[4];
	// 頂点数
	static const int vertNum = 4;
	// 色
	DirectX::XMFLOAT4 color_ = { 1,1,1,1 };
	// ブラーフラグ
	bool isPostE_ = false;
	// アルファ値
	float alpha_ = 1.0f;
};