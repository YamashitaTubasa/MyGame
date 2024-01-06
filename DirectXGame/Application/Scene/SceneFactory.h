/**
 * @file SceneFactory.h
 * @brief このゲームのシーンを生成するクラス
 * @author Yamashita Tubasa
 */

#pragma once

#include "AbstractSceneFactory.h"
#include "GameTitleScene.h"
#include "GamePlayScene.h"
#include "GameClearScene.h"
#include "GameOverScene.h"

class SceneFactory : public MyEngine::AbstractSceneFactory
{
public:
	/// <summary>
	/// シーン生成
	/// </summary>
	/// <param name="sceneName">シーン名</param>
	/// <returns></returns>
	GameBaseScene* CreateScene(const std::string& sceneName) override;
};