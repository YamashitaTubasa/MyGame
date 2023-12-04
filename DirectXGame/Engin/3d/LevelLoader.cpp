/**
 * @file LevelLoader.cpp
 * @brief レベルデータを読み込みに関するクラス
 * @author Yamashita Tubasa
 */

#include "LevelLoader.h"

LevelData* LevelLoader::LoadFile(const std::string& fileName)
{
	// 凍結してフルパスを得る
	const std::string fullpath = std::string("Resources/levelLoaders/") + fileName;

	// ファイルストリーム
	std::ifstream file;

	// ファイルを開く
	file.open(fullpath);
	// ファイルオープン失敗をチェック
	if (file.fail()) {
		assert(0);
	}

	// JSON文字列から解凍したデータ
	nlohmann::json deserialized;

	// 解凍
	file >> deserialized;

	// 正しいレベルデータファイルかチェック
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	// "name"を文字列として取得
	std::string name = deserialized["name"].get<std::string>();
	// 正しいレベルデータファイルかチェック
	assert(name.compare("scene") == 0);

	// レベルデータ格納用インスタンスを生成
	LevelData* levelData = new LevelData();

	// "objects"の全オブジェクトを走査
	for (nlohmann::json& object : deserialized["objects"]) {
		assert(object.contains("type"));

		// 種別を取得
		std::string type = object["type"].get<std::string>();

		//--------------- 種類ごとの処理 ---------------//
		// MESH
		if (type.compare("MESH") == 0) {
			// 要素追加
			levelData->objects_.emplace_back(LevelData::ObjectData{});
			// 今後追加した要素の参照を得る
			LevelData::ObjectData& objectData = levelData->objects_.back();

			if (object.contains("file_name")) {
				// ファイル名
				objectData.fileName_ = object["file_name"];
			}

			// トランスフォームのパラメータ読み込み
			nlohmann::json& transform = object["transform"];
			// 平行移動
			objectData.translation_.x = (float)transform["translation"][0];
			objectData.translation_.y = (float)transform["translation"][2];
			objectData.translation_.z = (float)transform["translation"][1];
			//objectData.translation_.m128_f32[3] = 1.0f;
			// 回転角
			objectData.rotation_.x = -(float)transform["rotation"][0];
			objectData.rotation_.y = -(float)transform["rotation"][2];
			objectData.rotation_.z = -(float)transform["rotation"][1];
			//objectData.rotation_.m128_f32[3] = 1.0f;
			// スケーリング
			objectData.scaling_.x = (float)transform["scaling"][1];
			objectData.scaling_.y = (float)transform["scaling"][2];
			objectData.scaling_.z = (float)transform["scaling"][0];
			//objectData.scaling_.m128_f32[3] = 1.0f;
		}

		// オブジェクト走査を再帰関数にまとめる、再帰呼出で枝を走査する
		if (object.contains("children")) {
			// 要素追加
			levelData->objects_.emplace_back(LevelData::ObjectData{});
			// 今後追加した要素の参照を得る
			LevelData::ObjectData& objectData = levelData->objects_.back();

			if (object.contains("file_name")) {
				// ファイル名
				objectData.fileName_ = object["file_name"];
			}

			// トランスフォームのパラメータ読み込み
			nlohmann::json& transform = object["transform"];
			// 平行移動
			objectData.translation_.x = (float)transform["translation"][0];
			objectData.translation_.y = (float)transform["translation"][2];
			objectData.translation_.z = (float)transform["translation"][1];
			//objectData.translation_.m128_f32[3] = 1.0f;
			// 回転角
			objectData.rotation_.x = -(float)transform["rotation"][0];
			objectData.rotation_.y = -(float)transform["rotation"][2];
			objectData.rotation_.z = -(float)transform["rotation"][1];
			//objectData.rotation_.m128_f32[3] = 1.0f;
			// スケーリング
			objectData.scaling_.x = (float)transform["scaling"][1];
			objectData.scaling_.y = (float)transform["scaling"][2];
			objectData.scaling_.z = (float)transform["scaling"][0];
			//objectData.scaling_.m128_f32[3] = 1.0f;
		}
	}

	return levelData;
}
