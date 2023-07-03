#include "Enemy.h"

Enemy::Enemy()
{
}

Enemy::~Enemy()
{
	// 3Dオブジェクト解放
	delete enemyO3;
	// 3Dモデル解放
	delete enemyM;
}

void Enemy::Initialize()
{
	// OBJからモデルデータを読み込む
	enemyM = Model::LoadFromOBJ("enemy");
	// 3Dオブジェクト生成
	enemyO3 = Object3d::Create();
	// オブジェクトにモデルをひも付ける
	enemyO3->SetModel(enemyM);
	// 3Dオブジェクトの位置を指定
	position[0] = { 0,0,-20 };
	rotation[0] = { 0,180,0 };
	enemyO3->SetPosition(position[0]);
	enemyO3->SetScale({ 5, 5, 5 });
	enemyO3->SetRotation(rotation[0]);
}

void Enemy::Update()
{
	enemyO3->Update();
}

void Enemy::Draw()
{
	enemyO3->Draw();
}
