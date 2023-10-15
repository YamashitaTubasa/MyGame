/**
 * @file Particle.h
 * @brief パーティクルに関するクラス
 * @author Yamashita Tubasa
 */

#pragma once

#pragma warning(push)
#pragma warning(disable:4061)
#pragma warning(disable:4265)
#pragma warning(disable:4625)
#pragma warning(disable:4626)
#pragma warning(disable:4668)
#pragma warning(disable:4820)
#pragma warning(disable:4365)
#pragma warning(disable:5039)
#pragma warning(disable:5204)
#pragma warning(disable:5220)
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <forward_list>
#include <string>
#pragma warning(pop)

class Particle
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // サブクラス
	// 頂点データ構造体
	struct VertexPos
	{
		XMFLOAT3 pos; // xyz座標
		float scale; // スケール
	};

	// パーティクル1粒
	struct ParticleOne
	{
		// DirectX::を省略
		using XNFLOAT3 = DirectX::XMFLOAT3;
		// 座標
		XMFLOAT3 position = {};
		// 速度
		XMFLOAT3 velocity = {};
		// 加速度
		XMFLOAT3 accel = {};
		// 現在フレーム
		int frame = 0;
		// 終了フレーム
		int num_frame = 0;
		// スケール
		float scale = 1.0f;
		// 初期値
		float s_scale = 1.0f;
		// 最終値
		float e_scale = 0.0f;
	};

private: // 定数
	static const int vertexCount_ = 1024; // 頂点数
	static const std::string baseDirectory_;

private: // 静的メンバ変数
	// デバイス
	static ID3D12Device* device_;
	// デスクリプタサイズ
	UINT descriptorHandleIncrementSize_;
	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> rootsignature_;
	// パイプラインステートオブジェクト
	ComPtr<ID3D12PipelineState> pipelinestate_;
	// デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeap_;
	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuff_;
	// テクスチャバッファ
	ComPtr<ID3D12Resource> texbuff_;
	// シェーダリソースビューのハンドル(CPU)
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV_;
	// シェーダリソースビューのハンドル(GPU)
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV_;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView_;
	// 頂点データ配列
	VertexPos vertices_[vertexCount_];

public:// 静的メンバ関数
	/// <summary>
	/// OBJファイルからパーティクルマネージャーを読み込む
	/// </summary>
	/// <param name="filename">ファイル名</param>
	/// <returns></returns>
	static Particle* LoadFromOBJ(const std::string& filename);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	void LoadTexture(const std::string& filename);

	/// <summary>
	/// デスクリプタヒープの初期化
	/// </summary>
	void InitializeDescriptorHeap();

	/// <summary>
	/// モデル作成
	/// </summary>
	void CreateModel();

	/// <summary>
	/// パーティクルの追加
	/// </summary>
	/// <param name = "life">生存時期</param>
	/// <param name = "position">初期座標</param>
	/// <param name = "velocity">速度</param>
	/// <param name = "accel">加速度</param>
	void Add(int life, XMFLOAT3 position, XMFLOAT3 velocity, XMFLOAT3 accel, float start_scale, float end_scale);

public:
	/// <summary>
	/// デバイスのセット
	/// </summary>
	/// <param name="device"></param>
	[[maybe_unused]] static void SetDevice(ID3D12Device* device) { Particle::device_ = device; } // device_のsetter

private: // メンバ変数
	// 親オブジェクト
	std::forward_list<ParticleOne> particles_;
};