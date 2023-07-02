#include "PlayerBullet.h"

PlayerBullet::PlayerBullet() 
{
}

PlayerBullet::~PlayerBullet() 
{
	// 3Dオブジェクト解放
	delete bulletO3;
	// 3Dモデル解放
	delete bulletM;
}

void PlayerBullet::Initialize()
{
	// OBJからモデルデータを読み込む
	bulletM = Model::LoadFromOBJ("bullet");
	// 3Dオブジェクト生成
	bulletO3 = Object3d::Create();
	// オブジェクトにモデルをひも付ける
	bulletO3->SetModel(bulletM);
	// 3Dオブジェクトの位置を指定
	position[1] = { 0,0,50 };
	bulletO3->SetPosition(position[1]);
	bulletO3->SetScale({ 5,5,5 });
	bulletO3->SetRotation({ 0, 0, 0 });
}

void PlayerBullet::Update()
{
	// 3Dオブジェクト更新
	bulletO3->Update();

	position[1].z -= 1;
	if (position[1].z < -100) {
		position[1].z = 50;
	}

	bulletO3->SetPosition(position[1]);
}

void PlayerBullet::Draw()
{
	bulletO3->Draw();
}
