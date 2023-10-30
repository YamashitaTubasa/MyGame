/**
 * @file MyGame.cpp
 * @brief ゲーム全体を司るクラス
 * @author Yamashita Tubasa
 */

#include "MyGame.h"

#include "SceneFactory.h"

void MyGame::Initialize()
{	
	// 基底クラスの初期化処理
	TYFramework::Initialize();

	// ポストエフェクト
	postEffect_ = new PostEffect();
	postEffect_->Initialize(L"Resources/shaders/PostEffectPS.hlsl");
	postEffect1_ = new PostEffect();
	postEffect1_->Initialize(L"Resources/shaders/PostEffectPS.hlsl");

	// シーンファクトリを生成し、マネージャにセット
	sceneFactory_ = new SceneFactory();
	sceneManager_->SetSceneFactory(sceneFactory_);
	// シーンマネージャに最初のシーンをセット
	sceneManager_->ChangeScene("GAMEPLAY");
}

void MyGame::Finalize()
{
	delete postEffect_;
	delete postEffect1_;

	// 基底クラスの終了処理
	TYFramework::Finalize();
}

void MyGame::Update()
{
	// 基底クラスの更新処理
	TYFramework::Update();
}

void MyGame::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon_->GetCommandList();

#pragma region ゲームシーン描画

	// レンダーテクスチャの前処理
	postEffect_->PreDraw(cmdList);

	//=== シーンマネージャの描画 ===//
	sceneManager_->Draw();

	// レンダーテクスチャの後処理
	postEffect_->PostDraw(cmdList);

#pragma endregion

#pragma region ポストエフェクトの描画

	// レンダーテクスチャ1の前処理
	postEffect1_->PreDraw(cmdList);

	//=== ポストエフェクトの描画 ===//
	postEffect_->Draw(cmdList);

	// レンダーテクスチャ1の後処理
	postEffect1_->PostDraw(cmdList);

#pragma endregion

#pragma region ポストエフェクト1の描画

	// 描画前処理
	dxCommon_->PreDraw();

	//=== ポストエフェクト1の描画 ===//
	postEffect1_->Draw(cmdList);

	// 描画後処理
	dxCommon_->PostDraw();

#pragma endregion
}
