#include "Skydome.h"

Skydome::Skydome()
{
}

Skydome::~Skydome()
{
	// 3Dオブジェクト解放
	delete skydomeO3;
	// 3Dモデル解放
	delete skydomeM;
}

void Skydome::Initialize()
{
	// OBJからモデルデータを読み込む
	skydomeM = Model::LoadFromOBJ("skydome");
	// 3Dオブジェクト生成
	skydomeO3 = Object3d::Create();
	// オブジェクトにモデルをひも付ける
	skydomeO3->SetModel(skydomeM);
	// 3Dオブジェクトの位置を指定
	skydomeO3->SetPosition({ 0,30,0 });
	skydomeO3->SetScale({ 100, 100, 100 });
	skydomeO3->SetRotation({ 0,0,0 });
}

void Skydome::Update()
{
	// 3Dオブジェクト更新
	skydomeO3->Update();
}

void Skydome::Draw()
{
	// 天球の描画
	skydomeO3->Draw();
}
