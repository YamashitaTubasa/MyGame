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

void BossEnemy::Move()
{
	position_.z += speed_;
	if (isMove_ == Move::RightMove) {
		position_.x += xSpeed_;
	}
	if (position_.x >= screenMax_) {
		position_.x = screenMax_;
		isMoveTime_++;
		if (isMoveTime_ >= Move::MoveMaxTime) {
			isMove_ = Move::LeftMove;
			isMoveTime_ = Move::MoveInitializeTime;
		}
	}
	if (isMove_ == Move::LeftMove) {
		position_.x -= xSpeed_;
	}
	if (position_.x <= -screenMax_) {
		position_.x = -screenMax_;
		isMoveTime_++;
		if (isMoveTime_ >= Move::MoveMaxTime) {
			isMove_ = Move::RightMove;
			isMoveTime_ = Move::MoveInitializeTime;
		}
	}
	bossObject_->SetPosition(position_);
}
