#pragma once

#pragma warning(push)
#pragma warning(disable:4061)
#pragma warning(disable:4265)
#pragma warning(disable:4365)
#pragma warning(disable:4625)
#pragma warning(disable:4626)
#pragma warning(disable:4668)
#pragma warning(disable:4820)
#pragma warning(disable:5039)
#pragma warning(disable:5204)
#pragma warning(disable:5220)
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <string>
#pragma warning(pop)

class Model 
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	// コンストラクタ
	Model();
	// デストラクタ
	~Model();

public: // サブクラス
	// 頂点データ構造体
	struct VertexPosNormalUv
	{
		XMFLOAT3 pos; // xyz座標
		XMFLOAT3 normal; // 法線ベクトル
		XMFLOAT2 uv;  // uv座標
	};

	// 定数バッファ用データ構造体B0
	struct ConstBufferDataB0
	{
		//XMFLOAT4 color;	// 色 (RGBA)
		XMMATRIX mat;	// ３Ｄ変換行列
	};

	// 定数バッファ用データ構造体B1
	struct ConstBufferDataB1
	{
		XMFLOAT3 ambient; // アンビエント係数
		float pad1;       // パディング
		XMFLOAT3 diffuse; // ディフューズ係数
		float pad2;       // パディング
		XMFLOAT3 specular; // スペキュラー係数
		float alpha;       // アルファ
	};

	// マテリアル
	struct Material
	{
		std::string name; // マテリアル名
		XMFLOAT3 ambient; // アンビエント影響度
		XMFLOAT3 diffuse; // ディフェーズ影響度
		XMFLOAT3 specular;// スペキュラー影響度
		float alpha;      // アルファ
		std::string textureFilename; // テクスチャファイル名
		// コンストラクタ
		Material() {
			ambient = { 0.3f,0.3f,0.3f };
			diffuse = { 0.0f,0.0f,0.0f };
			specular = { 0.0f,0.0f,0.0f };
			alpha = 1.0f;
		}
	};

private: // 定数
	static const int division = 50; // 分割数
	static const float radius; // 底面の半径
	static const float prizmHeight; // 柱の高さ
	static const int planeCount = division * 2 + division * 2; // 面の数
	static const int vertexCount = planeCount * 3; // 頂点数

public: // 静的メンバ関数
	
	// OBJファイルから3Dモデルを読み込む
	static Model* LoadFromOBJ(const std::string& modelname);

	// マテリアル読み込み
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);

	// テクスチャ読み込み
	void LoadTexture(const std::string& directoryPath, const std::string& filename);
	void LoadTexture(const std::string& filename = "Resources");

	// 描画
	/// <param name="cmdList">描画コマンドリスト</param>
	/// <param name="rootParamIndexMaterial">マテリアル用ルートパラメータ番号</param>
	void Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParamIndexMaterial);

	// setter
	[[maybe_unused]] static void SetDevice(ID3D12Device* device) { Model::device_ = device; }


private: // 静的メンバ変数
	// デバイス
	static ID3D12Device* device_;
	// 頂点データ配列
	std::vector<VertexPosNormalUv> vertices_;
	// 頂点インデックス配列
	std::vector<unsigned short> indices_;
	// マテリアル
	Material material_;
	// テクスチャバッファ
	ComPtr<ID3D12Resource> texbuff_;
	// デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeap_;
	// シェーダリソースビューのハンドル(CPU)
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV_;
	// シェーダリソースビューのハンドル(CPU)
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV_;
	// デスクリプタサイズ
	UINT descriptorHandleIncrementSize_;
	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuff_;
	// インデックスバッファ
	ComPtr<ID3D12Resource> indexBuff_;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView_;
	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView_;
	// 定数バッファ（マテリアル）
	ComPtr<ID3D12Resource> constBuffB1_; // 定数バッファ

private:// 静的メンバ関数
	// OBJファイルから3Dモデルを読み込む(非公開)
	void LoadFromOBJInternal(const std::string& modelname);

	// デスクリプタヒープの初期化
	void InitializeDescriptorHeap();

	// 各種バッファ生成
	void CreateBuffers();
};