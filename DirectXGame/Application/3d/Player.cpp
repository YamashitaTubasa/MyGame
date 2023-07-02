#include "Player.h"

using namespace std;

#define PI 3.141592

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

float Player::easeInQuad(float x)
{
	return (float)sin((x * PI) / 2.0);
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

	PlayerBullet::Initialize();
}

void Player::Update()
{
	// 3Dオブジェクト更新
	playerO3->Update();

	//===== プレイヤーの移動処理 =====//;
	// 上への移動処理
	if (input->PushKey(DIK_W)) {
		position[0].y += 0.3f;
		isUpMove = true;
	}else {
		isUpMove = false;
	}
	// 上へ行くときの傾き処理
	if (isUpMove) {
		if (rotation[0].x >= -20) {
			rotation[0].x -= 1.0f;
		}
	}else {
		if (rotation[0].x <= 0) {
			rotation[0].x += 1.0f;
		}
	}
	
	// 左への移動処理
	if(input->PushKey(DIK_A)) {
		position[0].x -= 0.3f;
		isLeftMove = true;
	} else{
		isLeftMove = false;
	}
	// 左へ行くときの機体の傾き処理
	if(isLeftMove) {
		if (rotation[0].z <= 20) {
			rotation[0].z += 1.0f;
		}
	} else{
		if(rotation[0].z >= 0) {
			rotation[0].z -= 1.0f;
		}
	}

	// 下への移動処理
	if(input->PushKey(DIK_S)) {
		position[0].y -= 0.3f;
		isDownMove = true;
	} else{
		isDownMove = false;
	}
	// 下へ行く時の傾き処理
	if (isDownMove) {
		if (rotation[0].x <= 20) {
			rotation[0].x += 1.0f;
		}
	} else{
		if (rotation[0].x >= 0) {
			rotation[0].x -= 1.0f;
		}
	}

	// 右への移動処理
	if(input->PushKey(DIK_D)) {
		position[0].x += 0.3f;
		isRightMove = true;
	} else{
		isRightMove = false;
	}
	// 右へ行くときの傾き処理
	if(isRightMove) {
		if (rotation[0].z >= -20) {
			rotation[0].z -= 1.0f;
		}
	} else{
		if(rotation[0].z <= 0) {
			rotation[0].z += 1.0f;
		}
	}

	playerO3->SetPosition(position[0]);
	playerO3->SetRotation(rotation[0]);

	PlayerBullet::Update();
}

void Player::Draw()
{
	playerO3->Draw();

	PlayerBullet::Draw();
}
