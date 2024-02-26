/**
 * @file SceneFactory.h
 * @brief シーンを生成するクラス
 * @author Yamashita Tubasa
 */

#pragma once

#include "GameBaseScene.h"
#include <string>

namespace MyEngine {
	class AbstractSceneFactory
	{
	public:
		/// 仮想デストラクタ
		virtual ~AbstractSceneFactory() = default;

		/// <summary>
		/// シーン生成
		/// </summary>
		/// <param name="sceneName">シーン名</param>
		/// <returns></returns>
		virtual GameBaseScene* CreateScene(const std::string& sceneName) = 0;
	};
};