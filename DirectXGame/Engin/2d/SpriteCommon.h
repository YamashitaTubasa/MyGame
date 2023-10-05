#pragma once

#include "DirectXCommon.h"

#include <DirectXMath.h>
#include <d3dx12.h>
#include <array>
#include <d3dcompiler.inl>

#pragma comment (lib, "d3dcompiler.lib")

// パイプラインセット
struct PipelineSet {
	// パイプラインステート
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelinestate;
	// ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootsignature;
};

// =================//
// スプライト共通部 //
// =================//
class SpriteCommon final
{
private: // エイリアス
	// Microsoft::WRL::の省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using TexMetadata = DirectX::TexMetadata;
	using ScratchImage = DirectX::ScratchImage;

private:
	// コンストラクタ
	SpriteCommon() = default;
	// デストラクタ
	~SpriteCommon() = default;
	// コピーコンストラクタの禁止
	SpriteCommon(const SpriteCommon&) = delete;
	// 代入演算子の禁止
	SpriteCommon& operator=(const SpriteCommon&) = delete;

public:
	// インスタンスの取得
	static SpriteCommon* GetInstance();

public: // メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(DirectXCommon* dxCommon);

	/// <summary>
	/// 3Dオブジェクト用パイプライン生成
	/// </summary>
	void CreateGraphicsPipeline();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 描画前処理
	/// </summary>
	/// <param name="cmdList"></param>
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// 描画後処理
	/// </summary>
	static void PostDraw();

	/// <summary>
	/// テクスチャの読み込み
	/// </summary>
	/// <param name = "index"> テクスチャ番号 </param>
	/// <param name = "filename"> ファイル名 </param>
	void LoadTexture(uint32_t index, const wchar_t* filename);

public: // getter
	DirectXCommon* GetDirectXCommon() const{ return dxCommon; }

private:
	// デバイス
	ID3D12Device* device = nullptr;
	// 射影行列
	DirectX::XMMATRIX matProjection{};
	// テクスチャ用デスクリプタヒープの生成
	static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeap;
	// SRVの最大枚数
	static const size_t kMaxSRVCount = 2056;
	// コマンドリスト
	static ID3D12GraphicsCommandList* cmdList_;
	// テクスチャバッファ
	std::array<ComPtr<ID3D12Resource>, kMaxSRVCount> texBuffers_;

private: // Getter
	// テクスチャバッファ取得
	ID3D12Resource* GetTexBuffer(uint32_t index) const { return texBuffers_[index].Get(); }

private:
	DirectXCommon* dxCommon = nullptr;

	// テクスチャ番号
	uint32_t index_ = 0;
};