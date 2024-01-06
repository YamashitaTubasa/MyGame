/**
 * @file Sprite.h
 * @brief スプライトに関するクラス
 * @author Yamashita Tubasa
 */

#pragma once

#include "DirectXCommon.h"

#pragma warning(push)
#pragma warning(disable:4668)
#include <D3dx12.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <cassert>
#include <DirectXTex.h>
#include <wrl.h>
#include <array>
#include <Windows.h>
#pragma warning(pop)

#pragma comment(lib, "d3dcompiler.lib")

namespace MyEngine {

	// 頂点データ
	struct VertexPosUv {
		DirectX::XMFLOAT3 pos_;
		DirectX::XMFLOAT2 uv_;
	};

	// パイプラインセット
	struct PipelineSet {
		// パイプラインステートオブジェクト
		Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelinestate_;
		// ルートシグネチャ
		Microsoft::WRL::ComPtr<ID3D12RootSignature> rootsignature_;
	};

	// スプライトの共通データ
	struct SpriteCommon {

		// パイプラインセット
		PipelineSet pipelineSet_;
		// 射影行列
		DirectX::XMMATRIX matProjection_{};
		// テクスチャ用デスクリプタヒープの生成
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeap_;
		// SRVの最大枚数
		static const size_t kMaxSRVCount_ = 512;
		// テクスチャソース(テクスチャバッファ)の配列
		Microsoft::WRL::ComPtr<ID3D12Resource> texBuff_[kMaxSRVCount_];
		// テクスチャバッファ取得
		ID3D12Resource* GetTexBuffer(uint32_t index) const { return texBuff_[index].Get(); }
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
		Sprite() = default;
		Sprite([[maybe_unused]] UINT texNumber, [[maybe_unused]] XMFLOAT3 pos, [[maybe_unused]] XMFLOAT2 size, [[maybe_unused]] XMFLOAT4 color, [[maybe_unused]] XMFLOAT2 anchorpoint, [[maybe_unused]] bool isFlipX, [[maybe_unused]] bool isFlipY);
		~Sprite();

	private:
		struct ConstBufferData {
			XMFLOAT4 color_; // 色 (RGBA)
			float alpha_; // アルファ値
			XMMATRIX mat_; //座標
		};

	public:
		/// <summary>
		/// スプライト共通データ生成
		/// </summary>
		/// <param name="window_width">横幅</param>
		/// <param name="window_height">縦幅</param>
		/// <returns></returns>
		SpriteCommon SpriteCommonCreate();

		/// <summary>
		/// 3Dオブジェクト用パイプライン生成
		/// </summary>
		/// <param name="device"></param>
		/// <returns></returns>
		PipelineSet SpriteCreateGraphicsPipeline();

		/// <summary>
		/// スプライト共通テクスチャ読み込み
		/// </summary>
		/// <param name="spriteCommon"></param>
		/// <param name="texnumber">テクスチャ番号</param>
		/// <param name="filename">ファイル名</param>
		void LoadTexture(SpriteCommon& spriteCommon, UINT texnumber, const wchar_t* filename);

		/// <summary>
		/// スプライト生成
		/// </summary>
		/// <param name="window_width">横幅</param>
		/// <param name="window_height">縦幅</param>
		/// <param name="texNumber">テクスチャ番号</param>
		/// <param name="spriteCommon"></param>
		/// <param name="anchorpoint">アンカーポイント</param>
		/// <param name="isFlipX">フリップX</param>
		/// <param name="FlipY">フリップY</param>
		void SpriteCreate(float window_width, float window_height, UINT texNumber,
			const SpriteCommon& spriteCommon, XMFLOAT2 anchorpoint, bool isFlipX, bool FlipY);

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
		void SpriteDraw(const SpriteCommon& spriteCommon);

		/// <summary>
		/// 終了処理
		/// </summary>
		void Finalize();

	public: // セッター
		/// <summary>
		/// 座標の設定
		/// </summary>
		/// <param name="position">座標</param>
		void SetPosition(const XMFLOAT3& position) { this->position_ = position; }

		/// <summary>
		/// サイズの設定
		/// </summary>
		/// <param name="scale">サイズ</param>
		void SetScale(const XMFLOAT2& scale) { this->scale_ = scale; }

		/// <summary>
		/// 回転の設定
		/// </summary>
		/// <param name="rotation">回転</param>
		void SetRotation(float rotation) { this->f_rotation_ = rotation; }

		/// <summary>
		/// 番号のセット
		/// </summary>
		/// <param name="texNumber">番号</param>
		void SetTexNumber(UINT texNumber) { this->texNumber_ = texNumber; }

		/// <summary>
		/// 色の設定
		/// </summary>
		/// <param name="color">色</param>
		void SetColor(const XMFLOAT4& color) { this->color_ = color; }

		/// <summary>
		/// フリップXの設定
		/// </summary>
		/// <param name="isFlipX">フリップXフラグ</param>
		void SetIsFlipX(bool isFlipX) { this->isFlipX_ = isFlipX; }

		/// <summary>
		/// フリップYの設定
		/// </summary>
		/// <param name="isFlipX">フリップYフラグ</param>
		void SetIsFlipY(bool isFlipY) { this->isFlipY_ = isFlipY; }

		/// <summary>
		/// テクスチャ左上座標
		/// </summary>
		/// <param name="texLeftTop">テクスチャ左上座標</param>
		void SetTexLeftTop(const XMFLOAT2& texLeftTop) { this->texLeftTop_ = texLeftTop; }

		/// <summary>
		/// テクスチャ切り出しサイズ
		/// </summary>
		/// <param name="texSize">テクスチャ切り出しサイズ</param>
		void SetTexSize(const XMFLOAT2& texSize) { this->texSize_ = texSize; }

		/// <summary>
		/// アルファ値
		/// </summary>
		/// <param name="alpha"></param>
		void SetAlpha(float alpha);

	public: // ゲッター
		/// <summary>
		/// 座標
		/// </summary>
		/// <returns>座標</returns>
		XMFLOAT3 GetPosition() const { return position_; }

		/// <summary>
		/// サイズ
		/// </summary>
		/// <returns>サイズ</returns>
		XMFLOAT2 GetScale() const { return scale_; }

		/// <summary>
		/// 回転
		/// </summary>
		/// <returns>回転</returns>
		float GetRotation() const { return f_rotation_; }

		/// <summary>
		/// 番号
		/// </summary>
		/// <returns>番号</returns>
		UINT GetTexNumber() const { return texNumber_; }

		/// <summary>
		/// 色
		/// </summary>
		/// <returns>色</returns>
		XMFLOAT4 GetColor() const { return color_; }

		/// <summary>
		/// フリップX
		/// </summary>
		/// <returns>フリップX</returns>
		bool GetIsFlipX() const { return isFlipX_; }

		/// <summary>
		/// フリップY
		/// </summary>
		/// <returns>フリップY</returns>
		bool GetIsFlipY() const { return isFlipY_; }

		/// <summary>
		/// テクスチャ左上座標
		/// </summary>
		/// <returns>テクスチャ左上座標</returns>
		XMFLOAT2 GetTexLeftTop() const { return texLeftTop_; }

		/// <summary>
		/// テクスチャ切り出しサイズ
		/// </summary>
		/// <returns>テクスチャ切り出しサイズ</returns>
		XMFLOAT2 GetTexSize() const { return texSize_; }

	private:
		static ID3D12GraphicsCommandList* cmdList_;

	protected:
		// DirectXCommonのインスタンス
		DirectXCommon* dxCommon_ = nullptr;

		//頂点バッファビュー
		D3D12_VERTEX_BUFFER_VIEW vbView_{};
		D3D12_RESOURCE_DESC resDesc_;
		// デバイス
		ComPtr<ID3D12Device> device_;
		//頂点バッファ
		ComPtr<ID3D12Resource> vertBuff_;
		// デスクリプタヒープ
		ComPtr<ID3D12DescriptorHeap> descHeap_;
		// 定数バッファ
		ComPtr<ID3D12Resource> constBuff_;
		// SRVの最大枚数
		static const size_t kMaxSRVCount_ = 2056;
		// テクスチャバッファ
		std::array<ComPtr<ID3D12Resource>, kMaxSRVCount_> texBuffers_;
		ConstBufferData* constMap = nullptr;

		// テクスチャ番号
		UINT texNumber_ = 0;
		uint32_t texIndex_ = 0;
		// 座標
		XMFLOAT3 position_ = { 0.0f, 0.0f, 0.0f };
		// サイズ
		XMFLOAT2 scale_ = { 1.0f, 1.0f };
		// 回転
		XMFLOAT3 rotation_ = { 0.0f,0.0f,0.0f };
		// ワールド行列
		XMMATRIX matWorld_;
		// アンカーポイント
		XMFLOAT2 anchorpoint_ = { 0.0f,0.0f };
		// テクスチャ左上座標設定
		XMFLOAT2 texLeftTop_ = { 50.0f, 50.0f };
		// テクスチャ切り出しサイズ
		XMFLOAT2 texSize_ = { 100.0f, 100.0f };
		// 色
		XMFLOAT4 color_ = { 1,1,1,1 };

		// Z軸回りの回転
		float f_rotation_ = 0.0f;
		// 左右反転
		bool isFlipX_ = false;
		// 上下反転
		bool isFlipY_ = false;
		// 非表示
		bool isInvisible_ = false;
	};
};