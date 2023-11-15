/**
 * @file Player.cpp
 * @brief プレイヤーの挙動に関するクラス
 * @author Yamashita Tubasa
 */

#include "Player.h"

#include "SphereCollider.h"
#include "GameSceneManager.h"

using namespace std;
using namespace DirectX;

Player::Player()
{
}

Player::~Player()
{
	delete playerO3_;
	delete reticleO3_;
	delete reticle1O3_;
	delete playerM_;
	delete reticleM_;
	delete reticle1M_;
	delete particle_;
	delete particleMan_;
	delete blackSmokeMan_;
	delete blackSmoke_;
	// 弾の解放
	for (PlayerBullet* bullet : pBullets_) {
		delete bullet;
	}
	delete enemy_;
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
	dxCommon_ = DirectXCommon::GetInstance();

	if (!Object3d::Initialize()) {
		return false;
	}

	enemy_ = new Enemy();

	// コライダーの追加
	float radius = 0.6f;
	// 半径分だけ足元から浮いた座標を球の中心にする
	SetCollider(new SphereCollider(XMVECTOR({ 0, radius, 0, 0 }), radius));

	// カメラをセット
	//this->camera_ = camera;

	// OBJからモデルデータを読み込む
	playerM_ = Model::LoadFromOBJ("fighter");
	reticleM_ = Model::LoadFromOBJ("reticle");
	reticle1M_ = Model::LoadFromOBJ("reticle1");
	// 3Dオブジェクト生成
	playerO3_ = Object3d::Create();
	reticleO3_ = Object3d::Create();
	reticle1O3_ = Object3d::Create();
	// オブジェクトにモデルをひも付ける
	playerO3_->SetModel(playerM_);
	reticleO3_->SetModel(reticleM_);
	reticle1O3_->SetModel(reticle1M_);
	// カメラセット
	Object3d::SetCamera(camera_);
	// 3Dオブジェクトの位置を指定
	playerO3_->SetPosition(pPosition_);
	playerO3_->SetScale(pScale_);
	playerO3_->SetRotation(pRotation_);
	reticleO3_->SetPosition(rPosition_);
	reticleO3_->SetScale(rScale_);
	reticleO3_->SetRotation(rRotation_);
	reticle1O3_->SetPosition(r1Position_);
	reticle1O3_->SetScale(r1Scale_);
	reticle1O3_->SetRotation(r1Rotation_);

	Object3d::SetEye(eye_);
	target_ = pPosition_;
	Object3d::SetTarget(target_);

	// OBJの名前を指定してモデルデータを読み込む
	particle_ = Particle::LoadFromOBJ("bomb.png");
	blackSmoke_ = Particle::LoadFromOBJ("bombEffect.png");
	// パーティクルの生成
	particleMan_ = ParticleManager::Create();
	blackSmokeMan_ = ParticleManager::Create();
	// パーティクルマネージャーにパーティクルを割り当てる
	particleMan_->SetModel(particle_);
	blackSmokeMan_->SetModel(blackSmoke_);

	return true;
}

void Player::Update()
{
	input_ = Input::GetInstance();

	//if (pPosition_.z >= -40) {
	//	// ゲームプレイシーン（次シーン）を生成
	//	GameSceneManager::GetInstance()->ChangeScene("TITLE");
	//}

	// 3Dオブジェクト更新
	playerO3_->Update();
	reticleO3_->Update();
	reticle1O3_->Update();

	// パーティクルの更新
	particleMan_->Update();
	blackSmokeMan_->Update();

	// デスフラグの立った敵を削除
	enemys_.remove_if([](std::unique_ptr<Enemy>& enemy_) {
		return enemy_->GetIsDead();
	});

	// デスフラグの立った弾を削除
	pBullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;

			return true;
		}
		return false;
	});

	// パーティクルの実行
	particleMan_->Execution(particle_, particleBombPosX_, particleBombPosY_, particleBombPosZ_, particleLife, particleStartScale_, particleEndScale_);

	if (hp_ == -20) {

		isGameOverStaging_ = true;
	}

	if (isGameOverStaging_) {

		pPosition_.y -= playerFall_;
		playerO3_->SetPosition(pPosition_);

		// パーティクルの実行
		for (int i = 0; i < fireParticleMax_; i++) {
			// X,Y,Zすべて[-5.0f,+5.0f]でランダムに分布
			const float md_pos = 0.25f;
			const float md = 2.0f;
			DirectX::XMFLOAT3 pos{};
			pos.x = (float)rand() / RAND_MAX * md_pos - md_pos / md;
			pos.y = (float)rand() / RAND_MAX * md_pos - md_pos / md;
			pos.z = (float)rand() / RAND_MAX * md_pos - md_pos / md;
			// X,Y,Z全て[-0.05f,+0.05f]でランダム分布
			const float md_vel_x = 0.01f;
			const float md_vel_y = 0.05f;
			const float md_vel_z = 0.05f;
			DirectX::XMFLOAT3 vel{};
			vel.x = (float)rand() / RAND_MAX * md_vel_x - md_vel_x / md;
			vel.y = (float)rand() / RAND_MAX * md_vel_y - md_vel_y / md;
			vel.z = (float)rand() / RAND_MAX * md_vel_z - md_vel_z / md;
			// 重力に見立ててYのみ[-0.001f,0]でランダム分布
			DirectX::XMFLOAT3 acc{};
			const float md_acc_x = 0.00f;
			const float md_acc_y = 0.04f;
			acc.x = (float)rand() / RAND_MAX * md_acc_x;
			acc.y = (float)rand() / RAND_MAX * md_acc_y;

			// 追加
			blackSmoke_->Add(fireParticleLife, pos, vel, acc, fireParticleStartScale_, fireParticleEndScale_);
		}
	}

	if (isGameClearStaging_) {
		// パーティクルの実行
		//for (int i = 0; i < 2; i++) {
		//	// X,Y,Zすべて[-5.0f,+5.0f]でランダムに分布
		//	const float md_pos = 0.3f;
		//	const float md = 2.0f;
		//	DirectX::XMFLOAT3 pos{};
		//	pos.x = (float)rand() / RAND_MAX * md_pos - md_pos / md * -2.0f;
		//	pos.y = (float)rand() / RAND_MAX * md_pos - md_pos / md * 0.0f;
		//	pos.z = (float)rand() / RAND_MAX * md_pos - md_pos / md * 0.0f;
		//	// X,Y,Z全て[-0.05f,+0.05f]でランダム分布
		//	const float md_vel_x = 0.05f;
		//	const float md_vel_y = 0.05f;
		//	const float md_vel_z = 0.05f;
		//	DirectX::XMFLOAT3 vel{};
		//	vel.x = (float)rand() / RAND_MAX * md_vel_x - md_vel_x / md;
		//	vel.y = (float)rand() / RAND_MAX * md_vel_y - md_vel_y / md;
		//	vel.z = (float)rand() / RAND_MAX * md_vel_z - md_vel_z / md;
		//	// 重力に見立ててYのみ[-0.001f,0]でランダム分布
		//	DirectX::XMFLOAT3 acc{};
		//	const float md_acc_x = 0.04f;
		//	const float md_acc_y = 0.04f;
		//	acc.x = (float)rand() / RAND_MAX * md_acc_x - md_acc_x / 2.0f;
		//	acc.y = (float)rand() / RAND_MAX * md_acc_y - md_acc_y / 2.0f;

		//	// 追加
		//	blackSmoke_->Add(fireParticleLife, pos, vel, acc, fireParticleStartScale_, fireParticleEndScale_);
		//}
	}

	startCount_++;
	if (pPosition_.z >= -60) {
		isStartStaging_ = false;
	}
	if (!isStartStaging_) {
		startCount_ = 0;
		isReticle_ = true;
	}
	if (isStartStaging_) {
		if (pPosition_.z <= -100) {
			pPosition_.z += startSpeed_;
			rPosition_.z += startSpeed_;
			r1Position_.z += startSpeed_;
		}
		if (pPosition_.z >= -100) {
			pPosition_.z += 1.0f;
			rPosition_.z += 1.0f;
			r1Position_.z += 1.0f;
		}
		if (eye_.z <= -45.0f) {
		}
			eye_.z += 0.3f;
	}
	if (!isStartStaging_) {
		if (pPosition_.z < 100) {
			pPosition_.z += startSpeed_;
			rPosition_.z += startSpeed_;
			r1Position_.z += startSpeed_;
			eye_.z += startSpeed_;
			target_ = pPosition_;
		}
	}

	//===== プレイヤーの移動範囲の制限 =====//
	// 左右への移動範囲制限
	if (isEndStaging_ == false && isGameOverStaging_ == false) {
		if (pPosition_.x <= -8) {
			pPosition_.x += playerSpeed_;
			rPosition_.x += playerSpeed_;
			r1Position_.x += playerSpeed_;
			target_.x += playerSpeed_;
			eye_.x += playerSpeed_;
		}
		if (pPosition_.x >= 8) {
			pPosition_.x -= playerSpeed_;
			rPosition_.x -= playerSpeed_;
			r1Position_.x -= playerSpeed_;
			target_.x -= playerSpeed_;
			eye_.x -= playerSpeed_;
		}
		// 上下への移動範囲制限
		if (pPosition_.y <= -6) {
			pPosition_.y += playerSpeed_;
			rPosition_.y += playerSpeed_;
			r1Position_.y += playerSpeed_;
			target_.y += playerSpeed_;
			eye_.y += playerSpeed_;
		}
		if (pPosition_.y >= 6) {
			pPosition_.y -= playerSpeed_;
			rPosition_.y -= playerSpeed_;
			r1Position_.y -= playerSpeed_;
			target_.y -= playerSpeed_;
			eye_.y -= playerSpeed_;
		}
	}
	//===== プレイヤーの移動処理 =====//
	// 上への移動処理
	if (!isStartStaging_ && !isEndStaging_ && !isGameOverStaging_) {
		if (input_->PushKey(DIK_W)) {
			pPosition_.y += playerSpeed_;
			rPosition_.y += playerSpeed_;
			r1Position_.y += playerSpeed_;
			target_.y += playerSpeed_;
			eye_.y += playerSpeed_;
			isUpMove_ = true;
		}
	}
	if (!input_->PushKey(DIK_W)) {
			isUpMove_ = false;
	}
	
	// 上へ行くときの傾き処理
	if (isUpMove_) {
		if (pRotation_.x >= playerSlopeMax_) {
			pRotation_.x -= playerRotSpeed_;
		}
	}
	else {
		if (pRotation_.x <= playerSlopeMin_) {
			pRotation_.x += playerRotSpeed_;
		}
	}

	// 左への移動処理
	if (!isStartStaging_ && !isEndStaging_ && !isGameOverStaging_) {
		if (input_->PushKey(DIK_A)) {
			pPosition_.x -= playerSpeed_;
			rPosition_.x -= playerSpeed_;
			r1Position_.x -= playerSpeed_;
			target_.x -= playerSpeed_;
			eye_.x -= playerSpeed_;
			isLeftMove_ = true;
		}
	}
	if (!input_->PushKey(DIK_A)) {
			isLeftMove_ = false;
	}
	
	// 左へ行くときの機体の傾き処理
	if (isLeftMove_) {
		if (pRotation_.z <= playerSlopeMaxPlus_) {
			pRotation_.z += playerRotSpeed_;
		}
		if (pRotation_.y >= playerSlopeMaxY_) {
			pRotation_.y -= playerRotSpeed_;
		}
	}
	else {
		if (pRotation_.z >= playerSlopeMin_) {
			pRotation_.z -= playerRotSpeed_;
		}
		if (pRotation_.y <= playerSlopeMin_) {
			pRotation_.y += playerRotSpeed_;
		}
	}

	// 下への移動処理
	if (!isStartStaging_ && !isEndStaging_ && !isGameOverStaging_) {
		if (input_->PushKey(DIK_S)) {
			pPosition_.y -= playerSpeed_;
			rPosition_.y -= playerSpeed_;
			r1Position_.y -= playerSpeed_;
			target_.y -= playerSpeed_;
			eye_.y -= playerSpeed_;
			isDownMove_ = true;
		}
	}
	if (!input_->PushKey(DIK_S)) {
			isDownMove_ = false;
	}
	
	// 下へ行く時の傾き処理
	if (isDownMove_) {
		if (pRotation_.x <= playerSlopeMaxPlus_) {
			pRotation_.x += playerRotSpeed_;
		}
	}
	else {
		if (pRotation_.x >= playerSlopeMin_) {
			pRotation_.x -= playerRotSpeed_;
		}
	}

	// 右への移動処理
	if (!isStartStaging_ && !isEndStaging_ && !isGameOverStaging_) {
		if (input_->PushKey(DIK_D)) {
			pPosition_.x += playerSpeed_;
			rPosition_.x += playerSpeed_;
			r1Position_.x += playerSpeed_;
			target_.x += playerSpeed_;
			eye_.x += playerSpeed_;
			isRightMove_ = true;
		}
	}
	if (!input_->PushKey(DIK_D)) {
			isRightMove_ = false;
	}
	
	// 右へ行くときの傾き処理
	if (isRightMove_) {
		if (pRotation_.z >= playerSlopeMax_) {
			pRotation_.z -= playerRotSpeed_;
		}
		if (pRotation_.y <= playerSlopeMaxY_) {
			pRotation_.y += playerRotSpeed_;
		}
	}
	else {
		if (pRotation_.z <= playerSlopeMin_) {
			pRotation_.z += playerRotSpeed_;
		}
		if (pRotation_.y >= playerSlopeMin_) {
			pRotation_.y -= playerRotSpeed_;
		}
	}
	
	// ブースト時の回転処理
	if (input_->TriggerKey(DIK_U) || pPosition_.z == 100.000114f) {
		isEaseFlag_ = true;
		isRot_ = true;
		eFrame_ = 0;
	}
	if (isRot_) {
		if (isEaseFlag_) {
			eFrame_++;
		}

		// 自機の回転
		pRotation_.z = EasingManager::EaseOutQuintP(eFrame_, 0, 360, 60);

		// 回転最大時の値の初期化
		if (pRotation_.z == 360.0f) {
			pRotation_.z = 0.0f;
			isRot_ = false;
		}
	}

	if (pPosition_.z >= 100) {
		isReticle_ = false;
		isGameClearStaging_ = true;
		pPosition_.y += 0.3f;
		pPosition_.z += 2.0f;
		rPosition_.z += 0.1f;
		r1Position_.z += 0.1f;
		target_ = pPosition_;
	}
	if (pPosition_.z >= 150) {
		isEndStaging_ = true;
	}
	if (isEndStaging_) {
		isPostE_ = true;
	}

	playerO3_->SetPosition(pPosition_);
	playerO3_->SetRotation(pRotation_);
	reticleO3_->SetPosition(rPosition_);
	reticleO3_->SetRotation(rRotation_);
	reticle1O3_->SetPosition(r1Position_);
	reticle1O3_->SetRotation(r1Rotation_);
	Object3d::SetTarget(target_);
	Object3d::SetEye(eye_);

	// 弾の切り替え
	if (input_->TriggerKey(DIK_E)) {
		isBullet_ = 0;
	}

	// 自機のワールド座標から3Dレティクルのワールド座標を計算
	{
		// 自機から3Sレティクルへの距離
		//const float kDistancePlayerTo3DReticle = 50.0f;
		// 自機から3Dレティクルへのオフセット(z + 向き)
		DirectX::XMFLOAT3 offset = { 0,0,1.0f };
		// 自機のワールド行列の回転を反映
		//offset = DirectX::XMMatrixAffineTransformation()
	}

	// 自キャラの攻撃処理
	if (!isStartStaging_ && !isEndStaging_) {
		Attack();
	}

	// 弾更新
	for (PlayerBullet* bullet : pBullets_) {
		bullet->Update();
		pBulletP_ = bullet->GetPosition();
		if (bullet->GetPosition().z >= 10 && bullet->GetPosition().y > -4 && bullet->GetPosition().y < 5 && 
			bullet->GetPosition().x > -4 && bullet->GetPosition().x < 4) {

			damage_ = true;
		}
		if (bullet->GetPosition().z >= 10 && bullet->GetPosition().y > -4 && bullet->GetPosition().y < 5 &&
			bullet->GetPosition().x > 6 && bullet->GetPosition().x < 14) {

			damage01_ = true;
		}
		if (bullet->GetPosition().z >= 20 && bullet->GetPosition().y > -4 && bullet->GetPosition().y < 5 &&
			bullet->GetPosition().x > -12 && bullet->GetPosition().x < -4) {

			damage02_ = true;
		}
		if (bullet->GetPosition().z >= 30 && bullet->GetPosition().y > -4 && bullet->GetPosition().y < 4 &&
			bullet->GetPosition().x > -4 && bullet->GetPosition().x < 4) {

			damage03_ = true;
		}
	}

	if (pPosition_.x >= -2 && pPosition_.x <= 2 && pPosition_.y >= -2 && pPosition_.y <= 2 && pPosition_.z <= 9 && pPosition_.z >= 5 ||
		pPosition_.x <= -5 && pPosition_.x >= -9 && pPosition_.y >= -3 && pPosition_.y <= 1 && pPosition_.z <= 28 && pPosition_.z >= 23 || 
		pPosition_.x >= 4 && pPosition_.x <= 9 && pPosition_.y >= -1 && pPosition_.y <= 3 && pPosition_.z <= 22 && pPosition_.z >= 17 || 
		pPosition_.x >= -2 && pPosition_.x <= 2 && pPosition_.y >= -2 && pPosition_.y <= 2 && pPosition_.z <= 37 && pPosition_.z >= 32) {
		isEffect_ = true;
		isHp_ = true;
	}
	if (isEffect_) {
		effectTime_++;
	}
	if (isHp_) {
		hp_ -= 1;
	}
	if (effectTime_ >= 15) {
		isEffect_ = false;
		isHp_ = false;
		effectTime_ = 0;
	}

#ifdef _DEBUG

	if (input_->PushKey(DIK_UP)) {
		eye_.y -= 1;
	}
	if (input_->PushKey(DIK_DOWN)) {
		eye_.y += 1;
	}
	if (input_->PushKey(DIK_RIGHT)) {
		eye_.x += 1;
	}
	if (input_->PushKey(DIK_LEFT)) {
		eye_.x -= 1;
	}

	Object3d::SetEye(eye_);

	if (input_->TriggerKey(DIK_R)) {
		int a;
		a = 0;
	}

#endif
}

void Player::Draw()
{
	// プレイヤーのモデルの描画
	if (isPlayer_) {
		playerO3_->Draw();
	}

	/*if (!isStartStaging_) {
		if (isReticle_ == true) {
			reticleO3_->Draw();
			reticle1O3_->Draw();
		}
	}*/

	// 弾描画
	for (PlayerBullet* bullet : pBullets_) {
		bullet->Draw();
	}
}

void Player::Attack()
{
	// プレイヤーの攻撃処理
	if (input_->PushKey(DIK_SPACE) && isTime_ == false) {
		isTime_ = true;

		// 弾の速度
		const float bulletSpeed = 1.0f;
		DirectX::XMFLOAT3 velocity(0, 0, bulletSpeed);

		// 弾の生成と、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(pPosition_, velocity);

		// 球を登録する
		pBullets_.push_back(newBullet);
	}
	
	if (isTime_ == true) {
		time_++;
	}
	if (time_ >= 10) {
		time_ = 0;
		isTime_ = false;
	}
}

void Player::OnCollision([[maybe_unused]] const CollisionInfo& info)
{
	isPlayer_ = false;
}

void Player::Effect()
{
#pragma region パーティクルの描画

	// パーティクルの描画
	if (isEffect_) {
		particleMan_->Draw();
	}
	blackSmokeMan_->Draw();

#pragma endregion
}

void Player::Shake()
{
	// 乱数を取得
	std::random_device rnd;
	// 乱数のシードを設定
	std::mt19937 mt(rnd());
	std::uniform_int_distribution<> rand(shakeMin_, shakeMax_);

	shakePos_ = rand(mt) / shakeMd_;
	shakeEye_ = rand(mt) / shakeMdM_;
	
	shakeTimer_++;

	if (shakeTimer_ >= defaultShakeTimer_) {
		pPosition_.x -= shakePos_;
		eye_.x -= shakePos_;
	}
	if (shakeTimer_ >= shakeTimer10_) {
		pPosition_.x -= shakeEye_;
		eye_.x -= shakeEye_;
	}
	if (shakeTimer_ >= shakeTimer20_) {
		shakeTimer_ = defaultShakeTimer_;
	}
	
	Object3d::SetEye(eye_);
	playerO3_->SetPosition(pPosition_);
}
