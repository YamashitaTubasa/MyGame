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

	// カメラをセット
	this->camera_ = camera;

	// OBJからモデルデータを読み込む
	playerM = Model::LoadFromOBJ("fighter");
	reticleM = Model::LoadFromOBJ("reticle");
	reticle1M = Model::LoadFromOBJ("reticle1");
	// 3Dオブジェクト生成
	playerO3 = Object3d::Create();
	reticleO3 = Object3d::Create();
	reticle1O3 = Object3d::Create();
	// オブジェクトにモデルをひも付ける
	playerO3->SetModel(playerM);
	reticleO3->SetModel(reticleM);
	reticle1O3->SetModel(reticle1M);
	// カメラセット
	Object3d::SetCamera(camera_);
	// 3Dオブジェクトの位置を指定
	pPosition = { 0,-3,-35 };
	pRotation = { 0,0,0 };
	playerO3->SetPosition(pPosition);
	playerO3->SetScale({ 5, 5, 5 });
	playerO3->SetRotation(pRotation);
	rPosition = { 0,-3,-25 };
	rRotation = { 0,90,0 };
	reticleO3->SetPosition(rPosition);
	reticleO3->SetScale(rScale);
	reticleO3->SetRotation(rRotation);
	r1Position = { 0,-4,-20 };
	r1Rotation = { 0,90,0 };
	reticle1O3->SetPosition(r1Position);
	reticle1O3->SetScale(r1Scale);
	reticle1O3->SetRotation(r1Rotation);
	
	//Object3d::SetEye(pPosition);
	//Object3d::SetTarget(pPosition);
}

void Player::Update()
{
	// 3Dオブジェクト更新
	playerO3->Update();
	reticleO3->Update();
	reticle1O3->Update();

	// デスフラグの立った弾を削除
	pBullets.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;

			return true;
		}
		return false;
	});

	//===== プレイヤーの移動範囲の制限 =====//
	// 左右への移動範囲制限
	if (pPosition.x <= -8) {
		pPosition.x += 0.25f;
		rPosition.x += 0.25f;
		r1Position.x += 0.25f;
		target.x += 0.25f;
		eye.x += 0.25f;
	}
	if(pPosition.x >= 8){
		pPosition.x -= 0.25f;
		rPosition.x -= 0.25f;
		r1Position.x -= 0.25f;
		target.x -= 0.25f;
		eye.x -= 0.25f;
	}
	// 上下への移動範囲制限
	if (pPosition.y <= -6) {
		pPosition.y += 0.25f;
		rPosition.y += 0.25f;
		r1Position.y += 0.25f;
		target.y += 0.25f;
		eye.y += 0.25f;
	}
	if(pPosition.y >= 6){
		pPosition.y -= 0.25f;
		rPosition.y -= 0.25f;
		r1Position.y -= 0.25f;
		target.y -= 0.25f;
		eye.y -= 0.25f;
	}
	//===== プレイヤーの移動処理 =====//
	// 上への移動処理
	if (input->PushKey(DIK_W)) {
		pPosition.y += 0.25f;
		rPosition.y += 0.25f;
		r1Position.y += 0.25f;
		target.y += 0.25f;
		eye.y += 0.25f;
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
		rPosition.x -= 0.25f;
		r1Position.x -= 0.25f;
		target.x -= 0.25f;
		eye.x -= 0.25f;
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
		rPosition.y -= 0.25f;
		r1Position.y -= 0.25f;
		target.y -= 0.25f;
		eye.y -= 0.25f;
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
		rPosition.x += 0.25f;
		r1Position.x += 0.25f;
		target.x += 0.25f;
		eye.x += 0.25f;
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
	reticleO3->SetPosition(rPosition);
	reticleO3->SetRotation(rRotation);
	reticle1O3->SetPosition(r1Position);
	reticle1O3->SetRotation(r1Rotation);
	Object3d::SetTarget(target);
	Object3d::SetEye(eye);

	// 自機のワールド座標から3Dレティクルのワールド座標を計算
	{
		// 自機から3Sレティクルへの距離
		const float kDistancePlayerTo3DReticle = 50.0f;
		// 自機から3Dレティクルへのオフセット(z + 向き)
		DirectX::XMFLOAT3 offset = { 0,0,1.0f };
		// 自機のワールド行列の回転を反映
		//offset = DirectX::XMMatrixAffineTransformation()
	}

	if(input->TriggerKey(DIK_Y)){
		int a = 0;
	}

	// 自キャラの攻撃処理
	Attack();

	// 弾更新
	for (PlayerBullet* bullet : pBullets) {
		bullet->Update();
		pBulletP = bullet->GetPosition();
		if (bullet->GetPosition().z == 10 && bullet->GetPosition().y > -2 && bullet->GetPosition().y < 2 && 
			bullet->GetPosition().x > -2 && bullet->GetPosition().x < 2) {
			hp -= 1;
		}
	}
}

void Player::Draw()
{
	// プレイヤーのモデルの描画
	playerO3->Draw();
	reticleO3->Draw();
	reticle1O3->Draw();

	// 弾描画
	for (PlayerBullet* bullet : pBullets) {
		bullet->Draw();
	}
}

void Player::Attack()
{
	// プレイヤーの攻撃処理
	if (input->TriggerKey(DIK_SPACE) && isTime == false) {
		isTime = true;

		// 弾の速度
		const float bulletSpeed = 1.0f;
		DirectX::XMFLOAT3 velocity(0, 0, bulletSpeed);

		// 弾の生成と、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(pPosition, velocity);

		// 球を登録する
		pBullets.push_back(newBullet);
	}
	
	if (isTime == true) {
		time++;
	}
	if (time >= 100) {
		time = 0;
		isTime = false;
	}
}
