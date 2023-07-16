#include "GameSceneManager.h"

GameSceneManager::~GameSceneManager()
{
	// 最後のシーンの終了と解放
	scene_->Finalize();
	delete scene_;
}

GameSceneManager* GameSceneManager::GetInstance() 
{
	static GameSceneManager instance;

	return &instance;
}

void GameSceneManager::Update()
{
	// 予約シーンの予約があるなら
	if (nextScene_) {
		// 旧シーンの終了
		if (scene_) {
			scene_->Finalize();

			delete scene_;
		}

		// シーン切り替え
		scene_ = nextScene_;
		nextScene_ = nullptr;

		// シーンマネージャをセット
		scene_->SetSceneManager(this);

		// 次シーンを初期化する
		scene_->Initialize(spriteCommon);
	}

	// 実行中シーンを更新する
	scene_->Update();
}

void GameSceneManager::Draw()
{
	scene_->Draw();
}
