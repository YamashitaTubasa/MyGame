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
	// 弾の解放
	for (PlayerBullet* bullet : pBullets) {
		delete bullet;
	}
}

float Player::easeInQuad(float x)
{
	return (float)sin((x * PI) / 2.0);
}

void Player::Initialize(Camera* camera)
{
	input = Input::GetInstance();

	this->camera_ = camera;

	// OBJからモデルデータを読み込む
	playerM = Model::LoadFromOBJ("fighter");
	// 3Dオブジェクト生成
	playerO3 = Object3d::Create();
	// オブジェクトにモデルをひも付ける
	playerO3->SetModel(playerM);
	// カメラセット
	Object3d::SetCamera(camera_);
	// 3Dオブジェクトの位置を指定
	pPosition = { 0,-3,-35 };
	pRotation = { 0,0,0 };
	playerO3->SetPosition(pPosition);
	playerO3->SetScale({ 5, 5, 5 });
	playerO3->SetRotation(pRotation);
	//object3d[0]->SetEye(eye[0]);
}

void Player::Update()
{
	// 3Dオブジェクト更新
	playerO3->Update();

	//===== プレイヤーの移動処理 =====//
	// 上への移動処理
	if (input->PushKey(DIK_W)) {
		pPosition.y += 0.25f;
		isUpMove = true;
	}else {
		isUpMove = false;
	}
	// 上へ行くときの傾き処理
	if (isUpMove) {
		if (pRotation.x >= -20) {
			pRotation.x -= 1.0f;
		}
	}else {
		if (pRotation.x <= 0) {
			pRotation.x += 1.0f;
		}
	}
	
	// 左への移動処理
	if(input->PushKey(DIK_A)) {
		pPosition.x -= 0.25f;
		isLeftMove = true;
	} else{
		isLeftMove = false;
	}
	// 左へ行くときの機体の傾き処理
	if(isLeftMove) {
		if (pRotation.z <= 20) {
			pRotation.z += 1.0f;
		}
	} else{
		if(pRotation.z >= 0) {
			pRotation.z -= 1.0f;
		}
	}

	// 下への移動処理
	if(input->PushKey(DIK_S)) {
		pPosition.y -= 0.25f;
		isDownMove = true;
	} else{
		isDownMove = false;
	}
	// 下へ行く時の傾き処理
	if (isDownMove) {
		if (pRotation.x <= 20) {
			pRotation.x += 1.0f;
		}
	} else{
		if (pRotation.x >= 0) {
			pRotation.x -= 1.0f;
		}
	}

	// 右への移動処理
	if(input->PushKey(DIK_D)) {
		pPosition.x += 0.25f;
		isRightMove = true;
	} else{
		isRightMove = false;
	}
	// 右へ行くときの傾き処理
	if(isRightMove) {
		if (pRotation.z >= -20) {
			pRotation.z -= 1.0f;
		}
	} else{
		if(pRotation.z <= 0) {
			pRotation.z += 1.0f;
		}
	}

	playerO3->SetPosition(pPosition);
	playerO3->SetRotation(pRotation);

	// 自キャラの攻撃処理
	Attack();

	// 弾更新
	for (PlayerBullet* bullet : pBullets) {
		bullet->Update();
	}
}

void Player::Draw()
{
	playerO3->Draw();

	// 弾描画
	for (PlayerBullet* bullet : pBullets) {
		bullet->Draw();
	}
}

void Player::Attack()
{
	if (input->TriggerKey(DIK_SPACE)) {

		// 弾の速度
		const float bulletSpeed = 1.0f;
		Vector3 velocity(0, 0, bulletSpeed);

		// 弾の生成と、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(pPosition, velocity);

		// 球を登録する
		pBullets.push_back(newBullet);
	}
}
