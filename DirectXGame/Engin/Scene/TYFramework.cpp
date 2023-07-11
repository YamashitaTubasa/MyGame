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
	winApp = WinApp::GetInstance();
	winApp->Initialize();

	// DirectXの初期化
	dXCommon = DirectXCommon::GetInstance();
	dXCommon->Initialize(winApp);

	// 入力の初期化
	input = Input::GetInstance();
	input->Initialize(winApp);

	// ImGuiの初期化
	imGuiManager = new ImGuiManager();
	imGuiManager->Initialize(dXCommon, winApp);

	// Cameraの初期化
	camera = new Camera();
	camera->Initialize();

	// ポストエフェクト
	postEffect = PostEffect::GetInstance();
	postEffect->Initialize();

	// FBX
	FbxLoader::GetInstance()->Initialize(dXCommon->GetDevice());

	// 3Dオブジェクト静的初期化
	Object3d::StaticInitialize(dXCommon->GetDevice(), WinApp::window_width, WinApp::window_height, camera);
	// パーティクル静的初期化
	ParticleManager::StaticInitialize(dXCommon->GetDevice(), WinApp::window_width, WinApp::window_height);
}

void TYFramework::Finalize()
{
	FbxLoader::GetInstance()->Finalize();

	// imguiの終了処理
	imGuiManager->Finalize();
	// imguiの解放
	delete imGuiManager;
	imGuiManager = nullptr;

	// WindowsAPIの終了処理
	winApp->Finalize();

	// DirectX解放
	dXCommon->fpsFixedFinalize();
}

void TYFramework::Update()
{
	// Windowsのメッセージ処理
	if (winApp->ProcessMessage()) {
		// ゲームループを抜ける
		endRequst_ = true;
	}

	// 入力の更新
	input->Update();
}
