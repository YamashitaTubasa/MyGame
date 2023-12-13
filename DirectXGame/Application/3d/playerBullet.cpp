/**
 * @file PlayerBullet.cpp
 * @brief プレイヤーの弾の挙動に関するクラス
 * @author Yamashita Tubasa
 */

#include "PlayerBullet.h"

PlayerBullet::PlayerBullet() 
{
}

PlayerBullet::~PlayerBullet() 
{
}

void PlayerBullet::Initialize(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& velocity)
{
	// 引数で受け取った速度をメンバ変数に代入
	this->velocity__ = velocity;
	
	// OBJからモデルデータを読み込む
	bulletM_ = Model::LoadFromOBJ("bullet");
	// 3Dオブジェクト生成
	bulletO3_ = Object3d::Create();
	// オブジェクトにモデルをひも付ける
	bulletO3_->SetModel(bulletM_.get());
	// 3Dオブジェクトの位置を指定
	pBulletPos_ = position;
	bulletO3_->SetPosition(pBulletPos_);
	bulletO3_->SetScale({ 3,3,3 });
	bulletO3_->SetRotation({ 0, 180, 0 });
}

void PlayerBullet::Update()
{
	// 3Dオブジェクト更新
	bulletO3_->Update();

	// 時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	// 弾に速度追加
	pBulletPos_.z += velocity__.z;

	// 弾の座標にセット
	bulletO3_->SetPosition(pBulletPos_);
}

void PlayerBullet::Draw()
{
	// 弾の描画
	bulletO3_->Draw();
}