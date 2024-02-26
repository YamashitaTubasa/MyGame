/**
 * @file Model.h
 * @brief モデルの読み込みに関するクラス
 * @author Yamashita Tubasa
 */

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
		XMFLOAT3 pos_; // xyz座標
		XMFLOAT3 normal_; // 法線ベクトル
		XMFLOAT2 uv_;  // uv座標
	};

	// 定数バッファ用データ構造体B0
	struct ConstBufferDataB0
	{
		//XMFLOAT4 color;	// 色 (RGBA)
		XMMATRIX mat_;	// ３Ｄ変換行列
	};

	// 定数バッファ用データ構造体B1
	struct ConstBufferDataB1
	{
		XMFLOAT3 ambient_;  // アンビエント係数
		float pad1_;        // パディング
		XMFLOAT3 diffuse_;  // ディフューズ係数
		float pad2_;        // パディング
		XMFLOAT3 specular_; // スペキュラー係数
		float alpha_;       // アルファ
	};

	// マテリアル
	struct Material
	{
		std::string name_;  // マテリアル名
		XMFLOAT3 ambient_;  // アンビエント影響度
		XMFLOAT3 diffuse_;  // ディフェーズ影響度
		XMFLOAT3 specular_; // スペキュラー影響度
		float alpha_;       // アルファ
		std::string textureFilename_; // テクスチャファイル名
		// コンストラクタ
		Material() {
			ambient_ = { 0.3f,0.3f,0.3f };
			diffuse_ = { 0.0f,0.0f,0.0f };
			specular_ = { 0.0f,0.0f,0.0f };
			alpha_ = 1.0f;
		}
	};

private: // 定数
	static const int division_ = 50; // 分割数
	static const float radius_; // 底面の半径
	static const float prizmHeight_; // 柱の高さ
	static const int planeCount_ = division_ * 2 + division_ * 2; // 面の数
	static const int vertexCount_ = planeCount_ * 3; // 頂点数

public: // 静的メンバ関数
	
	/// <summary>
	/// OBJファイルから3Dモデルを読み込む
	/// </summary>
	/// <param name="modelname">モデル名</param>
	/// <returns></returns>
	static std::unique_ptr<Model> LoadFromOBJ(const std::string& modelname);

	/// <summary>
	/// マテリアル読み込み
	/// </summary>
	/// <param name="directoryPath">ディレクトリパス</param>
	/// <param name="filename"></param>
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);

	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	/// <param name="directoryPath"></param>
	/// <param name="filename"></param>
	void LoadTexture(const std::string& directoryPath, const std::string& filename);

	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	/// <param name="filename"></param>
	void LoadTexture(const std::string& filename = "Resources");

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="cmdList">描画コマンドリスト</param>
	/// <param name="rootParamIndexMaterial">マテリアル用ルートパラメータ番号</param>
	void Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParamIndexMaterial);

	/// <summary>
	/// デバイスのセット
	/// </summary>
	/// <param name="device"></param>
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
	/// <summary>
	/// OBJファイルから3Dモデルを読み込む(非公開)
	/// </summary>
	/// <param name="modelname"></param>
	void LoadFromOBJInternal(const std::string& modelname);

	/// <summary>
	/// デスクリプタヒープの初期化
	/// </summary>
	void InitializeDescriptorHeap();

	/// <summary>
	/// 各種バッファ生成
	/// </summary>
	void CreateBuffers();
};