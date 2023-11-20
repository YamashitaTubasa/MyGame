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

	models_.insert(std::make_pair("building", buildingM_.get()));
	models_.insert(std::make_pair("asphalt", asphaltM_.get()));
	models_.insert(std::make_pair("bridge", bridgeM_.get()));
	models_.insert(std::make_pair("tree", treeM_.get()));
	//models_.insert(std::make_pair("wheel", wheelM_));

	// レベルデータからオブジェクトを生成、配置
	for (auto& objectData : levelData_->objects_) {
		// ファイル名から登録済みモデルを検索
		std::unique_ptr<Model> model = nullptr; 
		decltype(models_)::iterator it = models_.find(objectData.fileName_);
		if (it != models_.end()) { model = it->second; }
		// モデルを指定して3Dオブジェクトを生成
		std::unique_ptr<Object3d> newObject = Object3d::Create();
		newObject->SetModel(model.get());
		// 座標
		DirectX::XMFLOAT3 pos;
		DirectX::XMStoreFloat3(&pos, objectData.translation_);
		newObject->SetPosition(pos);
		// 回転
		DirectX::XMFLOAT3 rot;
		DirectX::XMStoreFloat3(&rot, objectData.rotation_);
		newObject->SetRotation(rot);
		// サイズ
		DirectX::XMFLOAT3 scale;
		DirectX::XMStoreFloat3(&scale, objectData.scaling_);
		newObject->SetScale(scale);

		// 配列に登録
		objects_.push_back(newObject.get());
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
