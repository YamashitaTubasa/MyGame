#pragma once

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"

#include <string>
#include <DirectXMath.h>
#include <vector>
#include <DirectXTex.h>
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <fbxsdk.h>

struct Node
{
	// 名前
	std::string name;
	// ローカルスケール
	Vector4 scaling = { 1,1,1,0 };
	// ローカル回転角
	Vector4 rotation = { 0,0,0,0 };
	// ローカル移動
	Vector4 translation = { 0,0,0,1 };
	// ローカル変形行列
	Matrix4 transform;
	// グローバル変形行列
	Matrix4 globalTransform;
	// 親ノード
	Node* parent = nullptr;
};

class FbxModel 
{
public:
	// フレンドクラス
	friend class FbxLoader;

private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
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
		Vector3 pos;    // xyz座標
		Vector3 normal; // 法線ベクトル
		Vector2 uv;     // uv座標
		UINT boneIndex[MAX_BONE_INDICES];   // ボーン　番号
		float boneWeight[MAX_BONE_INDICES]; // ボーン　重み
	};

	// ボーン構造体
	struct Bone
	{
		// 名前
		std::string name;
		// 初期姿勢の逆行列
		Matrix4 invInitialPose;
		// クラスター(FBX側のボーン情報)
		FbxCluster* fbxCluster;
		// コンストラクタ
		Bone(const std::string& name) {
			this->name = name;
		}
	};

public:
	// デストラクタ
	~FbxModel();

public:
	// バッファ生成
	void CreateBuffers(ID3D12Device* device);
	// 描画
	void Draw(ID3D12GraphicsCommandList* cmdList);

public: // getter
	// モデルの変形行列取得
	const Matrix4& GetModelTransform() { return meshNode->globalTransform; }
	// bonesの取得
	std::vector<Bone>& GetBones() { return bones; }
	// fbxSceneの取得
	FbxScene* GetFbxScene() { return fbxScene; }

private:
	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;
	// インデックスバッファ
	ComPtr<ID3D12Resource> indexBuff;
	// テクスチャバッファ
	ComPtr<ID3D12Resource> texbuff;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView = {};
	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView = {};
	// SRV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;

private:
	// メッシュを持つノード
	Node* meshNode = nullptr;
	// FBXシーン
	FbxScene* fbxScene = nullptr;
	// モデル名
	std::string name;
	// ノード配列
	std::vector<Node> nodes;
	// 頂点データ配列
	std::vector<VertexPosNormalUvSkin> vertices;
	// 頂点インデックス配列
	std::vector<unsigned short> indices;
	// ボーン配列
	std::vector<Bone> bones;
	// アンビエント係数
	Vector3 ambient = { 1,1,1 };
	// ディフューズ係数
	Vector3 diffuse = { 1,1,1 };
	// テクスチャメタデータ
	DirectX::TexMetadata metadata = {};
	// スクラッチイメージ
	DirectX::ScratchImage scratchImg = {};
};
