#include "EnemyBullet.h"

void EnemyBullet::Initialize(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& velocity)
{
	this->velocity_ = velocity;

	// モデルの生成
	bulletModel_ = Model::LoadFromOBJ("bullet");
	bulletObject_ = Object3d::Create();
	bulletObject_->SetModel(bulletModel_.get());
	// 座標の設定
	position_ = position;
	bulletObject_->SetPosition(position_);
	bulletObject_->SetScale(scale_);
	bulletObject_->SetRotation(rotate_);
}

void EnemyBullet::Update()
{
	// 敵の弾の更新
	bulletObject_->Update();

	// 時間経過でデス
	if (--dethTimer_ <= 0) {
		isDead_ = true;
	}

	// 弾の速度
	position_.z -= velocity_.z;
	bulletObject_->SetPosition(position_);
}

void EnemyBullet::Draw()
{
	// 敵の弾の描画
	bulletObject_->Draw();
}
