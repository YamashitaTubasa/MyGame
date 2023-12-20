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
	// 弾の解放
	for (PlayerBullet* bullet : pBullets_) {
		delete bullet;
	}
}

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
	dxCommon_ = DirectXCommon::GetInstance();

	if (!Object3d::Initialize()) {
		return false;
	}

	enemy_ = std::make_unique<Enemy>();

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
	bossModel_ = Model::LoadFromOBJ("enemy");
	// 3Dオブジェクト生成
	playerO3_ = Object3d::Create();
	reticleO3_ = Object3d::Create();
	reticle1O3_ = Object3d::Create();
	bossObj_ = Object3d::Create();
	// オブジェクトにモデルをひも付ける
	playerO3_->SetModel(playerM_.get());
	reticleO3_->SetModel(reticleM_.get());
	reticle1O3_->SetModel(reticle1M_.get());
	bossObj_->SetModel(bossModel_.get());
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
	bossObj_->SetPosition(bossPos_);
	bossObj_->SetScale(bossScale_);
	bossObj_->SetRotation(bossRot_);

	Object3d::SetEye(eye_);
	target_ = pPosition_;
	Object3d::SetTarget(target_);

	// OBJの名前を指定してモデルデータを読み込む
	particle_ = Particle::LoadFromOBJ("bombEffect.png");
	blackSmoke_ = Particle::LoadFromOBJ("bombEffect.png");
	rotationParticle_ = Particle::LoadFromOBJ("23682202.png");
	// パーティクルの生成
	particleMan_ = ParticleManager::Create();
	blackSmokeMan_ = ParticleManager::Create();
	rotationParticleMan_ = ParticleManager::Create();
	// パーティクルマネージャーにパーティクルを割り当てる
	particleMan_->SetModel(particle_.get());
	blackSmokeMan_->SetModel(blackSmoke_.get());
	rotationParticleMan_->SetModel(rotationParticle_.get());

	// スプライト
	sprite_ = std::make_unique<Sprite>();
	spriteCommon_ = sprite_->SpriteCommonCreate();
	// スプライト用パイプライン生成呼び出し
	PipelineSet spritePipelineSet = sprite_->SpriteCreateGraphicsPipeline();

	// 敵のHP
	enemyHp_ = std::make_unique<Sprite>();
	enemyHp_->LoadTexture(spriteCommon_, 7, L"Resources/Image/enemyHp.png");
	enemyHp_->SpriteCreate(1280, 720, 7, spriteCommon_, XMFLOAT2(1.0f, 0.0f), false, false);
	enemyHp_->SetColor(XMFLOAT4(1, 1, 1, 1));
	enemyHp_->SetPosition(enemyHpPosition_);
	enemyHp_->SetScale(enemyHpScale_);
	enemyHp_->SetRotation(0.0f);
	enemyHp_->SetAlpha(enemyAlpha_);
	enemyHp_->SpriteTransferVertexBuffer(enemyHp_.get(), spriteCommon_, 7);
	enemyHp_->SpriteUpdate(enemyHp_.get(), spriteCommon_);
	// 敵のHPバー
	enemyHpBar_ = std::make_unique<Sprite>();
	enemyHpBar_->LoadTexture(spriteCommon_, 8, L"Resources/Image/enemyHpBar.png");
	enemyHpBar_->SpriteCreate(1280, 720, 8, spriteCommon_, XMFLOAT2(1.0f, 0.0f), false, false);
	enemyHpBar_->SetColor(XMFLOAT4(1, 1, 1, 1));
	enemyHpBar_->SetPosition(enemyHpBarPosition_);
	enemyHpBar_->SetScale(XMFLOAT2(502 * 1, 22 * 1));
	enemyHpBar_->SetRotation(0.0f);
	enemyHpBar_->SetAlpha(enemyAlpha_);
	enemyHpBar_->SpriteTransferVertexBuffer(enemyHpBar_.get(), spriteCommon_, 8);
	enemyHpBar_->SpriteUpdate(enemyHpBar_.get(), spriteCommon_);
	// 敵のHP背景
	enemyHpBack_ = std::make_unique<Sprite>();
	enemyHpBack_->LoadTexture(spriteCommon_, 9, L"Resources/Image/enemyHpBack.png");
	enemyHpBack_->SpriteCreate(1280, 720, 9, spriteCommon_, XMFLOAT2(1.0f, 0.0f), false, false);
	enemyHpBack_->SetColor(XMFLOAT4(1, 1, 1, 1));
	enemyHpBack_->SetPosition(enemyHpBackPosition_);
	enemyHpBack_->SetScale(XMFLOAT2(500 * 1, 20 * 1));
	enemyHpBack_->SetRotation(0.0f);
	enemyHpBack_->SetAlpha(enemyAlpha_);
	enemyHpBack_->SpriteTransferVertexBuffer(enemyHpBack_.get(), spriteCommon_, 9);
	enemyHpBack_->SpriteUpdate(enemyHpBack_.get(), spriteCommon_);

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
	bossObj_->Update();

	// パーティクルの更新
	particleMan_->Update();
	blackSmokeMan_->Update();
	rotationParticleMan_->Update();

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
	particleMan_->Execution(particle_.get(), particleBombPosX_, particleBombPosY_, particleBombPosZ_, particleLife, particleStartScale_, particleEndScale_);

	if (hp_ <= -20) {

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

	//if (damage_ == true) {

	//	// パーティクルの実行
	//	for (int i = 0; i < 3; i++) {
	//		// X,Y,Zすべて[-5.0f,+5.0f]でランダムに分布
	//		const float md_pos = 0.5f;
	//		const float md = 2.0f;
	//		DirectX::XMFLOAT3 pos{};
	//		pos.x = (float)rand() / RAND_MAX * md_pos - md_pos / md;
	//		pos.y = (float)rand() / RAND_MAX * md_pos - md_pos / md;
	//		pos.z = (float)rand() / RAND_MAX * md_pos - md_pos / md;
	//		// X,Y,Z全て[-0.05f,+0.05f]でランダム分布
	//		const float md_vel_x = 0.01f;
	//		const float md_vel_y = 0.05f;
	//		const float md_vel_z = 0.05f;
	//		DirectX::XMFLOAT3 vel{};
	//		vel.x = (float)rand() / RAND_MAX * md_vel_x - md_vel_x / md;
	//		vel.y = (float)rand() / RAND_MAX * md_vel_y - md_vel_y / md;
	//		vel.z = (float)rand() / RAND_MAX * md_vel_z - md_vel_z / md;
	//		// 重力に見立ててYのみ[-0.001f,0]でランダム分布
	//		DirectX::XMFLOAT3 acc{};
	//		const float md_acc_x = 0.00f;
	//		const float md_acc_y = 0.04f;
	//		acc.x = (float)rand() / RAND_MAX * md_acc_x;
	//		acc.y = (float)rand() / RAND_MAX * md_acc_y;

	//		// 追加
	//		particle_->Add(15, pos, vel, acc, 1.0f, 0.0f);
	//	}
	//}

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
		if (pPosition_.z < 300) {
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

	if (isBoss_ == true) {
		isReticle_ = false;
		isGameClearStaging_ = true;
		pPosition_.y += 0.3f;
		pPosition_.z += 2.0f;
		rPosition_.z += 0.1f;
		r1Position_.z += 0.1f;
		target_ = pPosition_;
	}
	if (isBoss_ == true) {
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
		if (!damage_) {
			if (bullet->GetPosition().z >= 10 && bullet->GetPosition().y > -4 && bullet->GetPosition().y < 5 &&
				bullet->GetPosition().x > -4 && bullet->GetPosition().x < 4) {

				damage_ = true;
				bullet->SetIsDead(true);
			}
		}
		if (!damage01_) {
			if (bullet->GetPosition().z >= 10 && bullet->GetPosition().y > -4 && bullet->GetPosition().y < 5 &&
				bullet->GetPosition().x > 6 && bullet->GetPosition().x < 14) {

				damage01_ = true;
				bullet->SetIsDead(true);
			}
		}
		if (!damage02_) {
			if (bullet->GetPosition().z >= 20 && bullet->GetPosition().y > -4 && bullet->GetPosition().y < 5 &&
				bullet->GetPosition().x > -12 && bullet->GetPosition().x < -4) {

				damage02_ = true;
				bullet->SetIsDead(true);
			}
		}
		if (!damage03_) {
			if (bullet->GetPosition().z >= 30 && bullet->GetPosition().y > -4 && bullet->GetPosition().y < 4 &&
				bullet->GetPosition().x > -4 && bullet->GetPosition().x < 4) {

				damage03_ = true;
				bullet->SetIsDead(true);
			}
		}

		if (isBossStaging_ == true) {
			if (bullet->GetPosition().x <= bossPos_.x + 20 && bullet->GetPosition().x >= bossPos_.x - 20 &&
				bullet->GetPosition().y <= bossPos_.y + 10 && bullet->GetPosition().y >= bossPos_.y - 10 &&
				bullet->GetPosition().z <= bossPos_.z + 10 && bullet->GetPosition().z >= bossPos_.z - 10) {


				isBH_ = true;
				bullet->SetIsDead(true);
			}
		}
		if (isBH_ == true) {
			bossTimer_++;
			isBossHp_ = true;
			hp_ -= 1;
		}
		if (bossTimer_ >= 5) {
			isBossHp_ = false;
			bossTimer_ = 0;
			isBH_ = false;
		}
	}

	if (pPosition_.x <= bossPos_.x + 20 && pPosition_.x >= bossPos_.x - 20 &&
		pPosition_.y <= bossPos_.y + 10 && pPosition_.y >= bossPos_.y - 10 &&
		pPosition_.z <= bossPos_.z + 10 && pPosition_.z >= bossPos_.z - 10) {


		isBd_ = true;
	}

	if (pPosition_.x >= -2 && pPosition_.x <= 2 && pPosition_.y >= -2 && pPosition_.y <= 2 && pPosition_.z <= 9 && pPosition_.z >= 5) {
		if (!damage_) {
			isEffect_ = true;
			isHp_ = true;
		}
	}
	if (pPosition_.x >= 4 && pPosition_.x <= 9 && pPosition_.y >= -1 && pPosition_.y <= 3 && pPosition_.z <= 22 && pPosition_.z >= 17)
	{
		if (!damage01_) {
			isEffect_ = true;
			isHp_ = true;
		}
	}
	if (pPosition_.x <= -5 && pPosition_.x >= -9 && pPosition_.y >= -3 && pPosition_.y <= 1 && pPosition_.z <= 28 && pPosition_.z >= 23)
	{
		if (!damage02_) {
			isEffect_ = true;
			isHp_ = true;
		}
	}
	if (pPosition_.x >= -2 && pPosition_.x <= 2 && pPosition_.y >= -2 && pPosition_.y <= 2 && pPosition_.z <= 37 && pPosition_.z >= 32)
	{
		if (!damage03_) {
			isEffect_ = true;
			isHp_ = true;
		}
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

	if (pPosition_.x >= -4 && pPosition_.x <= 4 && pPosition_.y >= -4 && pPosition_.y <= 4 && pPosition_.z <= 11 && pPosition_.z >= 2) {
		if (input_->PushKey(DIK_U)) {
			damage_ = true;
		}
	}
	if (pPosition_.x >= 2 && pPosition_.x <= 11 && pPosition_.y >= -3 && pPosition_.y <= 5 && pPosition_.z <= 24 && pPosition_.z >= 14)
	{
		if (input_->PushKey(DIK_U)) {
			damage01_ = true;
		}
	}
	if (pPosition_.x <= -3 && pPosition_.x >= -11 && pPosition_.y >= -5 && pPosition_.y <= 3 && pPosition_.z <= 30 && pPosition_.z >= 20)
	{
		if (input_->PushKey(DIK_U)) {
			damage02_ = true;
		}
	}
	if (pPosition_.x >= -4 && pPosition_.x <= 4 && pPosition_.y >= -4 && pPosition_.y <= 4 && pPosition_.z <= 39 && pPosition_.z >= 29)
	{
		if (input_->PushKey(DIK_U)) {
			damage03_ = true;
		}
	}

	if (pPosition_.z >= 100) {
		isBossStaging_ = true;
	}

	if (isBossHp_) {
		if (enemyHpScale_.x >= 0) {
			enemyHpScale_.x -= enemyHpMove_.x;
			isBossDamage_ = true;
		}
	}
	enemyHp_->SetScale(enemyHpScale_);
	enemyHp_->SpriteTransferVertexBuffer(enemyHp_.get(), spriteCommon_, 7);
	enemyHp_->SpriteUpdate(enemyHp_.get(), spriteCommon_);

	// 敵のHPフェード処理
	if (isBossStaging_ == true) {
		if (enemyAlpha_ <= 1.0f) {
			enemyAlpha_ += 0.05f;
			enemyHp_->SetAlpha(enemyAlpha_);
			enemyHpBar_->SetAlpha(enemyAlpha_);
			enemyHpBack_->SetAlpha(enemyAlpha_);
		}
	}

	if (isBossDamage_) {
		bossDamageTimer_++;
		if (enemyHpScale_.x >= 0) {
			if (enemyHpScale_.x > 300 && enemyHpScale_.x <= 500) {
				enemyHpScale_.x -= 5;
			}
			if (enemyHpScale_.x <= 300 && enemyHpScale_.x > 100) {
				enemyHpScale_.x -= 5;
			}
		}
	}
	if (bossDamageTimer_ >= 20) {
		isBossDamage_ = false;
		bossDamageTimer_ = 0;
	}
	if (enemyHpScale_.x <= 0) {
		isBoss_ = true;
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

	if (isBossStaging_ == true && isBoss_ == false) {
		bossObj_->Draw();
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
	if (damage_ == true || damage01_ == true || damage02_ == true || damage03_ == true) {
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
		eye_.x -= shakePosX_;
		pPosition_.y -= shakePosY_;
		eye_.y -= shakePosY_;
	}
	if (shakeTimer_ >= shakeTimer10_) {
		pPosition_.x -= shakePosX2_;
		eye_.x -= shakePosX2_;
		pPosition_.y -= shakePosY2_;
		eye_.y -= shakePosY2_;
	}
	if (shakeTimer_ >= shakeTimer20_) {
		pPosition_.x = shakeDefaultPos_;
		eye_.x = shakeDefaultEye_;
		shakeTimer_ = defaultShakeTimer_;
	}
	
	Object3d::SetEye(eye_);
	playerO3_->SetPosition(pPosition_);
}

void Player::SpriteDraw()
{
	Sprite::PreDraw(DirectXCommon::GetInstance()->GetCommandList(), spriteCommon_);
	// 敵のHP
	if (isBossStaging_ == true && isBoss_ == false || isGameOverStaging_ == true) {

		enemyHpBar_->SpriteDraw(spriteCommon_);
		enemyHpBack_->SpriteDraw(spriteCommon_);
		enemyHp_->SpriteDraw(spriteCommon_);
	}
	Sprite::PostDraw();
}
