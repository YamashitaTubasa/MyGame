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

void PlayerBullet::Initialize(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& velocity)
{
	// 引数で受け取った速度をメンバ変数に代入
	this->velocity_ = velocity;
	
	// OBJからモデルデータを読み込む
	bulletM = Model::LoadFromOBJ("bullet");
	// 3Dオブジェクト生成
	bulletO3 = Object3d::Create();
	// オブジェクトにモデルをひも付ける
	bulletO3->SetModel(bulletM);
	// 3Dオブジェクトの位置を指定
	pBulletPos = position;
	bulletO3->SetPosition(pBulletPos);
	bulletO3->SetScale({ 3,3,3 });
	bulletO3->SetRotation({ 0, 180, 0 });
}

void PlayerBullet::Update()
{
	// 3Dオブジェクト更新
	bulletO3->Update();

	// 時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	// 弾に速度追加
	pBulletPos.z += velocity_.z;

	// 弾の座標にセット
	bulletO3->SetPosition(pBulletPos);
}

void PlayerBullet::Draw()
{
	// 弾の描画
	bulletO3->Draw();
}