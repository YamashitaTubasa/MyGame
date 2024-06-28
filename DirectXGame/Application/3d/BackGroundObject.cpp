/**
 * @file BackGroundObject.cpp
 * @brief 背景モデルを設置するクラス
 * @author Yamashita Tubasa
 */

#include "BackGroundObject.h"

BackGroundObject::BackGroundObject(){}

BackGroundObject::~BackGroundObject(){}

void BackGroundObject::Initialize()
{
	// レベルデータの読み込み
	levelData_ = LevelLoader::LoadFile("backGroundObject.json");

	// モデルデータを読み込む
	std::unique_ptr<Model> buildingM_ = Model::LoadFromOBJ("building");
	std::unique_ptr<Model> asphaltM_ = Model::LoadFromOBJ("asphalt");
	std::unique_ptr<Model> bridgeM_ = Model::LoadFromOBJ("bridge");
	std::unique_ptr<Model> treeM_ = Model::LoadFromOBJ("tree");
	std::unique_ptr<Model> wheelM_ = Model::LoadFromOBJ("wheel");
	std::unique_ptr<Model> floorM_ = Model::LoadFromOBJ("floor");
	std::unique_ptr<Model> enemyM_ = Model::LoadFromOBJ("enemy");

	models_.insert(std::make_pair("building", std::move(buildingM_)));
	models_.insert(std::make_pair("asphalt", std::move(asphaltM_)));
	models_.insert(std::make_pair("bridge", std::move(bridgeM_)));
	models_.insert(std::make_pair("tree", std::move(treeM_)));
	models_.insert(std::make_pair("wheel", std::move(wheelM_)));
	models_.insert(std::make_pair("floor", std::move(floorM_)));
	models_.insert(std::make_pair("enemy", std::move(enemyM_)));

	// レベルデータからオブジェクトを生成、配置
	for (auto& objectData : levelData_->objects_) {
		// ファイル名から登録済みモデルを検索
		Model* model = nullptr;
		decltype(models_)::iterator it = models_.find(objectData.fileName_);
		if (it != models_.end()) { model = it->second.get(); }
		// モデルを指定して3Dオブジェクトを生成
		std::unique_ptr<Object3d> newObject = Object3d::Create();
		newObject->SetModel(model);
		// 座標
		DirectX::XMFLOAT3 pos;
		DirectX::XMStoreFloat3(&pos, objectData.translation_);
		/*if (objectData.fileName_ == "wheel") {
			wheelPosition_ = { objectData.translation_.m128_f32[0], objectData.translation_.m128_f32[1], objectData.translation_.m128_f32[2] };
		}*/
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
		objects_.push_back(std::move(newObject));
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
