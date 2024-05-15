/**
 * @file Enemy.cpp
 * @brief 敵の挙動に関するクラス
 * @author Yamashita Tubasa
 */

#include "Enemy.h"

#include "CollisionManager.h"
#include "SphereCollider.h"
#include "Input.h"
#include "GamePlayScene.h"

#include <fstream>

Enemy::Enemy()
{
}

Enemy::~Enemy()
{
}

void Enemy::Initialize(const DirectX::XMFLOAT3& position)
{
	/*if (!Object3d::Initialize()) 
	{
		return false;
	}*/

	// OBJからモデルデータを読み込む
	enemyModel_ = Model::LoadFromOBJ("mobEnemy");
	// 3Dオブジェクト生成
	enemyObject_ = Object3d::Create();
	// オブジェクトにモデルをひも付ける
	enemyObject_->SetModel(enemyModel_.get());
	enemyObject_->SetCollider(new SphereCollider);
	// 3Dオブジェクトの位置を指定
	position_ = position;
	enemyObject_->SetPosition(position_);
	enemyObject_->SetScale(scale_);
	enemyObject_->SetRotation(rotation_);
}

void Enemy::Update()
{
	// 敵の行動
	/*switch (phase_) {
	default:
	case Phase::Access:

		break;
	}*/

	// 敵の更新
	enemyObject_->Update();
}

void Enemy::Draw()
{
	// 敵の描画
	enemyObject_->Draw();
}

void Enemy::Fire()
{
	// 敵の攻撃処理
	const float bulletSpeed = 1.5f;
	DirectX::XMFLOAT3 velocity(0, 0, bulletSpeed);

	// 弾の発射間隔
	if (--fireTime_ >= 0) {
		if (--bulletTime_ <= 0) {

			// 弾の生成と、初期化
			std::unique_ptr<EnemyBullet> newEnemyBullets = std::make_unique<EnemyBullet>();
			newEnemyBullets->Initialize(position_, velocity);

			// 球を登録する
			gamePlayScene_->AddEnemyBullet(std::move(newEnemyBullets));
			bulletTime_ = bulletInterval_;
		}
	}
	else if (fireTime_ <= -fireInterval_) {
		fireTime_ = fireInterval_;
	}
}

void Enemy::OnCollision()
{
	isDead_ = true;
}
