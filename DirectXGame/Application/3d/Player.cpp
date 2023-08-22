#include "Player.h"

using namespace std;

/// <summary>
/// 静的メンバ変数の実体
/// </summary>
Camera* Player::camera_ = nullptr;

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

float Player::EaseOutQuintP(float t, float b, float c, float d)
{
	float x = t / d;
	float v = EaseOutQuint(x);
	float result = c * v + b;

	return result;
}

float Player::EaseOutQuint(float x)
{
	return static_cast<float>(1 - pow(1 - x, 5));
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
		pPosition.y += speed;
		isUpMove = true;
	}else {
		isUpMove = false;
	}
	// 上へ行くときの傾き処理
	if (isUpMove) {
		if (pRotation.x >= -pRotMax) {
			pRotation.x -= rotSpeed;
		}
	}else {
		if (pRotation.x <= pRotMin) {
			pRotation.x += rotSpeed;
		}
	}
	
	// 左への移動処理
	if(input->PushKey(DIK_A)) {
		pPosition.x -= speed;
		isLeftMove = true;
	} else{
		isLeftMove = false;
	}
	// 左へ行くときの機体の傾き処理
	if(isLeftMove) {
		if (pRotation.z <= pRotMax) {
			pRotation.z += rotSpeed;
		}
	} else{
		if(pRotation.z >= pRotMin) {
			pRotation.z -= rotSpeed;
		}
	}

	// 下への移動処理
	if(input->PushKey(DIK_S)) {
		pPosition.y -= speed;
		isDownMove = true;
	} else{
		isDownMove = false;
	}
	// 下へ行く時の傾き処理
	if (isDownMove) {
		if (pRotation.x <= pRotMax) {
			pRotation.x += rotSpeed;
		}
	} else{
		if (pRotation.x >= pRotMin) {
			pRotation.x -= rotSpeed;
		}
	}

	// 右への移動処理
	if(input->PushKey(DIK_D)) {
		pPosition.x += speed;
		isRightMove = true;
	} else{
		isRightMove = false;
	}
	// 右へ行くときの傾き処理
	if(isRightMove) {
		if (pRotation.z >= -pRotMax) {
			pRotation.z -= rotSpeed;
		}
	} else{
		if(pRotation.z <= pRotMin) {
			pRotation.z += rotSpeed;
		}
	}

	// ブースト時の回転処理
	if (input->TriggerKey(DIK_U)) {
		isEaseFlag = true;
		isRot = true;
		eFrame = 0;
	}
	if (isRot) {
		if (isEaseFlag) {
			eFrame++;
		}

		// 自機の回転
		pRotation.z = EasingManager::EaseOutQuintP(eFrame, 0, 360, 60);

		// 回転最大時の値の初期化
		if (pRotation.z == 360.0f) {
			pRotation.z = 0.0f;
			isRot = false;
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

void Player::Draw(Camera* camera)
{
	playerO3->Draw(camera);

	// 弾描画
	for (PlayerBullet* bullet : pBullets) {
		bullet->Draw(camera);
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
		newBullet->Initialize(camera_, pPosition, velocity);

		// 球を登録する
		pBullets.push_back(newBullet);
	}
}

void Player::OnCollision(const CollisionInfo& info)
{
	isPlayer = false;
}