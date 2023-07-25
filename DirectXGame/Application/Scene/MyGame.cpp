#include "MyGame.h"

void MyGame::Initialize()
{	
	// 基底クラスの初期化処理
	TYFramework::Initialize();

	// ポストエフェクト
	postEffect = new PostEffect();
	postEffect->Initialize();
	postEffect1 = new PostEffect();
	postEffect1->Initialize();

	// 最初のシーンの生成
	GameBaseScene* scene = new GameTitleScene();
	// シーンマネージャに最初のシーンをセット
	sceneManager_->SetNextScene(scene);
}

void MyGame::Finalize()
{
	delete postEffect;
	delete postEffect1;

	// 基底クラスの終了処理
	TYFramework::Finalize();
}

void MyGame::Update()
{
	// 基底クラスの更新処理
	TYFramework::Update();

	postEffect->SetBlur(true);
}

void MyGame::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dXCommon->GetCommandList();

#pragma region ゲームシーン描画

	// レンダーテクスチャの前処理
	postEffect->PreDrawScene(cmdList);

	//=== シーンマネージャの描画 ===//
	sceneManager_->Draw();

	// レンダーテクスチャの後処理
	postEffect->PostDrawScene(cmdList);

#pragma endregion

#pragma region ポストエフェクトの描画

	// レンダーテクスチャ1の前処理
	postEffect1->PreDrawScene(cmdList);

	//=== ポストエフェクトの描画 ===//
	postEffect->Draw(cmdList);

	// レンダーテクスチャ1の後処理
	postEffect1->PostDrawScene(cmdList);

#pragma endregion

#pragma region ポストエフェクト1の描画

	// 描画前処理
	dXCommon->PreDraw();

	//=== ポストエフェクト1の描画 ===//
	postEffect1->Draw(cmdList);

	// 描画後処理
	dXCommon->PostDraw();

#pragma endregion
}
