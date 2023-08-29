#include "Player.h"

#include "SphereCollider.h"

using namespace std;
using namespace DirectX;

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

Player* Player::Create(Model* model)
{
	// 3Dオブジェクトのインスタンスを生成
	Player* instance = new Player();
	if (instance == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!instance->Initialize()) {
		delete instance;
		assert(0);
	}

	// モデルのセット
	if (model) {
		instance->SetModel(model);
	}

	return instance;
}

bool Player::Initialize()
{
	if (!Object3d::Initialize()) {
		return false;
	}

	// コライダーの追加
	float radius = 0.6f;
	// 半径分だけ足元から浮いた座標を球の中心にする
	SetCollider(new SphereCollider(XMVECTOR({ 0, radius, 0, 0 }), radius));

	// カメラをセット
	//this->camera_ = camera;

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
	pPosition = { 0,-2,-55 };
	pRotation = { 0,0,0 };
	playerO3->SetPosition(pPosition);
	playerO3->SetScale({ 5, 5, 5 });
	playerO3->SetRotation(pRotation);
	rPosition = { 0,-2,-45 };
	rRotation = { 0,90,0 };
	reticleO3->SetPosition(rPosition);
	reticleO3->SetScale(rScale);
	reticleO3->SetRotation(rRotation);
	r1Position = { 0,-2.5f,-40 };
	r1Rotation = { 0,90,0 };
	reticle1O3->SetPosition(r1Position);
	reticle1O3->SetScale(r1Scale);
	reticle1O3->SetRotation(r1Rotation);

	Object3d::SetEye(eye);
	Object3d::SetTarget(target);

	return true;
}

void Player::Update()
{
	input = Input::GetInstance();

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

	startCount++;
	if (startCount >= 90) {
		isStartStaging = false;
	}
	if (!isStartStaging) {
		startCount = 0;
		isReticle = true;
	}
	if (isStartStaging) {
		pPosition.z += 0.1f;
		rPosition.z += 0.1f;
		r1Position.z += 0.1f;
		if (eye.z <= -45.0f) {
			eye.z += 0.4f;
		}
		if (eye.y >= 0.0f) {
			eye.y -= 0.4f;
		}
	}
	if (!isStartStaging) {
		if (pPosition.z < 50) {
			pPosition.z += 0.4f;
			rPosition.z += 0.4f;
			r1Position.z += 0.4f;
			target.z += 0.4f;
			eye.z += 0.4f;
		}
	}

	//===== プレイヤーの移動範囲の制限 =====//
	// 左右への移動範囲制限
	if (!isEndStaging) {
		if (pPosition.x <= -8) {
			pPosition.x += 0.3f;
			rPosition.x += 0.3f;
			r1Position.x += 0.3f;
			target.x += 0.3f;
			eye.x += 0.3f;
		}
		if (pPosition.x >= 8) {
			pPosition.x -= 0.3f;
			rPosition.x -= 0.3f;
			r1Position.x -= 0.3f;
			target.x -= 0.3f;
			eye.x -= 0.3f;
		}
		// 上下への移動範囲制限
		if (pPosition.y <= -6) {
			pPosition.y += 0.3f;
			rPosition.y += 0.3f;
			r1Position.y += 0.3f;
			target.y += 0.3f;
			eye.y += 0.3f;
		}
		if (pPosition.y >= 6) {
			pPosition.y -= 0.3f;
			rPosition.y -= 0.3f;
			r1Position.y -= 0.3f;
			target.y -= 0.3f;
			eye.y -= 0.3f;
		}
	}
	//===== プレイヤーの移動処理 =====//
	// 上への移動処理
	if (!isStartStaging && !isEndStaging) {
		if (input->PushKey(DIK_W)) {
			pPosition.y += 0.3f;
			rPosition.y += 0.3f;
			r1Position.y += 0.3f;
			target.y += 0.3f;
			eye.y += 0.3f;
			isUpMove = true;
		}
	}
	if (!input->PushKey(DIK_W)) {
			isUpMove = false;
	}
	
	// 上へ行くときの傾き処理
	if (isUpMove) {
		if (pRotation.x >= -20) {
			pRotation.x -= 1.0f;
		}
	}
	else {
		if (pRotation.x <= 0) {
			pRotation.x += 1.0f;
		}
	}

	// 左への移動処理
	if (!isStartStaging && !isEndStaging) {
		if (input->PushKey(DIK_A)) {
			pPosition.x -= 0.3f;
			rPosition.x -= 0.3f;
			r1Position.x -= 0.3f;
			target.x -= 0.3f;
			eye.x -= 0.3f;
			isLeftMove = true;
		}
	}
	if (!input->PushKey(DIK_A)) {
			isLeftMove = false;
	}
	
	// 左へ行くときの機体の傾き処理
	if (isLeftMove) {
		if (pRotation.z <= 20.0f) {
			pRotation.z += 1.0f;
		}
		if (pRotation.y >= -10.0f) {
			pRotation.y -= 1.0f;
		}
	}
	else {
		if (pRotation.z >= 0.0f) {
			pRotation.z -= 1.0f;
		}
		if (pRotation.y <= 0.0f) {
			pRotation.y += 1.0f;
		}
	}

	// 下への移動処理
	if (!isStartStaging && !isEndStaging) {
		if (input->PushKey(DIK_S)) {
			pPosition.y -= 0.3f;
			rPosition.y -= 0.3f;
			r1Position.y -= 0.3f;
			target.y -= 0.3f;
			eye.y -= 0.3f;
			isDownMove = true;
		}
	}
	if (!input->PushKey(DIK_S)) {
			isDownMove = false;
	}
	
	// 下へ行く時の傾き処理
	if (isDownMove) {
		if (pRotation.x <= 20) {
			pRotation.x += 1.0f;
		}
	}
	else {
		if (pRotation.x >= 0) {
			pRotation.x -= 1.0f;
		}
	}

	// 右への移動処理
	if (!isStartStaging && !isEndStaging) {
		if (input->PushKey(DIK_D)) {
			pPosition.x += 0.3f;
			rPosition.x += 0.3f;
			r1Position.x += 0.3f;
			target.x += 0.3f;
			eye.x += 0.3f;
			isRightMove = true;
		}
	}
	if (!input->PushKey(DIK_D)) {
			isRightMove = false;
	}
	
	// 右へ行くときの傾き処理
	if (isRightMove) {
		if (pRotation.z >= -20.0f) {
			pRotation.z -= 1.0f;
		}
		if (pRotation.y <= 10.0f) {
			pRotation.y += 1.0f;
		}
	}
	else {
		if (pRotation.z <= 0.0f) {
			pRotation.z += 1.0f;
		}
		if (pRotation.y >= 0.0f) {
			pRotation.y -= 1.0f;
		}
	}

	// ブースト時の回転処理
	if (input->TriggerKey(DIK_U) || pPosition.z == 5.09989643f && pPosition.x >= -2 && pPosition.x <= 2 && pPosition.y >= 2 && pPosition.y <= 6) {
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

	if (pPosition.z >= 50) {
		isReticle = false;
		isEndStaging = true;
		pPosition.y += 0.2f;
		pPosition.z += 1.2f;
		rPosition.z += 0.1f;
		r1Position.z += 0.1f;
		target = pPosition;
	}

	if (isEndStaging) {
		isPostE = true;
	}

	playerO3->SetPosition(pPosition);
	playerO3->SetRotation(pRotation);
	reticleO3->SetPosition(rPosition);
	reticleO3->SetRotation(rRotation);
	reticle1O3->SetPosition(r1Position);
	reticle1O3->SetRotation(r1Rotation);
	Object3d::SetTarget(target);
	Object3d::SetEye(eye);

	// 弾の切り替え
	if (input->TriggerKey(DIK_E)) {
		isBullet = 0;
	}

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
	if (isPlayer) {
		playerO3->Draw();
	}

	if (isReticle == true) {
		reticleO3->Draw();
		reticle1O3->Draw();
	}

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

void Player::OnCollision(const CollisionInfo& info)
{
	isPlayer = false;
}
