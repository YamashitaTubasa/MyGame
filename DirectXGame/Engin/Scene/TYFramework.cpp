/**
 * @file TYFramework.cpp
 * @brief フレームワーククラス
 * @author Yamashita Tubasa
 */

#include "TYFramework.h"

void TYFramework::Run()
{
	// ゲームの初期化
	Initialize();

	while (true) // ゲームループ 
	{
		// 毎フレーム更新
		Update();

		// 終了リクエストが来たらループを抜ける
		if (IsEndRequst()) {
			// ゲームループを抜ける
			break;
		}

		// 描画
		Draw();
	}

	// ゲームの終了
	Finalize();
}

void TYFramework::Initialize()
{
	// WindowsAPIの初期化
	winApp_ = WinApp::GetInstance();
	winApp_->Initialize();

	// DirectXの初期化
	dxCommon_ = DirectXCommon::GetInstance();
	dxCommon_->Initialize(winApp_);

	// 入力の初期化
	input_ = Input::GetInstance();
	input_->Initialize(winApp_);

	// ImGuiの初期化
	imGuiManager_ = std::make_unique<ImGuiManager>();
	imGuiManager_->Initialize(dxCommon_, winApp_);

	// シーンマネージャの生成
	sceneManager_ = GameSceneManager::GetInstance();

	// Cameraの初期化
	/*camera = new Camera();
	camera->Initialize();*/

	// FBX
	FbxLoader::GetInstance()->Initialize(dxCommon_->GetDevice());

	// 3Dオブジェクト静的初期化
	Object3d::StaticInitialize(dxCommon_->GetDevice(), WinApp::window_width, WinApp::window_height, camera_);

	// パーティクル静的初期化
	ParticleManager::StaticInitialize(dxCommon_->GetDevice(), WinApp::window_width, WinApp::window_height);
}

void TYFramework::Finalize()
{
	sceneManager_->Destroy();

	FbxLoader::GetInstance()->Finalize();

	// imguiの終了処理
	imGuiManager_->Finalize();

	// WindowsAPIの終了処理
	winApp_->Finalize();

	// DirectX解放
	dxCommon_->fpsFixedFinalize();
	dxCommon_->DeleteInstance();
}

void TYFramework::Update()
{
	// Windowsのメッセージ処理
	if (winApp_->ProcessMessage()) {
		// ゲームループを抜ける
		endRequst_ = true;
	}

	// 入力の更新
	input_->Update();

	// ImGui受付開始
	imGuiManager_->Begin();

	// シーンマネージャの更新
	sceneManager_->Update();

	// ImGui受付終了
	imGuiManager_->End();
}
