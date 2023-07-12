#include "MyGame.h"

void MyGame::Initialize()
{	
	// 基底クラスの初期化処理
	TYFramework::Initialize();

	// ゲームシーンの初期化
	gameScene = new GameScene();
	gameScene->Initialize(camera, spriteCommon);
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

	// ImGui受付開始
	imGuiManager->Begin();

	// ゲームシーンの更新
	gameScene->Update();

	// ImGui受付終了
	imGuiManager->End();
}

void MyGame::Draw()
{
#pragma region ゲームシーン描画

	// レンダーテクスチャの前処理
	postEffect->PreDrawScene(dXCommon->GetCommandList());

	//=== ゲームシーン描画 ===//
	gameScene->Draw(camera);

	// レンダーテクスチャの後処理
	postEffect->PostDrawScene(dXCommon->GetCommandList());

#pragma endregion

#pragma region 描画

	// 描画前処理
	dXCommon->PreDraw();

	//=== ポストエフェクトの描画 ===//
	postEffect->Draw(dXCommon->GetCommandList());

	// ImGui描画
	//imGuiManager->Draw(dXCommon);

	// 描画後処理
	dXCommon->PostDraw();

#pragma endregion
}
