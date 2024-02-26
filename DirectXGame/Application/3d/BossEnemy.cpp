/**
 * @file BossEnemy.cpp
 * @brief ボス敵クラス
 * @author Yamashita Tubasa
 */

#include "BossEnemy.h"

void BossEnemy::Initialize()
{
	// OBJからモデルデータを読み込む
	bossModel_ = Model::LoadFromOBJ("boss");
	// 3Dオブジェクト生成
	bossObject_ = Object3d::Create();
	// オブジェクトにモデルをひも付ける
	bossObject_->SetModel(bossModel_.get());
	// 3Dオブジェクトの位置を指定
	bossObject_->SetPosition(position_);
	bossObject_->SetScale(scale_);
	bossObject_->SetRotation(rotation_);
}

void BossEnemy::Update()
{
	// オブジェクト更新
	bossObject_->Update();
}

void BossEnemy::Draw()
{
	// 更新
	bossObject_->Draw();
}

void BossEnemy::OnCollision()
{
	isDead_ = true;
}
