#include "MyGame.h"

void MyGame::Initialize()
{	
	// 基底クラスの初期化処理
	TYFramework::Initialize();

	//dXCommon = DirectXCommon::GetInstance();

	// ゲームシーンの初期化
	gameScene = new GameScene();
	gameScene->Initialize(spriteCommon);
}

void MyGame::Finalize()
{
	// ゲームシーンの解放
	gameScene->Finalize();
	delete gameScene;

	// 基底クラスの終了処理
	TYFramework::Finalize();
}

void MyGame::Update()
{
	// 基底クラスの更新処理
	TYFramework::Update();

	// ゲームシーンの更新
	gameScene->Update();
}

void MyGame::Draw()
{
#pragma region 描画

	// 描画前処理
	dXCommon->PreDraw();

	// ゲームシーンの描画
	gameScene->Draw(spriteCommon);

	// ImGui描画
	//imGuiManager->Draw(dXCommon);

	// 描画後処理
	dXCommon->PostDraw();

#pragma endregion
}
