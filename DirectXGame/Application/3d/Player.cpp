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

Player::Player(){}

Player::~Player(){}

std::unique_ptr<Player> Player::Create(Model* model)
{
	// 3Dオブジェクトのインスタンスを生成
	std::unique_ptr<Player> instance = std::make_unique<Player>();
	if (instance == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!instance->Initialize()) {
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
	dxCommon_ = MyEngine::DirectXCommon::GetInstance();
	input_ = MyEngine::Input::GetInstance();

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
	playerM_ = Model::LoadFromOBJ("fighter");
	reticleM_ = Model::LoadFromOBJ("reticle");
	reticle1M_ = Model::LoadFromOBJ("reticle1");
	// 3Dオブジェクト生成
	playerO3_ = Object3d::Create();
	reticleO3_ = Object3d::Create();
	reticle1O3_ = Object3d::Create();
	// オブジェクトにモデルをひも付ける
	playerO3_->SetModel(playerM_.get());
	reticleO3_->SetModel(reticleM_.get());
	reticle1O3_->SetModel(reticle1M_.get());
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
	particle_ = MyEngine::Particle::LoadFromOBJ("bomb.png");
	blackSmoke_ = MyEngine::Particle::LoadFromOBJ("bombEffect.png");
	rotationParticle_ = MyEngine::Particle::LoadFromOBJ("pSpin.png");
	// パーティクルの生成
	particleMan_ = MyEngine::ParticleManager::Create();
	blackSmokeMan_ = MyEngine::ParticleManager::Create();
	rotationParticleMan_ = MyEngine::ParticleManager::Create();
	// パーティクルマネージャーにパーティクルを割り当てる
	particleMan_->SetModel(particle_.get());
	blackSmokeMan_->SetModel(blackSmoke_.get());
	rotationParticleMan_->SetModel(rotationParticle_.get());

	return true;
}

void Player::Update()
{
	// 3Dオブジェクト更新
	playerO3_->Update();
	reticleO3_->Update();
	reticle1O3_->Update();

	// パーティクルの更新
	particleMan_->Update();
	blackSmokeMan_->Update();
	rotationParticleMan_->Update();

	// デスフラグの立った弾を削除
	pBullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		if (bullet->GetIsDead()) {
			return true;
		}
		return false;
	});

	// パーティクルの実行
	if (isDamageEffect_) {
		for (int i = 0; i < 5; i++) {
			// X,Y,Zすべて[-5.0f,+5.0f]でランダムに分布
			const float md_pos = 0.7f;
			XMFLOAT3 pos{};
			pos.x = (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f + position_.x;
			pos.y = (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f + position_.y;
			pos.z = (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f + position_.z;
			// X,Y,Z全て[-0.05f,+0.05f]でランダム分布
			const float md_vel = 0.1f;
			XMFLOAT3 vel{};
			vel.x = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
			vel.y = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
			vel.z = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
			// 重力に見立ててYのみ[-0.001f,0]でランダム分布
			XMFLOAT3 acc{};
			const float md_acc = 0.001f;
			acc.y = (float)rand() / RAND_MAX * md_acc;

			// 追加
			particle_->Add(particleLife, pos, vel, acc, particleStartScale_, particleEndScale_);
		}
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

	if (input_->TriggerKey(DIK_U) && !isRotationParticle_) {

		// パーティクルの実行
		for (int i = 0; i < rotationParticleMax_; i++) {
			// X,Y,Zすべて[-5.0f,+5.0f]でランダムに分布
			const float md_pos = 1.5f;
			const float md = 2.0f;
			DirectX::XMFLOAT3 pos{};
			pos.x = (float)rand() / RAND_MAX * md_pos - md_pos / md;
			pos.y = (float)rand() / RAND_MAX * md_pos - md_pos / md;
			pos.z = (float)rand() / RAND_MAX * md_pos - md_pos / md;
			// X,Y,Z全て[-0.05f,+0.05f]でランダム分布
			const float md_vel_x = 0.1f;
			const float md_vel_y = 0.1f;
			const float md_vel_z = 0.1f;
			DirectX::XMFLOAT3 vel{};
			vel.x = (float)rand() / RAND_MAX * md_vel_x - md_vel_x / md;
			vel.y = (float)rand() / RAND_MAX * md_vel_y - md_vel_y / md;
			vel.z = (float)rand() / RAND_MAX * md_vel_z - md_vel_z / md;
			// 重力に見立ててYのみ[-0.001f,0]でランダム分布
			DirectX::XMFLOAT3 acc{};
			const float md_acc_x = 0.001f;
			const float md_acc_y = 0.001f;
			acc.x = (float)rand() / RAND_MAX * md_acc_x;
			acc.y = (float)rand() / RAND_MAX * md_acc_y;

			// 追加
			rotationParticle_->Add(rotationParticleLife, pos, vel, acc, rotationParticleStartScale_, rotationParticleEndScale_);
		}
	}

	startCount_++;
	if (isStartStaging_) {
		pPosition_.z += startSpeed_;
		rPosition_.z += startSpeed_;
		r1Position_.z += startSpeed_;
		eye_.z += 0.4f;
		if (pPosition_.z >= -50) {
			isStartStaging_ = false;
		}
	}
	if (!isStartStaging_) {
		isReticle_ = true;
		pPosition_.z += 1.0f;
		rPosition_.z += 1.0f;
		r1Position_.z += 1.0f;
		eye_.z += 1.0f;
		target_ = pPosition_;
	}
	
	//===== プレイヤーの移動範囲の制限 =====//
	// 左右への移動範囲制限
	if (isGameClearStaging_ == false && isGameOverStaging_ == false) {
		if (pPosition_.x <= -15) {
			pPosition_.x += playerSpeed_;
			rPosition_.x += playerSpeed_;
			r1Position_.x += playerSpeed_;
			target_.x += playerSpeed_;
			eye_.x += playerSpeed_;
		}
		if (pPosition_.x >= 15) {
			pPosition_.x -= playerSpeed_;
			rPosition_.x -= playerSpeed_;
			r1Position_.x -= playerSpeed_;
			target_.x -= playerSpeed_;
			eye_.x -= playerSpeed_;
		}
		// 上下への移動範囲制限
		if (pPosition_.y <= -10) {
			pPosition_.y += playerSpeed_;
			rPosition_.y += playerSpeed_;
			r1Position_.y += playerSpeed_;
			target_.y += playerSpeed_;
			eye_.y += playerSpeed_;
		}
		if (pPosition_.y >= 10) {
			pPosition_.y -= playerSpeed_;
			rPosition_.y -= playerSpeed_;
			r1Position_.y -= playerSpeed_;
			target_.y -= playerSpeed_;
			eye_.y -= playerSpeed_;
		}
	}
	//===== プレイヤーの移動処理 =====//

	// 上への移動処理
	if (!isStartStaging_ && !isGameClearStaging_ && !isGameOverStaging_) {
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
	if (!isStartStaging_ && !isGameClearStaging_ && !isGameOverStaging_) {
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
	if (!isStartStaging_ && !isGameClearStaging_ && !isGameOverStaging_) {
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
	if (!isStartStaging_ && !isGameClearStaging_ && !isGameOverStaging_) {
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
	
	if (isBossDead_ == true) {
		isReticle_ = false;
		isGameClearStaging_ = true;
		pPosition_.y += 0.3f;
		pPosition_.z += 2.0f;
		rPosition_.z += 0.1f;
		r1Position_.z += 0.1f;
		target_ = pPosition_;
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
	if (!isStartStaging_ && !isGameClearStaging_) {
		Attack();
	}

	// 弾更新
	for (std::unique_ptr<PlayerBullet>& bullet : pBullets_) {
		bullet->Update();
	}
	if (isHit_) {
		isDamageEffect_ = true;
	}
	if (isDamageEffect_) {
		if (--effectTime_ <= 0) {
			isDamageEffect_ = false;
			effectTime_ = 10;
		}
	}

	if (pPosition_.z >= 930) {
		isBossStaging_ = true;
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
	if (input_->PushKey(DIK_I)) {
		
		target_.y -= 1;
	}
	if (input_->PushKey(DIK_K)) {
		
		target_.y += 1;
	}
	if (input_->PushKey(DIK_L)) {
		
		target_.x += 1;
	}
	if (input_->PushKey(DIK_J)) {
		
		target_.x -= 1;
	}

	Object3d::SetEye(eye_);

#endif
}

void Player::Draw()
{
	// プレイヤーのモデルの描画
	playerO3_->Draw();

	if (isReticle_ == true && isGameOverStaging_ == false) {
		reticleO3_->Draw();
		reticle1O3_->Draw();
	}

	// 弾描画
	for (std::unique_ptr<PlayerBullet>& bullet : pBullets_) {
		bullet->Draw();
	}
}

void Player::Attack()
{
	// プレイヤーの攻撃処理
	if (input_->PushKey(DIK_SPACE) && isTime_ == false) {
		isTime_ = true;

		// 弾の速度
		const float bulletSpeed = 4.0f;
		DirectX::XMFLOAT3 velocity(0, 0, bulletSpeed);

		// 弾の生成と、初期化
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(pPosition_, velocity);

		// 球を登録する
		pBullets_.push_back(std::move(newBullet));
	}
	
	if (isTime_ == true) {
		const float bulletTime = 10.0f;
		if (++time_ >= bulletTime) {
			time_ = 0;
			isTime_ = false;
		}
	}
}

void Player::OnCollision([[maybe_unused]] const CollisionInfo& info)
{
}

void Player::Effect()
{
#pragma region パーティクルの描画

	// パーティクルの描画
	particleMan_->Draw();
	if (isSpinEffect_ == true) {
		rotationParticleMan_->Draw();
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
	std::uniform_int_distribution<> rand1(shakeMin_, shakeMax_);

	shakeDefaultPos_ = pPosition_.x;
	shakeDefaultEye_ = eye_.x;

	shakePosX_ = rand(mt) / shakeMdX_;
	shakePosX2_ = rand(mt) / shakeMdMX_;

	shakePosY_ = rand1(mt) / shakeMdY_;
	shakePosY2_ = rand1(mt) / shakeMdMY_;
	
	shakeTimer_++;

	if (shakeTimer_ >= defaultShakeTimer_) {
		pPosition_.x -= shakePosX_;
		rPosition_.x -= shakePosX_;
		r1Position_.x -= shakePosX_;
		eye_.x -= shakePosX_;
		pPosition_.y -= shakePosY_;
		rPosition_.y -= shakePosY_;
		r1Position_.y -= shakePosY_;
		eye_.y -= shakePosY_;
	}
	if (shakeTimer_ >= shakeTimer10_) {
		pPosition_.x -= shakePosX2_;
		rPosition_.x -= shakePosX2_;
		r1Position_.x -= shakePosX2_;
		eye_.x -= shakePosX2_;
		pPosition_.y -= shakePosY2_;
		rPosition_.y -= shakePosY2_;
		r1Position_.y -= shakePosY2_;
		eye_.y -= shakePosY2_;
	}
	if (shakeTimer_ >= shakeTimer20_) {
		pPosition_.x = shakeDefaultPos_;
		rPosition_.x = shakeDefaultPos_;
		r1Position_.x = shakeDefaultPos_;
		eye_.x = shakeDefaultEye_;
		shakeTimer_ = defaultShakeTimer_;
	}
	
	Object3d::SetEye(eye_);
	playerO3_->SetPosition(pPosition_);
	reticleO3_->SetPosition(rPosition_);
	reticle1O3_->SetPosition(r1Position_);
}

void Player::Spin()
{
	// イージングをオンにする
	if (isRot_) {
		
		eFrame_++;

		// 自機の回転
		pRotation_.z = EasingManager::EaseOutQuintP(eFrame_, 0, 360, 60);

		// 回転最大時の値の初期化
		if (pRotation_.z == 360.0f) {
			pRotation_.z = 0.0f;
			isRot_ = false;
			eFrame_ = 0;
		}
	}
}

void Player::OnCollision()
{
}
