/**
 * @file PostEffect.h
 * @brief ポストエフェクトに関するクラス
 * @author Yamashita Tubasa
 */

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
	/// <summary>
	/// 色のセット
	/// </summary>
	/// <param name="color">色</param>
	void SetColor(const DirectX::XMFLOAT4& color);

	/// <summary>
	/// ポストエフェクトのフラグのセット
	/// </summary>
	/// <param name="isPostE">ポストエフェクトのフラグ</param>
	void SetIsPostE(const bool& isPostE);

	/// <summary>
	/// アルファ値のセット
	/// </summary>
	/// <param name="alpha">アルファ値</param>
	void SetAlpha(const float& alpha);

	/// <summary>
	/// 色の取得
	/// </summary>
	/// <returns>色</returns>
	DirectX::XMFLOAT4 GetColor() const { return color_; }

private:
	// テクスチャバッファ
	ComPtr<ID3D12Resource> texBuff_;
	// SRV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapSRV_;
	// 震度バッファ
	ComPtr<ID3D12Resource> depthBuff_;
	// RTV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapRTV_;
	// DSV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapDSV_;
	// グラフィクスパイプライン
	ComPtr<ID3D12PipelineState> pipelineState_;
	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> rootSignature_;
	ComPtr<ID3D12Device> device_;
	ComPtr<ID3D12GraphicsCommandList> cmdList_;
	ConstBufferData* constMap_ = nullptr;

protected:
	// 画面クリアカラー
	static const float clearColor_[4];
	// 頂点数
	static const int vertNum_ = 4;
	// 色
	DirectX::XMFLOAT4 color_ = { 1,1,1,1 };
	// ブラーフラグ
	bool isPostE_ = false;
	// アルファ値
	float alpha_ = 1.0f;
};