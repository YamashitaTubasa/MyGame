/**
 * @file Player.cpp
 * @brief プレイヤーの挙動に関するクラス
 * @author Yamashita Tubasa
 */

#include "Player.h"

#include "SphereCollider.h"

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
	pPosition_ = { 0,-2,-55 };
	pRotation_ = { 0,0,0 };
	playerO3_->SetPosition(pPosition_);
	playerO3_->SetScale({ 5, 5, 5 });
	playerO3_->SetRotation(pRotation_);
	rPosition_ = { 0,-2,-45 };
	rRotation_ = { 0,90,0 };
	reticleO3_->SetPosition(rPosition_);
	reticleO3_->SetScale(rScale_);
	reticleO3_->SetRotation(rRotation_);
	r1Position_ = { 0,-2.5f,-40 };
	r1Rotation_ = { 0,90,0 };
	reticle1O3_->SetPosition(r1Position_);
	reticle1O3_->SetScale(r1Scale_);
	reticle1O3_->SetRotation(r1Rotation_);

	Object3d::SetEye(eye_);
	Object3d::SetTarget(target_);

	// OBJの名前を指定してモデルデータを読み込む
	particle_ = Particle::LoadFromOBJ("bombEffect.png");
	// パーティクルの生成
	particleMan_ = ParticleManager::Create();
	// パーティクルマネージャーにパーティクルを割り当てる
	particleMan_->SetModel(particle_);

	return true;
}

void Player::Update()
{
	input_ = Input::GetInstance();

	// 3Dオブジェクト更新
	playerO3_->Update();
	reticleO3_->Update();
	reticle1O3_->Update();

	// パーティクルの更新
	particleMan_->Update();

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
	particleMan_->Execution(particle_, 0.0f, 0.0f, 0.0f, 20, 0.9f, 0.0f);

	startCount_++;
	if (startCount_ >= 90) {
		isStartStaging_ = false;
	}
	if (!isStartStaging_) {
		startCount_ = 0;
		isReticle_ = true;
	}
	if (isStartStaging_) {
		pPosition_.z += 0.1f;
		rPosition_.z += 0.1f;
		r1Position_.z += 0.1f;
		if (eye_.z <= -45.0f) {
			eye_.z += 0.4f;
		}
		if (eye_.y >= 0.0f) {
			eye_.y -= 0.4f;
		}
	}
	if (!isStartStaging_) {
		if (pPosition_.z < 100) {
			pPosition_.z += 0.4f;
			rPosition_.z += 0.4f;
			r1Position_.z += 0.4f;
			target_.z += 0.4f;
			eye_.z += 0.4f;
		}
	}

	//===== プレイヤーの移動範囲の制限 =====//
	// 左右への移動範囲制限
	if (!isEndStaging_) {
		if (pPosition_.x <= -8) {
			pPosition_.x += 0.3f;
			rPosition_.x += 0.3f;
			r1Position_.x += 0.3f;
			target_.x += 0.3f;
			eye_.x += 0.3f;
		}
		if (pPosition_.x >= 8) {
			pPosition_.x -= 0.3f;
			rPosition_.x -= 0.3f;
			r1Position_.x -= 0.3f;
			target_.x -= 0.3f;
			eye_.x -= 0.3f;
		}
		// 上下への移動範囲制限
		if (pPosition_.y <= -6) {
			pPosition_.y += 0.3f;
			rPosition_.y += 0.3f;
			r1Position_.y += 0.3f;
			target_.y += 0.3f;
			eye_.y += 0.3f;
		}
		if (pPosition_.y >= 6) {
			pPosition_.y -= 0.3f;
			rPosition_.y -= 0.3f;
			r1Position_.y -= 0.3f;
			target_.y -= 0.3f;
			eye_.y -= 0.3f;
		}
	}
	//===== プレイヤーの移動処理 =====//
	// 上への移動処理
	if (!isStartStaging_ && !isEndStaging_) {
		if (input_->PushKey(DIK_W)) {
			pPosition_.y += 0.3f;
			rPosition_.y += 0.3f;
			r1Position_.y += 0.3f;
			target_.y += 0.3f;
			eye_.y += 0.3f;
			isUpMove_ = true;
		}
	}
	if (!input_->PushKey(DIK_W)) {
			isUpMove_ = false;
	}
	
	// 上へ行くときの傾き処理
	if (isUpMove_) {
		if (pRotation_.x >= -20) {
			pRotation_.x -= 1.0f;
		}
	}
	else {
		if (pRotation_.x <= 0) {
			pRotation_.x += 1.0f;
		}
	}

	// 左への移動処理
	if (!isStartStaging_ && !isEndStaging_) {
		if (input_->PushKey(DIK_A)) {
			pPosition_.x -= 0.3f;
			rPosition_.x -= 0.3f;
			r1Position_.x -= 0.3f;
			target_.x -= 0.3f;
			eye_.x -= 0.3f;
			isLeftMove_ = true;
		}
	}
	if (!input_->PushKey(DIK_A)) {
			isLeftMove_ = false;
	}
	
	// 左へ行くときの機体の傾き処理
	if (isLeftMove_) {
		if (pRotation_.z <= 20.0f) {
			pRotation_.z += 1.0f;
		}
		if (pRotation_.y >= -10.0f) {
			pRotation_.y -= 1.0f;
		}
	}
	else {
		if (pRotation_.z >= 0.0f) {
			pRotation_.z -= 1.0f;
		}
		if (pRotation_.y <= 0.0f) {
			pRotation_.y += 1.0f;
		}
	}

	// 下への移動処理
	if (!isStartStaging_ && !isEndStaging_) {
		if (input_->PushKey(DIK_S)) {
			pPosition_.y -= 0.3f;
			rPosition_.y -= 0.3f;
			r1Position_.y -= 0.3f;
			target_.y -= 0.3f;
			eye_.y -= 0.3f;
			isDownMove_ = true;
		}
	}
	if (!input_->PushKey(DIK_S)) {
			isDownMove_ = false;
	}
	
	// 下へ行く時の傾き処理
	if (isDownMove_) {
		if (pRotation_.x <= 20) {
			pRotation_.x += 1.0f;
		}
	}
	else {
		if (pRotation_.x >= 0) {
			pRotation_.x -= 1.0f;
		}
	}

	// 右への移動処理
	if (!isStartStaging_ && !isEndStaging_) {
		if (input_->PushKey(DIK_D)) {
			pPosition_.x += 0.3f;
			rPosition_.x += 0.3f;
			r1Position_.x += 0.3f;
			target_.x += 0.3f;
			eye_.x += 0.3f;
			isRightMove_ = true;
		}
	}
	if (!input_->PushKey(DIK_D)) {
			isRightMove_ = false;
	}
	
	// 右へ行くときの傾き処理
	if (isRightMove_) {
		if (pRotation_.z >= -20.0f) {
			pRotation_.z -= 1.0f;
		}
		if (pRotation_.y <= 10.0f) {
			pRotation_.y += 1.0f;
		}
	}
	else {
		if (pRotation_.z <= 0.0f) {
			pRotation_.z += 1.0f;
		}
		if (pRotation_.y >= 0.0f) {
			pRotation_.y -= 1.0f;
		}
	}

	// ブースト時の回転処理
	if (input_->TriggerKey(DIK_U) || pPosition_.z == 5.09989643f && pPosition_.x >= -2 && pPosition_.x <= 2 && pPosition_.y >= 2 && pPosition_.y <= 6) {
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
		isEndStaging_ = true;
		pPosition_.y += 0.2f;
		pPosition_.z += 1.2f;
		rPosition_.z += 0.1f;
		r1Position_.z += 0.1f;
		target_ = pPosition_;
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
	Attack();

	// 弾更新
	for (PlayerBullet* bullet : pBullets_) {
		bullet->Update();
		pBulletP_ = bullet->GetPosition();
		if (bullet->GetPosition().z >= 10 && bullet->GetPosition().y > -4 && bullet->GetPosition().y < 4 && 
			bullet->GetPosition().x > -4 && bullet->GetPosition().x < 4) {
			hp_ -= 1;
			enemy_->SetIsDead(true);
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
}

void Player::Draw()
{
	// プレイヤーのモデルの描画
	if (isPlayer_) {
		playerO3_->Draw();
	}

	if (isReticle_ == true) {
		reticleO3_->Draw();
		reticle1O3_->Draw();
	}

	// 弾描画
	for (PlayerBullet* bullet : pBullets_) {
		bullet->Draw();
	}
}

void Player::Attack()
{
	// プレイヤーの攻撃処理
	if (input_->TriggerKey(DIK_SPACE) && isTime_ == false) {
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
	if (time_ >= 100) {
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

#pragma endregion
}
