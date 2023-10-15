/**
 * @file LevelLoader.h
 * @brief レベルデータを読み込みに関するクラス
 * @author Yamashita Tubasa
 */

#pragma once

#include "Vector3.h"

#include <json.hpp>
#include <string>
#include <fstream>
#include <DirectXMath.h>

// レベルデータ
struct LevelData {
	// オブジェクトデータ
	struct ObjectData {
		// 移動
		DirectX::XMVECTOR translation;
		// 回転角
		DirectX::XMVECTOR rotation;
		// スケール
		DirectX::XMVECTOR scaling;
		// ファイル名
		std::string fileName;
	};

	std::vector<ObjectData> objects;
};

class LevelLoader
{
public:
	/// <summary>
	/// レベルデータファイル読み込み
	/// </summary>
	/// <returns>ファイル名</returns>
	static LevelData* LoadFile(const std::string& fileName);
};
