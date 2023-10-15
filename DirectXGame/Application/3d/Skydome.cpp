/**
 * @file Skydome.cpp
 * @brief 天球に関するクラス
 * @author Yamashita Tubasa
 */

#include "Skydome.h"

Skydome::Skydome()
{
}

Skydome::~Skydome()
{
	delete skydomeObj;
	delete skydomeModel;
}

void Skydome::Initialize()
{
	// OBJからモデルデータを読み込む
	skydomeModel = Model::LoadFromOBJ("skydome");
	// 3Dオブジェクトの生成
	skydomeObj = Object3d::Create();
	// 3Dオブジェクトにモデルをセット
	skydomeObj->SetModel(skydomeModel);
	// オブジェクトの位置の設定
	skydomeObj->SetPosition({ 0,30,0 });
	skydomeObj->SetScale({ 300,300,300 });
	skydomeObj->SetRotation(rotate);
}

void Skydome::Update()
{
	// オブジェクトの更新
	skydomeObj->Update();
}

void Skydome::Draw()
{
	// オブジェクトの描画
	skydomeObj->Draw();
}

void Skydome::RotateSky()
{
	// 天球の回転
	rotate.y += skySpeed;
	skydomeObj->SetRotation(rotate);
}
