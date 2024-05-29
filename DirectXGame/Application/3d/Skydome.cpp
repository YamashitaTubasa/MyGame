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
}

void Skydome::Initialize()
{
	// OBJからモデルデータを読み込む
	skydomeModel_ = Model::LoadFromOBJ("skydome");
	// 3Dオブジェクトの生成
	skydomeObj_ = Object3d::Create();
	// 3Dオブジェクトにモデルをセット
	skydomeObj_->SetModel(skydomeModel_.get());
	// オブジェクトの位置の設定
	skydomeObj_->SetPosition({ 0,30,0 });
	skydomeObj_->SetScale({ 900,900,900 });
	skydomeObj_->SetRotation(rotate_);
}

void Skydome::Update()
{
	// オブジェクトの更新
	skydomeObj_->Update();
}

void Skydome::Draw()
{
	// オブジェクトの描画
	skydomeObj_->Draw();
}

void Skydome::RotateSky()
{
	// 天球の回転
	rotate_.y += skyRotSpeed_;
	skydomeObj_->SetRotation(rotate_);
}

void Skydome::Move()
{
	// 天球の移動
	move_.z += skySpeed_;
	skydomeObj_->SetPosition(move_);
}
