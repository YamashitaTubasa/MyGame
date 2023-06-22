#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <forward_list>
#include <string>

//==============
// パーティクル
//==============
class ParticleM
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
	struct Particle0
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
	static const int vertexCount = 1024; // 頂点数

private: // 静的メンバ変数
    // デバイス
	static ID3D12Device* device;
	// デスクリプタサイズ
	static UINT descriptorHandleIncrementSize;
	// ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootsignature;
	// パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipelinestate;
	// デスクリプタヒープ
	static ComPtr<ID3D12DescriptorHeap> descHeap;
	// 頂点バッファ
	static ComPtr<ID3D12Resource> vertBuff;
	// テクスチャバッファ
	static ComPtr<ID3D12Resource> texbuff;
	// シェーダリソースビューのハンドル(CPU)
	static CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	// シェーダリソースビューのハンドル(CPU)
	static CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
	// 頂点バッファビュー
	static D3D12_VERTEX_BUFFER_VIEW vbView;
	// 頂点データ配列
	static VertexPos vertices[vertexCount];

public:// 静的メンバ関数
	/// <summary>
	/// OBJファイルからパーティクルマネージャーを読み込む
	/// </summary>
	/// <param name="filename">ファイル名</param>
	/// <returns></returns>
	static ParticleM* LoadFromOBJ(const std::string& filename);

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
	static void SetDevice(ID3D12Device* device) { ParticleM::device = device; }

private: // メンバ変数
	// 親オブジェクト
	std::forward_list<Particle0> particles;
};