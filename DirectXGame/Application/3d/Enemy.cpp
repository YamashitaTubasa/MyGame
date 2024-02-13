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
	state_ = new EnemyStateAccess();
}

Enemy::~Enemy()
{
	delete state_;
}

//std::unique_ptr<Enemy> Enemy::Create(Model* model)
//{
//	// 3Dオブジェクトのインスタンス生成
//	std::unique_ptr<Enemy> instance = std::make_unique<Enemy>();
//	if (instance == nullptr) {
//		return nullptr;
//	}
//
//	// 初期化
//	if (!instance->Initialize()) {
//		assert(0);
//	}
//
//	// モデルをセット
//	if (model) {
//		instance->SetModel(model);
//	}
//
//	return instance;
//}

void Enemy::Initialize(const DirectX::XMFLOAT3& position)
{
	/*if (!Object3d::Initialize()) 
	{
		return false;
	}*/

	// OBJからモデルデータを読み込む
	enemyModel_ = Model::LoadFromOBJ("enemy");
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
	//// 敵の行動
	//switch (phase_) {
	//default:
	//case Phase::Access:

	//	break;
	//}

	//// 敵の更新
	//enemyObject_->Update();
	state_->Update(this);
}

void Enemy::Draw()
{
	// 敵の描画
	enemyObject_->Draw();
}

void Enemy::Fire()
{
	// 敵の攻撃処理
	const float bulletSpeed = 1.0f;
	DirectX::XMFLOAT3 velocity(0, 0, bulletSpeed);

	// 弾の生成と、初期化
	std::unique_ptr<EnemyBullet> newEnemyBullets = std::make_unique<EnemyBullet>();
	newEnemyBullets->Initialize(position_, velocity);

	// 球を登録する
	gamePlayScene_->AddEnemyBullet(std::move(newEnemyBullets));
}

void Enemy::ChangeState(BaseEnemyState* newState)
{
	delete state_;
	state_ = newState;
}

void EnemyStateAccess::Update(Enemy* enemy)
{
	enemy->GetPosition();

	position_.x += 0.1f;
	enemy->SetPosition(position_);

	enemy->ChangeState(new EnemyStateAccess());
}

void EnemyStateFire::Update(Enemy* enemy)
{
	enemy->GetPosition();

	position_.x -= 0.1f;
	enemy->SetPosition(position_);

	enemy->ChangeState(new EnemyStateAccess());
}