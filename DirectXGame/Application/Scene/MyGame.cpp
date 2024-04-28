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
	postEffect_ = std::make_unique<MyEngine::PostEffect>();
	postEffect_->Initialize(L"Resources/shaders/PostEffectBlurPS.hlsl");
	postEffect1_ = std::make_unique<MyEngine::PostEffect>();
	postEffect1_->Initialize(L"Resources/shaders/PostEffectBlurPS.hlsl");

	// シーンファクトリを生成し、マネージャにセット
	sceneFactory_ = std::make_unique<SceneFactory>();
	sceneManager_->SetSceneFactory(sceneFactory_.get());
	// シーンマネージャに最初のシーンをセット
	sceneManager_->ChangeScene("TITLE");
}

void MyGame::Finalize()
{
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
#pragma region ゲームシーン描画

	//=== シーンマネージャの描画 ===//
	sceneManager_->Draw();

#pragma endregion
}
