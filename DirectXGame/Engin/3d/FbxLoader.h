/**
 * @file FbxLoader.h
 * @brief Fbxの読み込みに関するクラス
 * @author Yamashita Tubasa
 */

#pragma once

#include "fbxsdk.h"
#include "FbxModel.h"

#pragma warning(push)
#pragma warning(disable:4820)
#include <d3d12.h>
#include <d3dx12.h>
#include <cassert>
#include <string>
#pragma warning(pop)

class FbxLoader
{
private: // エイリアス
	// std::を省略
	using string = std::string;

public: // 定数
	// モデル格納ルートパス
	static const string baseDirectory;
	// テクスチャがない場合の標準テクスチャファイル名
	static const string defaultTextureFileName;

public:
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static FbxLoader* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="device_">D3D12デバイス</param>
	void Initialize(ID3D12Device* device);

	/// <summary>
	/// 後始末
	/// </summary>
	void Finalize();

	/// <summary>
	/// ファイルからFBXモデル読み込み
	/// </summary>
	/// <param name="modelName">モデル名</param>
	FbxModel* LoadModelFromFile(const string& modelName);

	/// <summary>
	/// 再帰的にノード構成を解析
	/// </summary>
	/// <param name="fbxModel">読み込み先モデルオブジェクト</param>
	/// <param name="fbxNode">解析対象のノード</param>
	void ParseNodeRecursive(FbxModel* fbxModel, FbxNode* fbxNode, Node* parent = nullptr);

	/// <summary>
	/// メッシュ読み取り
	/// </summary>
	/// <param name="odel">読み込み先モデルオブジェクト</param>
	/// <param name="fbxNode">解析対象のノード</param>
	void ParseMesh(FbxModel* fbxModel, FbxNode* fbxNode);

	/// <summary>
	/// FBXの行列をXMMatrixに変換する
	/// </summary>
	/// <param name="dst">書き込み先</param>
	/// <param name="src">元となるFBX行列</param>
	static void ConvertMatrixFromFbx(DirectX::XMMATRIX* dst, const FbxAMatrix& src);

public: // サブ関数
	/// <summary>
	/// 頂点座標読み取り
	/// </summary>
	/// <param name="fbxModel">モデル</param>
	/// <param name="fbxMesh">メッシュ</param>
	void ParseMeshVertices(FbxModel* fbxModel, FbxMesh* fbxMesh);

	/// <summary>
	/// 面情報読み取り
	/// </summary>
	/// <param name="fbxModel">モデル</param>
	/// <param name="fbxMesh">メッシュ/param>
	void ParseMeshFaces(FbxModel* fbxModel, FbxMesh* fbxMesh);

	/// <summary>
	/// マテリアル読み取り
	/// </summary>
	/// <param name="fbxModel">モデル</param>
	/// <param name="fbxNode">ノード</param>
	void ParseMaterial(FbxModel* fbxModel, FbxNode* fbxNode);

	/// <summary>
	/// スキニング情報の読み取り
	/// </summary>
	/// <param name="fbxModel">モデル</param>
	/// <param name="fbxMesh">メッシュ</param>
	void ParseSkin(FbxModel* fbxModel, FbxMesh* fbxMesh);

	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	/// <param name="fbxModel">モデル</param>
	/// <param name="fullpath">パス</param>
	void LoadTexture(FbxModel* fbxModel, const std::string& fullpath);

	/// <summary>
	/// ディレクトリを含んだファイルパスからファイル名を抽出する
	/// </summary>
	/// <param name="path">パス</param>
	/// <returns></returns>
	std::string ExtractFileName(const std::string& path);

private:
	// privateなコンストラクタ（シングルトンパターン）
	FbxLoader() = default;
	// privateなデストラクタ（シングルトンパターン）
	~FbxLoader() = default;
	// コピーコンストラクタを禁止（シングルトンパターン）
	FbxLoader(const FbxLoader& obj) = delete;
	// コピー代入演算子を禁止（シングルトンパターン）
	void operator=(const FbxLoader& obj) = delete;

private:
	// D3D12デバイス
	ID3D12Device* device_ = nullptr;
	// FBXマネージャ
	FbxManager* fbxManager_ = nullptr;
	// FBXインポータ
	FbxImporter* fbxImporter_ = nullptr;
};