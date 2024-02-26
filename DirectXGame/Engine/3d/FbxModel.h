/**
 * @file FbxModel.h
 * @brief Fbxのモデルに関するクラス
 * @author Yamashita Tubasa
 */

#pragma once

#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"

#pragma warning(push)
#pragma warning(disable:4668)
#pragma warning(disable:4820)
#include <string>
#include <DirectXMath.h>
#include <vector>
#include <DirectXTex.h>
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <fbxsdk.h>
#pragma warning(pop)

struct Node
{
	// ローカルスケール
	DirectX::XMVECTOR scaling_ = { 1,1,1,0 };
	// ローカル回転角
	DirectX::XMVECTOR rotation_ = { 0,0,0,0 };
	// ローカル移動
	DirectX::XMVECTOR translation_ = { 0,0,0,1 };
	// ローカル変形行列
	DirectX::XMMATRIX transform_;
	// グローバル変形行列
	DirectX::XMMATRIX globalTransform_;
	// 名前
	std::string name_;
	// 親ノード
	Node* parent_ = nullptr;
};

class FbxModel 
{
public:
	// フレンドクラス
	friend class FbxLoader;

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

public: // 定数
	// ボーンインデックスの最大数
	static const int MAX_BONE_INDICES = 4;

public: // サブクラス
	// 頂点デー構造体
	struct VertexPosNormalUvSkin 
	{
		DirectX::XMFLOAT3 pos_;              // xyz座標
		DirectX::XMFLOAT3 normal_;           // 法線ベクトル
		DirectX::XMFLOAT2 uv_;               // uv座標
		UINT boneIndex_[MAX_BONE_INDICES];   // ボーン　番号
		float boneWeight_[MAX_BONE_INDICES]; // ボーン　重み
	};

	// ボーン構造体
	struct Bone
	{
		// 名前
		std::string name_;
		// 初期姿勢の逆行列
		DirectX::XMMATRIX invInitialPose_;
		// クラスター(FBX側のボーン情報)
		FbxCluster* fbxCluster_;
		// コンストラクタ
		Bone(const std::string& name) {
			this->name_ = name;
		}
	};

public:
	// デストラクタ
	~FbxModel();

public:
	/// <summary>
	/// バッファ生成
	/// </summary>
	/// <param name="device">デバイス</param>
	void CreateBuffers(ID3D12Device* device);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="cmdList">コマンドリスト</param>
	void Draw(ID3D12GraphicsCommandList* cmdList);

public: // getter
	/// <summary>
	/// モデルの変形行列取得
	/// </summary>
	/// <returns>ノード</returns>
	const XMMATRIX& GetModelTransform() { return meshNode_->globalTransform_; }

	/// <summary>
	/// bonesの取得
	/// </summary>
	/// <returns>ボーン</returns>
	std::vector<Bone>& GetBones() { return bones_; }

	/// <summary>
	/// fbxSceneの取得
	/// </summary>
	/// <returns></returns>
	FbxScene* GetFbxScene() { return fbxScene_; }

private:
	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuff_;
	// インデックスバッファ
	ComPtr<ID3D12Resource> indexBuff_;
	// テクスチャバッファ
	ComPtr<ID3D12Resource> texbuff_;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView_ = {};
	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView_ = {};
	// SRV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapSRV_;

private:
	// メッシュを持つノード
	Node* meshNode_ = nullptr;
	// FBXシーン
	FbxScene* fbxScene_ = nullptr;
	// モデル名
	std::string name_;
	// ノード配列
	std::vector<Node> nodes_;
	// 頂点データ配列
	std::vector<VertexPosNormalUvSkin> vertices_;
	// 頂点インデックス配列
	std::vector<unsigned short> indices_;
	// ボーン配列
	std::vector<Bone> bones_;
	// アンビエント係数
	DirectX::XMFLOAT3 ambient_ = { 1,1,1 };
	// ディフューズ係数
	DirectX::XMFLOAT3 diffuse_ = { 1,1,1 };
	// テクスチャメタデータ
	DirectX::TexMetadata metadata_ = {};
	// スクラッチイメージ
	DirectX::ScratchImage scratchImg_ = {};
};
