#include "Skydome.h"

Skydome::Skydome()
{
}

Skydome::~Skydome()
{
	delete titleObj;
	delete titleModel;
}

void Skydome::Initialize()
{
	// OBJからモデルデータを読み込む
	titleModel = Model::LoadFromOBJ("skydome");
	// 3Dオブジェクトの生成
	titleObj = Object3d::Create();
	// 3Dオブジェクトにモデルをセット
	titleObj->SetModel(titleModel);
	// オブジェクトの位置の設定
	titleObj->SetPosition({ 0,30,0 });
	titleObj->SetScale({ 300,300,300 });
	titleObj->SetRotation({ 0,0,0 });
}

void Skydome::Update()
{
	// オブジェクトの更新
	titleObj->Update();
}

void Skydome::Draw()
{
	// オブジェクトの描画
	titleObj->Draw();
}
