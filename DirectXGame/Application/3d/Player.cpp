#include "Player.h"

Player::Player()
{
}

Player::~Player()
{
	// 3Dオブジェクト解放
	delete playerO3;
	// 3Dモデル解放
	delete playerM;
}

void Player::Initialize()
{
	input = Input::GetInstance();

	// OBJからモデルデータを読み込む
	playerM = Model::LoadFromOBJ("fighter");
	// 3Dオブジェクト生成
	playerO3 = Object3d::Create();
	// オブジェクトにモデルをひも付ける
	playerO3->SetModel(playerM);
	// 3Dオブジェクトの位置を指定
	position[0] = { 0,-3,-35 };
	rotation[0] = { 0,0,0 };
	playerO3->SetPosition(position[0]);
	playerO3->SetScale({ 5, 5, 5 });
	playerO3->SetRotation(rotation[0]);
	//object3d[0]->SetEye(eye[0]);
}

void Player::Update()
{
	// 3Dオブジェクト更新
	playerO3->Update();

	playerO3->SetPosition(position[0]);
	playerO3->SetRotation(rotation[0]);

	if (input->PushKey(DIK_W)) {
		position[0].y += 0.3f;
	}
	if (input->PushKey(DIK_A)) {
		position[0].x -= 0.3f;
	}
	if (input->PushKey(DIK_S)) {
		position[0].y -= 0.3f;
	}
	if (input->PushKey(DIK_D)) {
		position[0].x += 0.3f;
	}
}

void Player::Draw()
{
	playerO3->Draw();
}
