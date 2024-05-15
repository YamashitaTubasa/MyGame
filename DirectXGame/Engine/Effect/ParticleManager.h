/**
 * @file ParticleManager.h
 * @brief パーティクル管理クラス
 * @author Yamashita Tubasa
 */

#pragma once

#include "Particle.h"

#pragma warning(push)
#pragma warning(disable:4668)
#pragma warning(disable:4820)
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <forward_list>
#include <string>
#pragma warning(pop)

namespace MyEngine 
{
	class ParticleManager
	{
	private: // エイリアス
		// Microsoft::WRL::を省略
		template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
		// DirectX::を省略
		using XMFLOAT2 = DirectX::XMFLOAT2;
		using XMFLOAT3 = DirectX::XMFLOAT3;
		using XMFLOAT4 = DirectX::XMFLOAT4;
		using XMMATRIX = DirectX::XMMATRIX;

	public: // サブクラス
		// 定数バッファ用データ構造体
		struct ConstBufferData
		{
			XMMATRIX mat;	// ３Ｄ変換行列
			XMMATRIX matBillboard; // ビルボード行列
		};

	private: // 定数
		static const int division = 50;                            // 分割数
		static const float radius;                                 // 底面の半径
		static const float prizmHeight;                            // 柱の高さ
		static const int planeCount = division * 2 + division * 2; // 面の数
		static const int vertexCount = 1024;                       // 頂点数

	public: // 静的メンバ関数
		/// <summary>
		/// 静的初期化
		/// </summary>
		/// <param name="device_">デバイス</param>
		/// <param name="window_width">画面幅</param>
		/// <param name="window_height">画面高さ</param>
		static void StaticInitialize(ID3D12Device* device, int window_width, int window_height);

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
		static std::unique_ptr<ParticleManager> Create();

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

		/// <summary>
		/// ベクトルによる視点移動
		/// </summary>
		/// <param name = "move">移動量</param>
		static void CameraMoveEyeVector(XMFLOAT3 move);

	private: // 静的メンバ変数
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
		// ビルボード行列
		static XMMATRIX matBillboard_;
		// Y軸回りビルボード行列
		static XMMATRIX matBillboardY_;

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
		/// <returns></returns>
		bool Initialize();

		/// <summary>
		/// 毎フレーム処理
		/// </summary>
		void Update();

		/// <summary>
		/// 描画
		/// </summary>
		void Draw();

		/// <summary>
		/// モデルの設定
		/// </summary>
		/// <param name="particle"></param>
		[[maybe_unused]] void SetModel(Particle* particle) { this->particle_ = particle; }

		/// <summary>
		/// パーティクル実装
		/// </summary>
		void Execution(Particle* particle, float posx, float posy, float posz, int life, float start_scale, float end_scale);

	private: // メンバ変数
		// 定数バッファ
		ComPtr<ID3D12Resource> constBuff_;
		// スケール
		XMFLOAT3 scale_ = { 1,1,1 };
		// パーティクル
		Particle* particle_ = nullptr;
	};
};