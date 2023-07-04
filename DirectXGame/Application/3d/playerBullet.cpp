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

void PlayerBullet::Initialize(DirectX::XMFLOAT3 position, const DirectX::XMFLOAT3 velocity)
{
	
	// OBJからモデルデータを読み込む
	bulletM = Model::LoadFromOBJ("bullet");
	// 3Dオブジェクト生成
	bulletO3 = Object3d::Create();
	// オブジェクトにモデルをひも付ける
	bulletO3->SetModel(bulletM);
	// 3Dオブジェクトの位置を指定
	pBulletPos = position;
	bulletO3->SetPosition(pBulletPos);
	bulletO3->SetScale({ 1,1,1 });
	bulletO3->SetRotation({ 0, 180, 0 });
}

void PlayerBullet::Update()
{
	// 3Dオブジェクト更新
	bulletO3->Update();

	bulletO3->SetPosition(pBulletPos);
}

void PlayerBullet::Draw()
{
	bulletO3->Draw();
}
