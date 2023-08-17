#include "Enemy.h"

#include "CollisionManager.h"
#include "SphereCollider.h"

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

Enemy* Enemy::Create(Model* model)
{
	// 3Dオブジェクトのインスタンス生成
	Enemy* instance = new Enemy();
	if (instance == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!instance->Initialize()) {
		delete instance;
		assert(0);
	}

	if (model) {
		instance->SetModel(model);
	}

	return instance;
}

bool Enemy::Initialize()
{
	if (!Object3d::Initialize()) 
	{
		return false;
	}

	// OBJからモデルデータを読み込む
	enemyM = Model::LoadFromOBJ("enemy");
	// 3Dオブジェクト生成
	enemyO3 = Object3d::Create();
	// オブジェクトにモデルをひも付ける
	enemyO3->SetModel(enemyM);
	enemyO3->SetCollider(new SphereCollider);
	// 3Dオブジェクトの位置を指定
	position[0] = { 0,0,0 };
	rotation[0] = { 0,180,0 };
	enemyO3->SetPosition(position[0]);
	enemyO3->SetScale({ 5, 5, 5 });
	enemyO3->SetRotation(rotation[0]);

	return true;
}

void Enemy::Update()
{
	enemyO3->Update();
}

void Enemy::Draw()
{
	enemyO3->Draw();
}
