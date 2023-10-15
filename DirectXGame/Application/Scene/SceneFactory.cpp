/**
 * @file SceneFactory.cpp
 * @brief このゲームのシーンを生成するクラス
 * @author Yamashita Tubasa
 */

#include "SceneFactory.h"

GameBaseScene* SceneFactory::CreateScene(const std::string& sceneName)
{
    // 次のシーン生成
    GameBaseScene* newScene = nullptr;

    if (sceneName == "TITLE") {
        newScene = new GameTitleScene();
    }
    else if (sceneName == "GAMEPLAY") {
        newScene = new GamePlayScene();
    }
    else if (sceneName == "CLEAR") {
        newScene = new GameClearScene();
    }
    else if (sceneName == "OVER") {
        newScene = new GameOverScene();
    }

    return newScene;
}
