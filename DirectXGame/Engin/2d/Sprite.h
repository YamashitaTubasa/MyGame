#pragma once

#include <Windows.h>
#include <D3dx12.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <cassert>
#include <DirectXTex.h>
#include <wrl.h>
#include <array>

#include "DirectXCommon.h"

#pragma comment(lib, "d3dcompiler.lib")

// 頂点データ
struct VertexPosUv {
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 uv;
};

// パイプラインセット
struct PipelineSet {
	// パイプラインステートオブジェクト
	ComPtr<ID3D12PipelineState> pipelinestate;
	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> rootsignature;
};

// スプライトの共通データ
struct SpriteCommon {
	// パイプラインセット
	PipelineSet pipelineSet;
	// 射影行列
	DirectX::XMMATRIX matProjection{};
	// テクスチャ用デスクリプタヒープの生成
	ComPtr<ID3D12DescriptorHeap> descHeap;
	// SRVの最大枚数
	static const size_t kMaxSRVCount = 512;
	// テクスチャソース(テクスチャバッファ)の配列
	ComPtr<ID3D12Resource> texBuff[kMaxSRVCount];
	// テクスチャバッファ取得
	ID3D12Resource* GetTexBuffer(uint32_t index) const { return texBuff[index].Get(); }
};

class Sprite
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using TexMetadata = DirectX::TexMetadata;
	using ScratchImage = DirectX::ScratchImage;
	// std::を省略
	using string = std::string;
	template <class T> using vector = std::vector<T>;

public:
	Sprite(UINT texNumber, XMFLOAT2 pos, XMFLOAT2 size, XMFLOAT4 color, XMFLOAT2 anchorpoint, bool isFlipX, bool isFlipY);
	~Sprite();

private:
	struct ConstBufferData {
		XMFLOAT4 color; // 色 (RGBA)
		XMMATRIX mat; //座標
	};

public:
	/// <summary>
	/// スプライト共通データ生成
	/// </summary>
	SpriteCommon SpriteCommonCreate(ID3D12Device* dev, int window_width, int window_height);

	/// <summary>
	/// 3Dオブジェクト用パイプライン生成
	/// </summary>
	/// <param name="device"></param>
	/// <returns></returns>
	PipelineSet SpriteCreateGraphicsPipeline(ID3D12Device* device);

	/// <summary>
	/// スプライト共通テクスチャ読み込み
	/// </summary>
	void LoadTexture(SpriteCommon& spriteCommon, UINT texnumber, 
		const wchar_t* filename, ID3D12Device* dev);

	/// <summary>
	/// スプライト生成
	/// </summary>
	void SpriteCreate(ID3D12Device* dev, float window_width, float window_height, 
		UINT texNumber, const SpriteCommon& spriteCommon, XMFLOAT2 anchorpoint, bool isFlipX, bool FlipY);

	/// <summary>
	/// スプライト単体頂点バッファの転送
	/// </summary>
	void SpriteTransferVertexBuffer(const Sprite* sprite, const SpriteCommon& spriteCommon, uint32_t texIndex_ = UINT32_MAX);

	/// <summary>
	/// スプライト単体更新
	/// </summary>
	void SpriteUpdate(Sprite* sprite, const SpriteCommon& spriteCommon);

	/// <summary>
	/// 描画前処理
	/// </summary>
	static void PreDraw(ID3D12GraphicsCommandList* cmdList, const SpriteCommon& spriteCommon);

	/// <summary>
	/// 描画後処理
	/// </summary>
	static void PostDraw();

	/// <summary>
	/// スプライト単体描画
	/// </summary>
	void SpriteDraw(ID3D12GraphicsCommandList* cmdList_, const SpriteCommon& spriteCommon, 
		ID3D12Device* dev,D3D12_VERTEX_BUFFER_VIEW& vbView);

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

public: // セッター
	// 座標設定
	void SetPosition(const XMFLOAT3& position) { this->position = position; }
	// サイズ設定
	void SetScale(const XMFLOAT2& scale_) { this->scale = scale_; }
	// 回転
	void SetRotation(float rotation_) { this->rotation = rotation_; }
	// 番号
	void SetTexNumber(UINT texNumber_) { this->texNumber = texNumber_; }
	// 色
	void SetColor(const XMFLOAT4& color) { this->color_ = color; }
	// フリップ
	void SetIsFlipX(bool isFlipX) { this->isFlipX_ = isFlipX; } // X
	void SetIsFlipY(bool isFlipY) { this->isFlipY_ = isFlipY; } // Y
	// テクスチャ左上座標
	void SetTexLeftTop(const XMFLOAT2& texLeftTop) { this->texLeftTop_ = texLeftTop; }
	// テクスチャ切り出しサイズ
	void SetTexSize(const XMFLOAT2& texSize) { this->texSize_ = texSize; }

public: // ゲッター
	// 座標
	XMFLOAT3 GetPosition() const { return position; }
	// サイズ
	XMFLOAT2 GetScale() const { return scale; }
	// 回転
	float GetRotation() const { return rotation; }
	// 番号
	UINT GetTexNumber() const { return texNumber; }
	// 色
	XMFLOAT4 GetColor() const { return color_; }
	// フリップ
	bool GetIsFlipX() const { return isFlipX_; } // X
	bool GetIsFlipY() const { return isFlipY_; } // Y
	// テクスチャ左上座標
	XMFLOAT2 GetTexLeftTop() const { return texLeftTop_; }
	// テクスチャ切り出しサイズ
	XMFLOAT2 GetTexSize() const { return texSize_; }

public:
	//頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;
	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	D3D12_RESOURCE_DESC resDesc;

protected:
	// DirectXCommonのインスタンス
	DirectXCommon* dXCommon = nullptr;
	SpriteCommon spriteCommon_;

	ComPtr<ID3D12GraphicsCommandList> cmdList;
	ComPtr<ID3D12DescriptorHeap> descHeap;
	ComPtr<ID3D12Resource> constBuff;
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	static const size_t kMaxSRVCount = 2056;
	// テクスチャバッファ
	std::array<ComPtr<ID3D12Resource>, kMaxSRVCount> texBuffers_;

	// テクスチャ番号
	UINT texNumber = 0;
	uint32_t texIndex_ = 0;
	// 座標
	XMFLOAT3 position = { 0.0f, 0.0f, 0.0f };
	// サイズ
	XMFLOAT2 scale = { 1.0f, 1.0f };
	// 回転
	XMFLOAT3 rotation_ = { 0.0f,0.0f,0.0f };
	// ワールド行列
	XMMATRIX matWorld;
	// アンカーポイント
	XMFLOAT2 anchorpoint = { 0.0f,0.0f };
	// テクスチャ左上座標設定
	XMFLOAT2 texLeftTop_ = { 50.0f, 50.0f };
	// テクスチャ切り出しサイズ
	XMFLOAT2 texSize_ = { 100.0f, 100.0f };
	// 色
	XMFLOAT4 color_ = { 1,1,1,1 };

	// Z軸回りの回転
	float rotation = 0.0f;
	// 左右反転
	bool isFlipX_ = false;
	// 上下反転
	bool isFlipY_ = false;
	// 非表示
	bool isInvisible = false;
};

