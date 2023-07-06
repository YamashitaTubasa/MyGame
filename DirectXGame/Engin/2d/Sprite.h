#pragma once

#include "DirectXCommon.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"

#include <Windows.h>
#include <D3dx12.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <cassert>
#include <DirectXTex.h>
#include <wrl.h>
#include <array>

#pragma comment(lib, "d3dcompiler.lib")

// 頂点データ
struct VertexPosUv {
	Vector3 pos;
	Vector2 uv;
};

// パイプラインセット
struct PipelineSet {
	// パイプラインステートオブジェクト
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelinestate;
	// ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootsignature;
};

// スプライトの共通データ
struct SpriteCommon {
	// パイプラインセット
	PipelineSet pipelineSet;
	// 射影行列
	DirectX::XMMATRIX matProjection{};
	// テクスチャ用デスクリプタヒープの生成
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeap;
	// SRVの最大枚数
	static const size_t kMaxSRVCount = 512;
	// テクスチャソース(テクスチャバッファ)の配列
	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff[kMaxSRVCount];
	// テクスチャバッファ取得
	ID3D12Resource* GetTexBuffer(uint32_t index) const { return texBuff[index].Get(); }
};

class Sprite
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using TexMetadata = DirectX::TexMetadata;
	using ScratchImage = DirectX::ScratchImage;
	// std::を省略
	using string = std::string;
	template <class T> using vector = std::vector<T>;

public:
	Sprite() = default;
	Sprite(UINT texNumber, Vector3 pos, Vector2 size, Vector4 color, Vector2 anchorpoint, bool isFlipX, bool isFlipY);
	~Sprite();

private:
	struct ConstBufferData {
		Vector4 color; // 色 (RGBA)
		Matrix4 mat; //座標
	};

public:
	/// <summary>
	/// スプライト共通データ生成
	/// </summary>
	SpriteCommon SpriteCommonCreate(int window_width, int window_height);

	/// <summary>
	/// 3Dオブジェクト用パイプライン生成
	/// </summary>
	/// <param name="device"></param>
	/// <returns></returns>
	PipelineSet SpriteCreateGraphicsPipeline();

	/// <summary>
	/// スプライト共通テクスチャ読み込み
	/// </summary>
	void LoadTexture(SpriteCommon& spriteCommon, UINT texnumber, const wchar_t* filename);

	/// <summary>
	/// スプライト生成
	/// </summary>
	void SpriteCreate(float window_width, float window_height, UINT texNumber, 
		const SpriteCommon& spriteCommon, Vector2 anchorpoint, bool isFlipX, bool FlipY);

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
	void SpriteDraw(ID3D12GraphicsCommandList* cmdList_, const SpriteCommon& spriteCommon);

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

public: // セッター
	// 座標設定
	void SetPosition(const Vector3& position) { this->position = position; }
	// サイズ設定
	void SetScale(const Vector2& scale_) { this->scale = scale_; }
	// 回転
	void SetRotation(float rotation_) { this->rotation = rotation_; }
	// 番号
	void SetTexNumber(UINT texNumber_) { this->texNumber = texNumber_; }
	// 色
	void SetColor(const Vector4& color) { this->color_ = color; }
	// フリップ
	void SetIsFlipX(bool isFlipX) { this->isFlipX_ = isFlipX; } // X
	void SetIsFlipY(bool isFlipY) { this->isFlipY_ = isFlipY; } // Y
	// テクスチャ左上座標
	void SetTexLeftTop(const Vector2& texLeftTop) { this->texLeftTop_ = texLeftTop; }
	// テクスチャ切り出しサイズ
	void SetTexSize(const Vector2& texSize) { this->texSize_ = texSize; }

public: // ゲッター
	// 座標
	Vector3 GetPosition() const { return position; }
	// サイズ
	Vector2 GetScale() const { return scale; }
	// 回転
	float GetRotation() const { return rotation; }
	// 番号
	UINT GetTexNumber() const { return texNumber; }
	// 色
	Vector4 GetColor() const { return color_; }
	// フリップ
	bool GetIsFlipX() const { return isFlipX_; } // X
	bool GetIsFlipY() const { return isFlipY_; } // Y
	// テクスチャ左上座標
	Vector2 GetTexLeftTop() const { return texLeftTop_; }
	// テクスチャ切り出しサイズ
	Vector2 GetTexSize() const { return texSize_; }

public:

protected:
	// DirectXCommonのインスタンス
	DirectXCommon* dXCommon = nullptr;
	SpriteCommon spriteCommon_;

	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	D3D12_RESOURCE_DESC resDesc;
	ComPtr<ID3D12Device> device;
	//頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;
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
	Vector3 position = { 0.0f, 0.0f, 0.0f };
	// サイズ
	Vector2 scale = { 1.0f, 1.0f };
	// 回転
	Vector3 rotation_ = { 0.0f,0.0f,0.0f };
	// ワールド行列
	Matrix4 matWorld;
	// アンカーポイント
	Vector2 anchorpoint = { 0.0f,0.0f };
	// テクスチャ左上座標設定
	Vector2 texLeftTop_ = { 50.0f, 50.0f };
	// テクスチャ切り出しサイズ
	Vector2 texSize_ = { 100.0f, 100.0f };
	// 色
	Vector4 color_ = { 1,1,1,1 };

	// Z軸回りの回転
	float rotation = 0.0f;
	// 左右反転
	bool isFlipX_ = false;
	// 上下反転
	bool isFlipY_ = false;
	// 非表示
	bool isInvisible = false;
};

