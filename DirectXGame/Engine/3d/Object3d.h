/**
 * @file Object3d.h
 * @brief 3Dオブジェクトに関するクラス
 * @author Yamashita Tubasa
 */

#pragma once

#pragma warning(push)
#pragma warning(disable:4668)
#include <Windows.h>
#pragma warning(pop)

#pragma warning(push)
#pragma warning(disable:4820)
#include <d3d12.h>
#pragma warning(pop)

#include <wrl.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <string>

#include "Model.h"
#include "Camera.h"
#include "CollisionInfo.h"

class BaseCollider;

class Object3d
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

protected: // サブクラス
	// 定数バッファ用データ構造体B0
	struct ConstBufferDataB0
	{
		//XMFLOAT4 color;	// 色 (RGBA)
		XMMATRIX mat_;	// ３Ｄ変換行列
	};

	// ルートパラメータ
	enum Rootparams
	{
		rZero,
		rOne,
		rTwo,
		rMax
	};

	// シェーダ
	enum ShaderRegister
	{
		sZero,
		sOne,
	};

	// デスクリプタ
	enum Descriptor
	{
		dZero,
		dOne
	};

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Object3d() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Object3d();

public: // 静的メンバ関数
	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="device_">デバイス</param>
	/// <param name="window_width">画面幅</param>
	/// <param name="window_height">画面高さ</param>
	static void StaticInitialize(ID3D12Device* device_, int window_width, int window_height, [[maybe_unused]] Camera* camera);

	/// <summary>
	/// 描画前処理
	/// </summary>
	/// <param name="cmdList">描画コマンドリスト</param>
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// 描画後処理
	/// </summary>
	static void PostDraw();

	/// <summary>
	/// 3Dオブジェクト生成
	/// </summary>
	/// <returns></returns>
	static std::unique_ptr<Object3d> Create();

	/// <summary>
	/// 視点座標の取得
	/// </summary>
	/// <returns>座標</returns>
	static const XMFLOAT3& GetEye() { return eye_; }

	/// <summary>
	/// 視点座標の設定
	/// </summary>
	/// <param name="position">座標</param>
	static void SetEye(XMFLOAT3 eye);

	/// <summary>
	/// 注視点座標の取得
	/// </summary>
	/// <returns>座標</returns>
	static const XMFLOAT3& GetTarget() { return target_; }

	/// <summary>
	/// 注視点座標の設定
	/// </summary>
	/// <param name="position">座標</param>
	static void SetTarget(XMFLOAT3 target);

	/// <summary>
	/// ベクトルによる移動
	/// </summary>
	/// <param name="move">移動量</param>
	static void CameraMoveVector(XMFLOAT3 move);

private: // 静的メンバ変数
	//カメラ
	static Camera* camera_;
	// デバイス
	static ID3D12Device* device_;
	// コマンドリスト
	static ID3D12GraphicsCommandList* cmdList_;
	// ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootsignature_;
	// パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipelinestate_;
	// ビュー行列
	static XMMATRIX matView_;
	// 射影行列
	static XMMATRIX matProjection_;
	// 視点座標
	static XMFLOAT3 eye_;
	// 注視点座標
	static XMFLOAT3 target_;
	// 上方向ベクトル
	static XMFLOAT3 up_;

private:// 静的メンバ関数
	
	/// <summary>
	/// カメラ初期化
	/// </summary>
	/// <param name="window_width">画面横幅</param>
	/// <param name="window_height">画面縦幅</param>
	static void InitializeCamera(int window_width, int window_height);

	/// <summary>
	/// グラフィックパイプライン生成
	/// </summary>
	/// <returns>成否</returns>
	static void InitializeGraphicsPipeline();

	/// <summary>
	/// ビュー行列を更新
	/// </summary>
	static void UpdateViewMatrix();

public: // メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	virtual bool Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw();

public:
	/// <summary>
	/// モデルの設定
	/// </summary>
	/// <param name="model">モデル</param>
	void SetModel(Model* model) { this->model_ = model; }

	/// <summary>
	/// ワールド行列の取得
	/// </summary>
	/// <returns>ワールド行列</returns>
	const XMMATRIX& GetMatWorld() { return matWorld_; }

	/// <summary>
	/// コライダーのセット
	/// </summary>
	/// <param name="collider">コライダー</param>
	void SetCollider(BaseCollider* collider);

	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	/// <param name="info">衝突情報</param>
	virtual void OnCollision([[maybe_unused]] const CollisionInfo& info) {}

	/// <summary>
	/// オブジェクトの座標取得
	/// </summary>
	/// <returns>座標</returns>
	const XMFLOAT3& GetPosition() const { return position_; }

	/// <summary>
	/// オブジェクトの座標セット
	/// </summary>
	/// <param name="position">座標</param>
	void SetPosition(const XMFLOAT3& position) { this->position_ = position; }

	/// <summary>
	/// オブジェクトのサイズのセット
	/// </summary>
	/// <param name="scale_">サイズ</param>
	void SetScale(const XMFLOAT3& scale) { this->scale_ = scale; }

	/// <summary>
	/// オブジェクトのサイズの取得
	/// </summary>
	/// <returns>サイズ</returns>
	const XMFLOAT3& GetScale() const { return scale_; }

	/// <summary>
	/// オブジェクトの回転のセット
	/// </summary>
	/// <param name="rotation">回転</param>
	void SetRotation(const XMFLOAT3& rotation) { this->rotation_ = rotation; }

	/// <summary>
	/// オブジェクトの回転の取得
	/// </summary>
	/// <returns>回転</returns>
	const XMFLOAT3& GetRotation() const { return rotation_; }

	/// <summary>
	/// カメラ
	/// </summary>
	/// <param name="camera">カメラ</param>
	static void SetCamera(Camera* camera) { Object3d::camera_ = camera; }

protected: // メンバ変数
	// モデル
	Model* model_ = nullptr;
	// 定数バッファ
	ComPtr<ID3D12Resource> constBuffB0_; 
	// 色
	XMFLOAT4 color_ = { 1,1,1,1 };
	// ローカルスケール
	XMFLOAT3 scale_ = { 1,1,1 };
	// X,Y,Z軸回りのローカル回転角
	XMFLOAT3 rotation_ = { 0,0,0 };
	// ローカル座標
	XMFLOAT3 position_ = { 0,0,0 };
	// ローカルワールド変換行列
	XMMATRIX matWorld_;
	// 親オブジェクト
	Object3d* parent_ = nullptr;
	// クラス名（デバック用）
	const char* name_ = nullptr;
	// コライダー
	BaseCollider* collider_ = nullptr;
	// ルートパラメータ
	Rootparams rootparams_;
};

