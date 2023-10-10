#pragma once

#include "FbxModel.h"
#include "FbxLoader.h"
#include "Camera.h"

#pragma warning(push)
#pragma warning(disable:4668)
#include <Windows.h>
#pragma warning(pop)

#pragma warning(push)
#pragma warning(disable:4820)
#include <d3d12.h>
#pragma warning(pop)

#include <wrl.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <string>

class FbxObject3d
{
protected: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DiectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // 定数
	// ボーンの最大数
	static const int MAX_BONES = 32;

public: // サブクラス
	// 定数バッファ用データ構造体（座標変換行列）
	struct ConstBufferDataTransform
	{
		XMMATRIX viewproj;  // ビュープロジェクション行列
		XMMATRIX world;     // ワールド行列
		XMFLOAT3 cameraPos; // カメラ座標（ワールド座標）
	};

	// 定数バッファ用データ構造体(スキニング)
	struct ConstBufferDataSkin
	{
		XMMATRIX bones[MAX_BONES];
	};

public: // メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// スキニング情報定数バッファ生成
	/// </summary>
	void GeneratSkinConstBuff();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="cmdList">コマンドリスト</param>
	void Draw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// アニメーション開始
	/// </summary>
	void PlayAnimation();

public: // 静的メンバ関数
	/// <summary>
	/// グラフィックスパイプラインの生成
	/// </summary>
	static void CreateGraphicsPipeline();

public: // 静的メンバ変数
	// setter
	static void SetDevice(ID3D12Device* device) { FbxObject3d::device_ = device; }
	static void SetCamera(Camera* camera) { FbxObject3d::camera_ = camera; }

public: // setter
	/// <summary>
	/// モデルのセット
	/// </summary>
	/// <param name="model">モデル</param>
	void SetModel(FbxModel* fbxModel) { this->fbxModel_ = fbxModel; }

	// 座標
	void SetPosition(const XMFLOAT3& position) { this->position_ = position; }
	// 回転
	void SetRotation(const XMFLOAT3& rotation) { this->rotation_ = rotation; }
	// スケール
	void SetScale(const XMFLOAT3& scale) { this->scale_ = scale; }

public: // getter
	// 座標
	XMFLOAT3& GetPosition() { return position_; }
	// 回転
	XMFLOAT3& GetRotation() { return rotation_; }
	// スケール
	XMFLOAT3& GetScale() { return scale_; }

private: // 静的メンバ変数
	// デバイス
	static ID3D12Device* device_;
	// カメラ
	static Camera* camera_;
	// ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootsignature_;
	// パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipelinestate_;

protected: // メンバ変数
	// 定数バッファ
	ComPtr<ID3D12Resource> constBuffTransform_;
	// 定数バッファ(スキン)
	ComPtr<ID3D12Resource> constBuffSkin_;
	// ローカルスケール
	XMFLOAT3 scale_ = { 1,1,1 };
	// X,Y,Z軸周りのローカル回転角
	XMFLOAT3 rotation_ = { 0,0,0 };
	// ローカル座標
	XMFLOAT3 position_ = { 0,0,0 };
	// ローカルワールド変換行列
	XMMATRIX matWorld_;
	// モデル
	FbxModel* fbxModel_ = nullptr;
	// 1フレームの時間
	FbxTime frameTime_;
	// アニメーション開始時間
	FbxTime startTime_;
	// アニメーション終了時間
	FbxTime endTime_;
	// 現在時間(アニメーション)
	FbxTime currentTime_;
	// アニメーション再生中
	bool isPlay_ = false;
};

