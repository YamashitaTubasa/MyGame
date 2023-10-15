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
	delete buildingM;
	delete asphaltM;
	delete bridgeM;
	delete treeM;
	delete wheelM;
	for (auto& object: objects) {
		delete object;
	}
}

void BackGroundObject::Initialize()
{
	// レベルデータの読み込み
	levelData = LevelLoader::LoadFile("backGroundObject.json");

	// モデルデータを読み込む
	buildingM = Model::LoadFromOBJ("building");
	asphaltM = Model::LoadFromOBJ("asphalt");
	bridgeM = Model::LoadFromOBJ("bridge");
	treeM = Model::LoadFromOBJ("tree");
	wheelM = Model::LoadFromOBJ("wheel");

	models.insert(std::make_pair("building", buildingM));
	models.insert(std::make_pair("asphalt", asphaltM));
	models.insert(std::make_pair("bridge", bridgeM));
	models.insert(std::make_pair("tree", treeM));
	models.insert(std::make_pair("wheel", wheelM));

	// レベルデータからオブジェクトを生成、配置
	for (auto& objectData : levelData->objects) {
		// ファイル名から登録済みモデルを検索
		Model* model = nullptr;
		decltype(models)::iterator it = models.find(objectData.fileName);
		if (it != models.end()) { model = it->second; }
		// モデルを指定して3Dオブジェクトを生成
		Object3d* newObject = Object3d::Create();
		newObject->SetModel(model);
		// 座標
		DirectX::XMFLOAT3 pos;
		DirectX::XMStoreFloat3(&pos, objectData.translation);
		newObject->SetPosition(pos);
		// 回転
		DirectX::XMFLOAT3 rot;
		DirectX::XMStoreFloat3(&rot, objectData.rotation);
		newObject->SetRotation(rot);
		// サイズ
		DirectX::XMFLOAT3 scale;
		DirectX::XMStoreFloat3(&scale, objectData.scaling);
		newObject->SetScale(scale);

		// 配列に登録
		objects.push_back(newObject);
	}

	modelEye = { 0.0f,0.0f,-3.0f };
	Object3d::SetEye(modelEye);
}

void BackGroundObject::Update()
{
	for (auto& object : objects) {
		object->Update();
	}
}

void BackGroundObject::Draw()
{
	for (auto& object : objects) {
		object->Draw();
	}
}
