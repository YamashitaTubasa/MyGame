/**
 * @file BackGroundObject.cpp
 * @brief 背景モデルを設置するクラス
 * @author Yamashita Tubasa
 */

#include "BackGroundObject.h"

BackGroundObject::BackGroundObject()
{
}

BackGroundObject::~BackGroundObject()
{
	delete buildingM_;
	delete asphaltM_;
	delete bridgeM_;
	delete treeM_;
	delete wheelM_;
	for (auto& object: objects_) {
		delete object;
	}
}

void BackGroundObject::Initialize()
{
	// レベルデータの読み込み
	levelData_ = LevelLoader::LoadFile("backGroundObject.json");

	// モデルデータを読み込む
	buildingM_ = Model::LoadFromOBJ("building");
	asphaltM_ = Model::LoadFromOBJ("asphalt");
	bridgeM_ = Model::LoadFromOBJ("bridge");
	treeM_ = Model::LoadFromOBJ("tree");
	//wheelM_ = Model::LoadFromOBJ("wheel");

	models_.insert(std::make_pair("building", buildingM_));
	models_.insert(std::make_pair("asphalt", asphaltM_));
	models_.insert(std::make_pair("bridge", bridgeM_));
	models_.insert(std::make_pair("tree", treeM_));
	//models_.insert(std::make_pair("wheel", wheelM_));

	// レベルデータからオブジェクトを生成、配置
	for (auto& objectData : levelData_->objects_) {
		// ファイル名から登録済みモデルを検索
		Model* model = nullptr;
		decltype(models_)::iterator it = models_.find(objectData.fileName_);
		if (it != models_.end()) { model = it->second; }
		// モデルを指定して3Dオブジェクトを生成
		Object3d* newObject = Object3d::Create();
		newObject->SetModel(model);
		// 座標
		Vector3 pos;
		pos = objectData.translation_;
		newObject->SetPosition(pos);
		// 回転
		Vector3 rot;
		rot = objectData.rotation_;
		newObject->SetRotation(rot);
		// サイズ
		Vector3 scale;
		scale = objectData.scaling_;
		newObject->SetScale(scale);

		// 配列に登録
		objects_.push_back(newObject);
	}

	modelEye_ = eye_;
	Object3d::SetEye(modelEye_);
}

void BackGroundObject::Update()
{
	for (auto& object : objects_) {
		object->Update();
	}
}

void BackGroundObject::Draw()
{
	for (auto& object : objects_) {
		object->Draw();
	}
}
